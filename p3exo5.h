
#ifndef P3EXO5_H

#include "p2exo3.h"

typedef struct cellKey{ 
	Key* data;
	struct cellKey* next; 
} CellKey;

CellKey* read_public_keys(char *filename);
void delete_list_keys(CellKey *lck);
void print_list_keys(CellKey *lck);

typedef struct cellProtected{
	Protected* data;
	struct cellProtected* next;
} CellProtected;

CellProtected* create_cell_protected (Protected* pr);
void ajouter_en_tete(CellProtected** lcp,CellProtected* c);
void delete_cell_protected(CellProtected* pr);
void delete_list_cell(CellProtected* lcp);
CellProtected* read_protect(char *filename);
void affichage_list_cell_protected(CellProtected* pr);

void imposteur(CellProtected* lcp);

#endif