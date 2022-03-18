#ifndef P2EXO3_H

#define P2EXO3_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct key{
    long val;
    long n;
} Key;

typedef struct signature{
    long* content;
    int size;
} Signature;


void init_key(Key* key, long val, long n );
void init_pair_keys(Key* pkey,Key* skey, long low_size, long up_size);
char* key_to_str (Key* key);
Key* str_to_key(char* str);

#endif
