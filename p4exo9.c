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
        printf("%s\n",filename);
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

Key* compute_winner_BT(CellTree* tree,CellKey* candidates,CellKey* voters,int sizeC,int sizeV){
    CellProtected* liste_votes=NULL;
    if(!tree){
        return;
    }
    while(tree->block->votes){
        fusion_liste_protected(liste_votes,tree->block->votes);
    }
    supprime_declarations_non_valides(&liste_votes);
    Key* gagnant=compute_winner(liste_votes,candidates,voters,sizeC,sizeV);
    return gagnant;
}
