#ifndef P1EXO2_H

#define P1EXO2_H

#include <stdio.h>
#include <string.h>

void generate_key_values(long p, long q, long *n, long *s, long*u);
long *encrypt(char *chaine, long s, long n);
char *decrypt (long *crypted, int size, long u, long n);



#endif
