
#ifndef P3EXO5_H
#define P3EXO5_H
#include "p2exo3.h"

typedef struct cellKey{ 
	Key* data;
	struct cellKey* next; 
} CellKey;

typedef struct cellProtected{
	Protected* data;
	struct cellProtected* next;
} CellProtected;

CellKey* read_public_keys(char *filename);
void delete_list_keys(CellKey *lck);
void print_list_keys(CellKey *lck);
CellProtected* create_cell_protected (Protected* pr);
void delete_list_cell(CellProtected* lcp);
CellProtected* read_protected(char *filename);
void print_list_cell_protected(CellProtected* pr);
void delete_cell_protected(CellProtected* c);

#endif