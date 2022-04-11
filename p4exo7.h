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


void ecrire_fichier(char* nom, Block* b);
Block * create_Block(Key *author, CellProtected *votes, unsigned char *hash, unsigned char *previous_hash, int nonce);
Block* lire_fichier(char* nom);
char* block_to_str(Block* block);
unsigned char* crypteensha256 (char* message);
void delete_block(Block *b);


#endif