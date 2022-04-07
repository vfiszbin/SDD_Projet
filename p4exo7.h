#ifndef P4EXO7_H
#define P4EXO7_H
#include "p3exo6.h"


typedef struct block{
    Key* author;
    int nb_votes;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;


#endif