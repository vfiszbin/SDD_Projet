#include "p1exo1.h"
#include "p1exo2.h"
#include "p2exo3.h"

/*Permet d'initialiser une cle key deja allouee*/
void init_key(Key* key, long val, long n ){
    if (key){
        key->val=val;
        key->n=n;
    }else{
        printf("key n'est pas allouée");
        return;
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
    //Pour avoir des cles positives : 
	if (u<0){
		long t = (p-1)*(q-1);
		u = u+t; //on aura toujours s*u mod t = 1 
	}

    init_key(pkey,s,n);
    init_key(skey,u,n);

}

/*Permet de passer d'une variable de type Key a sa representation sous forme de chaine de caracteres*/
char* key_to_str (Key* key){
    if (key == NULL)
        return NULL;
    long a,b;
    char* u = (char*)malloc (20*sizeof(char));
    if(u==NULL){
        return NULL;
    }
    a=key->val;
    b=key->n;
    int carac = sprintf(u,"(%lx,%lx)",a,b);
    u[carac]='\0';
    return u;
}

/*Alloue, initialise et retourne une cle de type Key a partir de sa representation sous forme de chaine de caracteres*/
Key* str_to_key(char* str){
    long val,n;
    sscanf(str,"(%lx,%lx)", &val, &n);
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

/*Cree une signature a partir du message mess (declaration de vote) et de la cle secrete sKey de l'emetteur*/
Signature* sign(char* mess, Key* sKey){
    long* content = encrypt(mess, sKey->val ,sKey->n);
    Signature* sgn = init_signature(content, strlen(mess));
    if (!sgn)
        return NULL;
    return sgn;
}


/*Permet de passer d'une signature sgn a sa representation sous forme de chaine de caracteres*/
char* signature_to_str(Signature* sgn){
    char* result = malloc(10*sgn->size*sizeof(char));
    if (!result)
        return NULL;
    result[0]= '#';
    int pos = 1;
    char buffer [156];
    for (int i=0; i<sgn->size; i++){ 
        sprintf(buffer, "%lx", sgn->content[i]); 
        for (size_t j=0; j < strlen(buffer); j++){
            result[pos] = buffer[j];
            pos = pos +1; 
        }
        result[pos] = '#';
        pos = pos +1; 
        }
    result[pos] = '\0';
    result = realloc(result, (pos+1)*sizeof(char)); 
    return result;
}

/*Alloue, initialise et retourne une signature a partir de sa representation sous forme de chaine de caracteres*/
Signature* str_to_signature(char* str){
    int len = strlen(str);
    long* content = (long*)malloc(sizeof(long)*len); 
    int num = 0;
    if (!content)
        return NULL;
    char buffer [256];
    int pos = 0;
    for (int i=0; i<len; i++){
        if (str[i] != '#'){ 
            buffer[pos] = str[i]; 
            pos=pos+1;
        }else{
            if (pos != 0){
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num])); 
                num = num + 1;
                pos = 0;
            } 
        }
    }
    content=realloc(content ,num*sizeof(long)); 
    if (!content)
        return NULL;
    return init_signature(content , num);
}

/*Libere une structure de type protected ainsi que tous ses champs*/
void delete_protected(Protected * pr){
    free(pr->pKey);
    free(pr->mess);
    free(pr->sgn);
    free(pr);
}

/*Alloue, initialise et retourne une structure Protected avec la clé publique pKey, le message mess et la signature sgn*/
Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
    Protected *pr = malloc(sizeof(Protected));
    if (!pr)
        return NULL;
    pr->pKey = pKey;
    pr->mess = mess;
    pr->sgn = sgn;
    return pr;
}

/*Verifie que la signature contenue dans pr correspond bien au message et a la personne contenus dans pr*/
int verify(Protected* pr){
    char *decrypted_mess = decrypt(pr->sgn->content, pr->sgn->size, pr->pKey->val, pr->pKey->n);
    if (!decrypted_mess)
        return -1;
    
    int cmp = strcmp(decrypted_mess, pr->mess); //compare le message decrypte et le message contenu dans pr
    free(decrypted_mess);
    return cmp == 0;
}

/*Permet de passer d'un Protected a sa representation sous forme de chaine de caracteres*/
char* protected_to_str (Protected* pr){
    char *pKey = key_to_str(pr->pKey);
    if (!pKey)
        return NULL;
    char *sgn = signature_to_str(pr->sgn);
    if (!sgn)
        return NULL;

    int size = strlen(pKey) + strlen(sgn) + strlen(pr->mess) + 3; //taille de la chaine de caracteres a retourner
    char *result = (char*) malloc(sizeof(char) * size);
    if (!result)
        return NULL;

    int i = 0;
    int j = 0;
    while(pKey[j]){ //copie la cle publique dans la chaine a retourner
        result[i] = pKey[j];
        i++;
        j++;
    }
    result[i] = ' '; //place un espace entre les chaines
    i++;
    j = 0;
    while(pr->mess[j]){ //copie le message dans la chaine a retourner
        result[i] = pr->mess[j];
        i++;
        j++;
    }
    result[i] = ' '; //place un espace entre les chaines
    i++;
    j = 0;
    while(sgn[j]){ //copie la signature dans la chaine a retourner
        result[i] = sgn[j];
        i++;
        j++;
    }
    result[i] = '\0';

    free(pKey);
    free(sgn);
    return result;
}

/*Alloue, initialise et retourne une signature a partir de sa representation sous forme de chaine de caracteres*/
Protected* str_to_protected (char* str){
    Protected *pr = (Protected*) malloc(sizeof(Protected));
    if (!pr)
        return NULL;

    char buffer_pKey [256];
    char buffer_mess [256];
    char buffer_sgn [256];
    sscanf(str, "%s %s %s", buffer_pKey, buffer_mess, buffer_sgn);

    pr->pKey = str_to_key(buffer_pKey);
    if (!pr->pKey){
        free(pr);
        return NULL;
    }
    pr->mess = strdup(buffer_mess);
    if (!pr->mess){
        free(pr->pKey);
        free(pr);
        return NULL;
    }
    pr->sgn = str_to_signature(buffer_sgn);
    if (!pr->sgn){
        free(pr->pKey);
        free(pr->mess);
        free(pr);
        return NULL;
    }

    return pr;
}









