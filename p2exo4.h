#ifndef P2EXO4_H
#define P2EXO4_H
#include "p2exo3.h"

typedef struct citoyen{
    Key *clepublic;
    Key *cleprive;
}Citoyen;

int generate_random_data(int nv, int nc);


#endif