
#ifndef P3EXO6_H
#define P3EXO6_H
#include "p3exo5.h"

void supprime_declarations_non_valides(CellProtected **lcp);

typedef struct hashcell{
    Key* key;
    int val;
}Hashcell;

typedef struct hashtable{
    Hashcell** tab;
    int size;
}Hashtable;

Hashcell* create_hashcell(Key* key);
//int hash_function(Key* key,int size);

#endif