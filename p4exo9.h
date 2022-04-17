#ifndef P4EXO9_H
#define P4EXO9_H
#include "p4exo8.h"
#include <dirent.h>


void submit_vote(Protected* pr);
void create_block(CellTree** tree, Key* author, int d);
void add_block(int d, char* name);
CellTree* read_tree();
Key* compute_winner_BT(CellTree* tree,CellKey* candidates,CellKey* voters,int sizeC,int sizeV);

#endif