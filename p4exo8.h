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
CellTree* highest_child(CellTree* cell);
#endif