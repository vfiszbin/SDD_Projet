#include "p4exo8.h"

/*Crée et initialise un noeud dont la hauteur vaut 0*/
CellTree* create_node(Block* b){
    CellTree* T = (CellTree*)malloc(sizeof(CellTree));
    if(!T){
        return NULL;
    }
    T->block=b;
    T->father=NULL;
    T->firstChild=NULL;
    T->nextBro=NULL;
    T->height=0;
    return T;
}

/*Retourne l'entier max entre a et b*/
int max(int a, int b){
    if (a >= b)
        return a;
    return b;
}

/*Met a jour la hauteur du noeud father selon la hauteur d'un fils child. Retourne 1 si la hauteur du pere a ete modifiee, 0 sinon*/
int update_height(CellTree* father, CellTree* child){
    if(!father || !child){
        return -1;
    }
    int previous_height = father->height;
    father->height = max(father->height, child->height + 1); //max entre hauteur courante et celle du child + 1

    if(father->height==previous_height) //si la hauteur n'a pas changee
        return 0;
    else //si elle a change
        return 1;
}

/*Ajoute un fils child au noeud father, met a jour la hauteur de tous les ascendants*/
void add_child(CellTree* father, CellTree* child){
    if (!child)
        return;
    if(!father){ // arbre vide
        father = child;
        return ;
    }

    child->father = father; //assigne son pere au fils

    //Insertion du child
    if (father->firstChild == NULL){ //si le pere n'a pas deja un fils
        father->firstChild = child;
    }
    else{ //si le pere a deja un fils (ou plus d'un fils)
        CellTree * brother = father->firstChild;

        while (brother->nextBro != NULL){ //on cherche le frere qui n'a pas encore de frere suivant
            brother = brother->nextBro;
        }
        brother->nextBro = child; //insere le child comme le dernier frere parmi les fils du father
    }

    //Mise a jour des hauteurs de tous les ascendants
    CellTree *ascendant = father;
    CellTree *descendant = child;
    while (ascendant != NULL){ //remonte tous les ascendants
        if (update_height(ascendant, descendant) == 0) //maj la hauteur
            return; //termine prematurement la boucle si la hauteur de l'ascendant n'a pas eu besoin d'etre modifiee
        descendant = ascendant;
        ascendant = ascendant->father;
    }
}

// //la racine
// void print_tree(CellTree* arbre){
//     CellTree* copie;
//     copie=arbre;
//     if(!arbre){
//         printf("le noeud est vide");
//         printf("\n");
//     }else
//     {
//         if(arbre->father==NULL){
//             printf("%d,%s\n",copie->height,copie->block->hash);
//             printf("voici mon fils\n");
//             print_tree(copie->firstChild);
//         }else{
//             printf("%d,%s\n",copie->height,copie->block->hash);
//             printf("voici mon frere\n");
//             print_tree(copie->nextBro);
//             printf("voici mon fils\n");
//             print_tree(copie->firstChild);
//         }
//     }
// }

/*Affiche un representation 2D d'un arbre sur la console*/
void print_tree2D(CellTree *cell, int spaces){
    if (cell == NULL)
        return;
 
    spaces += 10; //augmente distance entre les noeuds
    
    //Parcours infixe (inversé) (droite, racine, gauche) de l'arbre
    print_tree2D(cell->nextBro, spaces); //affiche d'abord les fils droits
 
    printf("\n");
    for (int i = 10; i < spaces; i++)
        printf(" ");
    printf("%d,", cell->height);
    print_hash_sha256(cell->block->hash);
 
    print_tree2D(cell->firstChild, spaces); //affiche ensuite les fils gauche
}


/*Libere un noeud de l'arbre en appelant delete_block*/
void delete_node(CellTree* node){
    if(node){
        delete_block(node->block);
        free(node);
    }
}

/*Libere l'arbre avec delete_block*/
void delete_tree(CellTree* tree){
    if(tree){
        delete_tree(tree->firstChild);
        delete_tree(tree->nextBro);
        delete_node(tree);
    }
}

/*Libere un noeud de l'arbre en appelant full_delete_block*/
void full_delete_node(CellTree* node){
    if(node){
        full_delete_block(node->block);
        free(node);
    }
}

/*Libere l'arbre avec full_delete_block*/
void full_delete_tree(CellTree* tree){
    if(tree){
        full_delete_tree(tree->firstChild);
        full_delete_tree(tree->nextBro);
        full_delete_node(tree);
    }
}

/*Renvoie le noeud fils de cell avec la plus grand hauteur*/
CellTree* highest_child(CellTree* cell){
    CellTree* high;
    CellTree* brother;
    if(cell){
        if(cell->firstChild!=NULL){
            high = cell->firstChild;
            brother = cell->firstChild->nextBro;
            while(brother){
                if(high->height < brother->height){
                    high = brother;
                }
            brother = brother->nextBro;
            }
            return high;
        }
        return NULL;
    }
    return NULL;
}

/*Renvoie le dernier noeud de la chaine avec la plus grand hauteur dans l'arbre tree*/
CellTree* last_node(CellTree* tree){
    if (tree == NULL) //arbre vide
        return NULL;

    CellTree* high = highest_child(tree);
    if (high == NULL) //si le noeud n'a aucun fils il est lui meme le dernier noeud
        return tree;

    while (high->firstChild || high->nextBro){ //tant que le noeud a un fils ou un frere, il n'est pas 
        high = highest_child(high);
    }
    return high;
}

/*Alloue et initialise une copie du protected pr*/
Protected *duplicate_protected(Protected * pr){
    Key *k = (Key*)malloc(sizeof(Key));
    if (!k)
        return NULL;
    init_key(k,pr->pKey->val, pr->pKey->n);

    Signature *s = (Signature*)malloc(sizeof(Signature));
    if (!s){
        free(k);
        return NULL;
    }
    s->size = pr->sgn->size;
    s->content = (long *)malloc(sizeof(long) * s->size);
    if (!s->content){
        free(k);
        free(s);
        return NULL;
    }
    for (int i = 0; i < s->size ; i++) //recopie le tab de long
        s->content[i] = pr->sgn->content[i];
    
    char *mess = strdup(pr->mess);
    if (!mess){
        free(k);
        free(s);
        free(s->content);
        return NULL;
    }

    Protected * ret_pr = init_protected(k, mess, s);
    if (!ret_pr){
        free(k);
        free(s);
        free(s->content);
        free(mess);
        return NULL;
    }
    return ret_pr;
}

/*Fusionne deux listes chainees de declarations signees. Les elements de la list2 sont inseres dans la list1.
Retourne le pointeur vers la nouvelle tete de list1*/
CellProtected* fusion_liste_protected(CellProtected* list1, CellProtected* list2){
    CellProtected *tmp;
    Protected * pr;
    CellProtected *cpr;
    while(list2 != NULL){
        tmp = list2;
        list2 = list2->next;

        //la cell protected est dupliquee pour pouvoir liberer les deux separement
        pr = duplicate_protected(tmp->data); 
        if (!pr){
            delete_list_cell(list1);
            return NULL;
        }
        cpr = create_cell_protected(pr);
        if (!cpr){
            delete_protected(pr);
            delete_list_cell(list1);
            return NULL;
        }
        add_cellProtected_to_front(&list1, cpr); //ajout de la cell en tete de list1
    }
    return list1;
}

/*Retourne la liste obtenue par fusion des listes chainees de declarations contenues dans les blocs de la plus longue chaine
de l'arbre tree*/
CellProtected* fusion_votes_arbre(CellTree* tree){
    if (tree == NULL) //arbre vide
        return NULL;
    
    CellProtected* liste_votes = NULL; 
    liste_votes = fusion_liste_protected(liste_votes, tree->block->votes); //prend en compte les votes de la racine
    if (!liste_votes)
        return NULL;
    
    CellTree* high = highest_child(tree);
    
    while(high){
    
        liste_votes = fusion_liste_protected(liste_votes, high->block->votes);
        if (!liste_votes)
            return NULL;

        high = highest_child(high);
    }
    return liste_votes;
}