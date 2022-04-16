#include "p4exo9.h"


/*Un cityoyen soumet son vote, c'est a dire qu'il est ajoute en fin du fichier Pending_votes.txt*/
void submit_vote(Protected* pr){
    if(!pr)
        return;

    FILE * f = fopen("Pending_votes.txt", "a"); //ouverture du flux en mode append
    if(!f){
        printf("Erreur d'ouverture du fichier Pending_votes.txt");
        return;
    }
    char* pr_str = protected_to_str(pr);
    if (!pr_str){
        fclose(f);
        return;
    }
    fprintf(f, "%s\n", pr_str);
    free(pr_str);
    fclose(f);
}

/*Cree un bloc valide contenant les votes en attente dans le fichier ”Pending votes.txt”,
supprime le fichier ”Pending votes.txt” apres avoir cree le bloc,
et ecrit le bloc obtenu dans un fichier ”Pending block”*/
void create_block(CellTree* tree, Key* author, int d){
    //Lit les votes en attente dans Pending_votes.txt
    CellProtected* votes = read_protected("Pending_votes.txt");
    if (!votes || !author)
        return;

    //Compte le nombre de votes
    int nb_votes = 0;
    CellProtected* tmp = votes;
    while (tmp){
        nb_votes++;
        tmp = tmp->next;
    }

    //Recupere la valeur hachee du dernier bloc de l'arbre
    CellTree* last = last_node(tree);
    unsigned char *previous_hash;
    if (last == NULL) //cas de la racine
        previous_hash = NULL;
    else{ //duplique le previous_hash
        previous_hash = malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH + 1);
        if (!previous_hash)
            return ;
        int i;
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
            previous_hash[i] = last->block->previous_hash[i];
        previous_hash[i] = '\0';
    }

    //Alloue et initialise le bloc
    Block* b = init_block(author, votes, NULL, previous_hash, 0, nb_votes);
    if (!b){
        delete_list_cell(votes);
        return;
    }

    //Calcule la proof of work du bloc
    if (compute_proof_of_work(b, d) == 0){ //la valeur hachée du bloc est maj dans la fonction
        full_delete_block(b);
        return;
    }

    // //Ajoute le bloc dans l'arbre
    // CellTree *node = create_node(b);
    // if (!node){
    //     full_delete_block(b);
    //     return;
    // }
    // add_child(last, node);

    //Supprime Pending_votes.txt et ecrit le bloc dans Pending_block
    if (ecrire_block("Pending_block", b) == 0){
        // if (node->father != NULL){
        //     node->father->firstChild = NULL;
        // }
        // full_delete_node(node);
        full_delete_block(b);
        return;
    }
    remove("Pending_votes.txt");

    full_delete_block(b);
}


/*Verifie que le bloc dans Pending_block est valide. Si oui ecrit ce bloc dans un fichier name
dans le repertoire Blockchain. Pending_block est supprime*/
void add_block(int d, char* name){
    Block * b = lire_block("Pending_block");
    if (!b){
        return;
    }
    if (verify_block(b, d) == 1){ //si le bloc est valide
        char *filename = strjoin("Blockchain/", name, 0);
        if (!filename){
            full_delete_block(b);
            return;
        }
        if (ecrire_block(filename, b) == 0){
            full_delete_block(b);
            free(filename);
            return;
        } 
        free(filename);
    }
    full_delete_block(b);
    remove("Pending_block");
}

/*Retourne 0 si deux chaine sont identique, la difference des valeurs ascii des premiers caracteres non identiques sinon*/
int strcmp_unsigned(const unsigned char * s1, const unsigned char * s2){
    if (s1 == NULL || s2 == NULL){
        return 1;
    }
	int i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}


/*Lit tous les blos du repertoire Blockchain et retourne l'arbre correspondant a ces blocs*/
CellTree* read_tree(){
    DIR *rep = opendir("./Blockchain/");
	if (rep != NULL){
		struct	dirent	* dir;
        int nb_blocks = 0;

        //calcul du nombre de block dans Blockchain
		while ((dir = readdir(rep))){
			if (strcmp(dir->d_name, ".") != 0 && strcmp (dir->d_name, "..") != 0){
				nb_blocks++;
			}
		}
		closedir(rep);

        //allocation du tableau de blocks
        CellTree **T = malloc(sizeof(CellTree*) * nb_blocks);
        if( !T){
            closedir(rep);
            return NULL;
        }

        //remplissage du tableau de noeuds T
        Block * b;
        int i = 0;
        int j;
        rep = opendir("./Blockchain/");
        if (!rep){
            return NULL;
        }
        while ((dir = readdir(rep))){
			if (strcmp(dir->d_name, ".") != 0 && strcmp (dir->d_name, "..") != 0){
                
                char *filename = strjoin("Blockchain/", dir->d_name, 0);
                if (!filename){
                    for (j = 0; j < i; j++)
                        full_delete_node(T[j]);
                    free(T);
                    closedir(rep);
                    return NULL;
                }

                b = lire_block(filename);
                if (!b){
                    for (j = 0; j < i; j++)
                        full_delete_node(T[j]);
                    free(T);
                    free(filename);
                    closedir(rep);
                    return NULL;
                }
                free(filename);
                T[i] = create_node(b);
    
                if (!T[i]){
                    for (j = 0; j < i; j++)
                        full_delete_node(T[j]);
                    full_delete_block(b);
                    free(T);
                    closedir(rep);
                    return NULL;
                }
                i++;
			}
		}
        closedir(rep);

        //recherche des fils de chaque noeud
        for(i = 0; i < nb_blocks; i++){
            for (j = 0; j < nb_blocks; j++){
                if (strcmp_unsigned(T[i]->block->hash, T[j]->block->previous_hash) == 0){
                    add_child(T[i], T[j]);
                }
            }
        }

        //recherche de la racine
        CellTree *root;
        for(i = 0; i < nb_blocks; i++){
            if (T[i]->block->previous_hash == NULL){
                root = T[i];
                free(T); //libere le tableau
                return root;
            }
        }

        //si on a pas trouve de racine l'arbre est invalide
        for(i = 0; i < nb_blocks; i++){
            full_delete_node(T[i]);
        }
        free(T);
        return NULL;

	}
    return NULL;
}

Key* compute_winner_BT(CellTree* tree,CellKey* candidates,CellKey* voters,int sizeC,int sizeV){
    CellProtected* liste_votes=NULL;
    if(!tree){
        printf("je suis vide")
    }
    while(tree->block->votes){
        fusion_liste_protected(liste_votes,tree->block->votes);
        compute_winner_BT(tree->nextBro,candidates,voters,sizeC,sizeV);
        compute_winner_BT(tree->firstChild,candidates,voters,sizeC,sizeV);
    }
    supprime_declarations_non_valides(&liste_votes);
    Key* gagnant=compute_winner(liste_votes,candidates,voters,sizeC,sizeV);
    return gagnant;
}
