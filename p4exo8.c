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

//la racine
void print_tree(CellTree* arbre){
    CellTree* copie;
    copie=arbre;
    if(!arbre){
        printf("le noeud est vide");
    }else
    {
        if(arbre->father==NULL){
            printf("%d,%s",copie->height,copie->block->hash);
            print_tree(copie->firstChild);
        }else{
            printf("%d,%s",copie->height,copie->block->hash);
            print_tree(copie->nextBro);
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
    CellTree* tmp;
    int hauteur_max=0;
    int cpt=0;
    if(cell){
        while(cell->nextBro!=NULL){
            while(cell->firstChild!=NULL){
                tmp=cell->firstChild;
                cell=tmp;
                cpt++;
            }
            if(hauteur_max>cpt){
            hauteur_max=cpt;
            cell=high;
            }
            cpt=0;
        }
        //on retourne le premier fils?
        return high;
    }else{
        return NULL;
    }
}