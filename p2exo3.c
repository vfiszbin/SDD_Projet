#include "p1exo1.h"
#include "p1exo2.h"
#include "p2exo3.h"

/*Permet d'initialiser une cle key deja allouee*/
void init_key(Key* key, long val, long n ){
    if (key){
        key->val=val;
        key->n=n;
    }else{
        printf ("key n'est pas allouée");
        exit(1);
    }
}

/*Utilise le protocole RSA pour initialiser une cle publique pkey et une cle secrete skey (deja allouees)*/
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

/*Permet de passer d’une variable de type Key a sa representation sous forme de chaine de caracteres*/
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

/*Alloue, initialise et retourne une clé de type Key a partir de sa representation sous forme de chaine de caracteres*/
Key* str_to_key(char* str){
    long val,n;
    sscanf(str,"(%lx,%lx)",&val,&n);
    Key* cle = malloc (sizeof(Key));
    if (cle == NULL)
        return NULL;
    cle->val=val;
    cle->n=n;
    return cle;
}

/*Alloue et remplit une signature avec un tableau de long content de taille size, deja alloue et initialise*/
Signature* init_signature(long* content, int size){
    Signature* s = malloc (sizeof(Signature));
    if(s==NULL){
        return NULL;
    }
    s->content=content;
    s->size=size;
    return s;
}

/*Cree une signature a partir du message mess (declaration de vote) et de la cle secrete sKey de l’emetteur*/
Signature* sign(char* mess, Key* sKey){
    long* content = encrypt(mess, sKey->val ,sKey->n);
    Signature* sgn = init_signature(content, strlen(mess));
    return sgn;
}





