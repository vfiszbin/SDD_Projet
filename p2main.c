#include "p1exo1.h"
#include "p1exo2.h"
#include "p2exo3.h"


int main (void) {
    srand(time (NULL)) ;
    //Testing Init Keys
    Key* pKey= malloc (sizeof (Key));
    Key* sKey= malloc (sizeof (Key));
    init_pair_keys(pKey,sKey,3,7);
    printf("pKey: %lx , %lx \n",pKey->val, pKey->n);
    printf ("sKey : %lx , %lx \n",sKey->val, sKey->n);
    //Testing Key Serialization
    char* chaine = key_to_str (pKey) ;
    printf("key_to_str:%s\n",chaine);
    Key* k = str_to_key(chaine);
    printf("str_to_key : %lx , %lx\n",k->val,k->n);
    return 0;
}
