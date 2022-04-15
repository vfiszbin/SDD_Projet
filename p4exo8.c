#include "p4exo8.h"


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

void add_child(CellTree* father, CellTree* child){
    if(!father || !child){
        return ;
    }

    //Insertion du child
    if (father->firstChild == NULL){ //si le pere n'a pas deja un fils
        father->firstChild = child;
    }
    else{ //si le pere a deja un fils (ou plus d'un fils)
        CellTree * brother = father->firstChild;

        while (brother->nextBro != NULL){ //on cherche le frere qui n'a pas encore de frere
            brother = brother->nextBro;
        }
        brother->nextBro = child; //insere le child comme le dernier frere parmi les fils du father
    }

    //Mise a jour des hauteur de tous les ascendants
    CellTree *ascendant = father;
    CellTree *descendant = child;
    while (ascendant != NULL){ //remonte tous les ascendants
        update_height(ascendant, descendant); //maj la hauteur
        descendant = ascendant;
        ascendant = ascendant->father;
    }
}

//la racine
void print_tree(CellTree* arbre){
    CellTree* copie;
    copie=arbre;
    if(!arbre){
        printf("le noeud est vide");
        printf("\n");
    }else
    {
        if(arbre->father==NULL){
            printf("%d,%s\n",copie->height,copie->block->hash);
            printf("voici mon fils\n");
            print_tree(copie->firstChild);
        }else{
            printf("%d,%s\n",copie->height,copie->block->hash);
            printf("voici mon frere\n");
            print_tree(copie->nextBro);
            printf("voici mon fils\n");
            print_tree(copie->firstChild);
        }
    }
    
}
//libere les noeuds de l'arbre
void delete_node(CellTree* node){
    if(node){
        delete_block(node->block);
        delete_node(node->father);
        delete_node(node->nextBro);
        delete_node(node->firstChild);
        free(node);
    }
}


CellTree* highest_child(CellTree* cell){
    CellTree* high;
    CellTree* tmp=cell->firstChild->nextBro;
    if(cell){
        if(cell->firstChild!=NULL){
            high=cell->firstChild;
            while(tmp){
                if(high->height<tmp->height){
                    high=tmp;
                }
            tmp=tmp->nextBro;
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