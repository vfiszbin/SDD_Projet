
#ifndef P3EXO5_H

#include "p2exo3.h"

typedef struct cellKey{ 
	Key* data;
	struct cellKey* next; 
} CellKey;

CellKey* read_public_keys(char *filename);
void delete_list_keys(CellKey *lck);
void print_list_keys(CellKey *lck);

#endif