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
    if(!father || !child){
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
    printf("(%d,%s)\n", cell->height ,cell->block->hash);
 
    print_tree2D(cell->firstChild, spaces); //affiche ensuite les fils gauche
}


//Libere un noeud de l'arbre
void delete_node(CellTree* node){
    if(node){
        delete_block(node->block);
        free(node);
    }
}

//Libere l'arbre
void delete_tree(CellTree* tree){
    if(tree){
        delete_tree(tree->firstChild);
        delete_tree(tree->nextBro);
        delete_node(tree);
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

CellTree* last_node(CellTree* tree){
    CellTree* high=highest_child(tree);
    CellTree* tmp=high->firstChild;
    if(high!=NULL){
        while(tmp!=NULL){
            tmp=tmp->firstChild;
        }
        return tmp;
    }
    return NULL;
}

CellProtected* fusion_liste_protected(CellProtected* list1,CellProtected* list2){
    CellProtected* listfusion = (CellProtected*)malloc(1024*sizeof(CellProtected));
    if(!listfusion){
        return NULL;
    }
    if(!list1){
        return list2;
    }
    if(!list2){
        return list1;
    }
    while(list1!=NULL){
        listfusion->data=list1->data;
        list1=list1->next;
        listfusion=listfusion->next;       
    }
    while(list2!=NULL){
        listfusion->data=list2->data;
        list2=list2->next;
        listfusion=listfusion->next;
    }
    return listfusion;
}

CellProtected* fusion_arbre(CellTree* tree){
    CellTree* high = highest_child(tree);
    CellProtected* liste_vote;
    if(!high){
        return NULL;
    }
    while(high->firstChild!=NULL){
        liste_vote=fusion_liste_protected(liste_vote,high->firstChild->block->votes);
        high=high->firstChild;
    }
    return liste_vote;
}