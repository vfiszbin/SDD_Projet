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

int update_height(CellTree* father, CellTree* child){
    if(!father){
        return -1;
    }
    if(!child){
        return -1;
    }
    if(father->height==(child->height)+1){
        return 0;
    }else{
        father->height=(child->height)+1;
        return 1;
    }
}

void add_child(CellTree* father, CellTree* child){
    if(!father){
        return;
    }
    if(!child){
        return;
    }
    father->height=child->height+1;
    father->firstChild=child;
    child->father=father;
    CellTree* Grandpa;
    CellTree* fatherbis;
    fatherbis=father;
    Grandpa = father->father;
    while(Grandpa!=NULL){
        Grandpa->height=(fatherbis->height)+1;
        fatherbis=Grandpa;
        Grandpa=Grandpa->father;
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