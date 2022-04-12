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
        return 0;
    }
    if(!child){
        return 0;
    }
    father->height=child->height+1;
    father->firstChild=child;
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
int hauteur_maximun (CellTree* tree){
    int cpt=0;
    if(!tree){
        return -1;
    }else{
        while(tree->father!=NULL){
            cpt=cpt+1;
        }
        return cpt;
    }
}


void print_tree(CellTree* arbre){
    CellTree* copie;
    CellTree* copie1;
    copie=arbre;
    if(!arbre){
        printf("l'arbre est vide");
    }else
    {
        while(copie->height!=hauteur_maximun(arbre)){
            copie1=copie->father;
            copie=copie1;
        }
        if(copie->height==hauteur_maximun(arbre)){
            printf("%d,%s",copie->height,copie->block->hash);
            print_tree(copie->firstChild);
        }else{
            printf("%d,%s",copie->height,copie->block->hash);
            print_tree(copie->nextBro);
            print_tree(copie->firstChild);

        }
    }
    
}