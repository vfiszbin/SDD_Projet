#include "p1exo1.h"
#include "p1exo2.h"
#include "p2exo3.h"

//permet d'initialise une cle deja allouer
void init_key(Key* key, long val, long n ){
    if (key){
        key->val=val;
        key->n=n;
    }else{
        printf ("key n'est pas allouer");
        exit(1);
    }
}

void init_pair_keys(Key* pkey,Key* skey, long low_size, long up_size){
    long p = random_prime_number(low_size,up_size, 5000); 
	long q = random_prime_number(low_size,up_size, 5000); 
	while(p==q){
		q = random_prime_number(3,7, 5000); 
	}
	long n, s, u; 
	generate_key_values(p,q,&n,&s,&u); 
    init_key(pkey,s,n);
    init_key(skey,u,n);

}

char* key_to_str (Key* key){
    long a,b;
    char* u = (char*)malloc (20*sizeof(char));
    if(u==NULL){
        return NULL;
    }
    if (key){
        a=key->val;
        b=key->n;
    }
    int carac = sprintf(u,"(%lx,%lx)",a,b);
    u[carac]='\0';
    return u;
}

Key* str_to_key(char* str){
    long val,n;
    sscanf(str,"(%lx,%lx)",&val,&n);
    Key* cle = malloc (sizeof(Key));
    cle->val=val;
    cle->n=n;
    return cle;
}

Signature* init_signature(long* content, int size){
    Signature* s = malloc (sizeof(Signature));
    if(s==NULL){
        return NULL;
    }
    s->tab=content;
    s->taille=size;
    return s;
}

Signature* sign(char* mes, Key* sKey){
    long* tab=encrypt(mes,sKey->val,sKey->n);
    Signature* s=init_signature(tab,strlen(mes));
    return mes;
}





