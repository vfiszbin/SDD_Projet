#include "p1exo1.h"
#include "p1exo2.h"
#include "p2exo3.h"

void print_long_vector(long *result, int size){
	printf("Vector: [");
	for (int i=0; i<size; i++){
		printf("%lx \t", result[i]);
	}
	printf("]\n");
}

int main (void) {
    srand(time(NULL));

    //Testing Init Keys
    Key* pKey= malloc (sizeof (Key));
    if (!pKey){
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    Key* sKey= malloc (sizeof (Key));
    if (!sKey){
        free(pKey);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    init_pair_keys(pKey,sKey,3,7);
    printf("pKey: %lx , %lx \n",pKey->val, pKey->n);
    printf ("sKey : %lx , %lx \n",sKey->val, sKey->n);


    //Testing Key Serialization
    char* chaine = key_to_str (pKey) ;
    if (!chaine){
        free(pKey);
        free(sKey);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    printf("key_to_str:%s\n",chaine);
    Key* k = str_to_key(chaine);
    if (!k){
        free(pKey);
        free(sKey);
        free(chaine);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    printf("str_to_key : %lx , %lx\n",k->val,k->n);


    //Testing signature
    //Candidate keys:
    Key* pKeyC = malloc(sizeof(Key));
    if (!pKeyC){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    Key* sKeyC = malloc(sizeof(Key)); init_pair_keys(pKeyC, sKeyC, 3, 7);
    if (!sKey){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    //Declaration:
    char* mess = key_to_str(pKeyC);
    if (!mess){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
        free(sKeyC);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    printf("%s vote pour %s\n",key_to_str(pKey), mess); 
    Signature* sgn = sign(mess, sKey);
    if (!sgn){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
        free(sKeyC);
        free(mess);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    printf("signature : "); print_long_vector(sgn->content, sgn->size);
    chaine = signature_to_str(sgn);
    if (!chaine){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
        free(sKeyC);
        free(mess);
        free(sgn);
		printf("Erreur allocation mémoire\n");
		return 1;
	}

    printf(" signature to str : %s \n", chaine);
    sgn = str_to_signature(chaine);
    if (!sgn){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
        free(sKeyC);
        free(mess);
        free(sgn);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    printf("str to signature : "); print_long_vector(sgn->content,sgn->size);

    //Testing protected:
    Protected* pr = init_protected(pKey, mess, sgn);
    if (!pr){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
        free(sKeyC);
        free(mess);
        free(sgn);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    //Verification:
    if (verify(pr)){ 
        printf("Signature valide\n");
    }else{
        printf("Signature non valide\n");
    }
    chaine = protected_to_str(pr);
    if (!chaine){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
        free(sKeyC);
        free(mess);
        free(sgn);
        free(pr);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    printf(" protected to str : %s\n", chaine);
    free(pr);
    pr = str_to_protected(chaine);
    if (!pr){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
        free(sKeyC);
        free(mess);
        free(sgn);
        free(pr);
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    printf("str to protected : %s %s %s\n", key_to_str(pr->pKey), pr->mess, signature_to_str(pr->sgn));


    //Testing generate_random_data
    if (! generate_random_data(50,10)){
        free(pKey);
        free(sKey);
        free(chaine);
        free(k);
        free(pKeyC);
        free(sKeyC);
        free(mess);
        free(sgn);
        free(pr->pKey);
        free(pr->mess);
        free(pr->sgn);
        free(pr);
		printf("Erreur allocation mémoire\n");
		return 1;
	}

    free(pKey);
    free(sKey);
    free(chaine);
    free(k);
    free(pKeyC);
    free(sKeyC);
    free(mess);
    free(sgn);
    free(pr->pKey);
    free(pr->mess);
    free(pr->sgn);
    free(pr);

    return 0;
}