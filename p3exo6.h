
#ifndef P3EXO6_H
#define P3EXO6_H
#include "p3exo5.h"

void supprime_declarations_non_valides(CellProtected **lcp);

typedef struct hashcell{
    Key* key;
    int val;
}HashCell;

typedef struct hashtable{
    HashCell** tab;
    int size;
}HashTable;


HashTable* create_HashTable(CellKey* keys, int size);
int find_position(HashTable* t, Key* key);
void delete_hashtable(HashTable* t);

#endif