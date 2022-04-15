#ifndef P4EXO8_H
#define P4EXO8_H
#include "p4exo7.h"

typedef struct block_tree_cell{
    Block* block;
    struct block_tree_cell* father;
    struct block_tree_cell* firstChild;
    struct block_tree_cell* nextBro;
    int height;
}CellTree;

CellTree* create_node(Block* b);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_tree(CellTree* arbre);
void delete_node(CellTree* node);
void delete_tree(CellTree* tree);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
CellProtected* fusion_liste_protected(CellProtected* list1,CellProtected* list2);
CellProtected* fusion_arbre(CellTree* tree);
void print_tree2D(CellTree *cell, int spaces);

#endif