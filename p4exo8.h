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


#endif