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

typedef struct protected{
    Key *pKey;
    char *mess;
    Signature *sgn;
} Protected;

void init_key(Key* key, long val, long n );
void init_pair_keys(Key* pkey,Key* skey, long low_size, long up_size);
char* key_to_str (Key* key);
Key* str_to_key(char* str);
Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);
Protected* init_protected(Key* pKey, char* mess, Signature* sgn);
int verify(Protected* pr);
char* protected_to_str (Protected* pr);
Protected* str_to_protected (char* str);

#endif
