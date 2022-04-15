#ifndef P4EXO7_H
#define P4EXO7_H
#include "p3exo6.h"
#include <string.h>
#include <openssl/sha.h>


typedef struct block{
    Key* author;
    int nb_votes;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;


int ecrire_block(char* nom, Block* b);
Block * create_Block(Key *author, CellProtected *votes, unsigned char *hash, unsigned char *previous_hash, int nonce, int nb_votes);
Block* lire_block(char* nom);
char* block_to_str(Block* block);
unsigned char* crypteensha256 (char* message);
void delete_block(Block *b);
void full_delete_block(Block *b);


#endif