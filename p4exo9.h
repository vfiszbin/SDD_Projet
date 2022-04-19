#ifndef P4EXO9_H
#define P4EXO9_H
#include "p4exo8.h"
#include <dirent.h>


void submit_vote(Protected* pr);
int create_block(CellTree** tree, Key* author, int d);
int add_block(int d, char* name);
CellTree* read_tree();
Key* compute_winner_BT(CellTree* tree,CellKey* candidates,CellKey* voters,int sizeC, int sizeV, int *nb_votes_vainqueur, int *nb_votes);

#endif