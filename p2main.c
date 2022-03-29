#include "p1exo1.h"
#include "p1exo2.h"
#include "p2exo3.h"
#include "p2exo4.h"

void print_long_vector(long *result, int size){
	printf("Vector: [");
	for (int i=0; i<size; i++){
		printf("%lx \t", result[i]);
	}
	printf("]\n");
}

int free_p2main(Key *pKey, Key *sKey, char *chaine, Key *k, Key *pKeyC, Key *sKeyC, char *mess, long *sgn_content, Signature *sgn,
Key *pr_pKey, char *pr_mess, long *pr_sgn_content, Signature *pr_sgn, Protected *pr, char *key_str){
    if (pKey)
        free(pKey);
    if (sKey)
        free(sKey);
    if (chaine)
        free(chaine);
    if (k)
        free(k);
    if (pKeyC)
        free(pKeyC);
    if (sKeyC)
        free(sKeyC);
    if (mess)
        free(mess);
    if (sgn_content)
        free(sgn_content);
    if (sgn)
        free(sgn);
    if (pr_pKey)
        free(pr_pKey);
    if (pr_mess)
        free(pr_mess);
    if (pr_sgn_content)
        free(pr_sgn_content);
    if (pr_sgn)
        free(pr_sgn);
    if (pr)
        free(pr);
    if (key_str)
        free(key_str);
    return 1;
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
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    init_pair_keys(pKey,sKey,3,7);
    printf("pKey: %lx , %lx \n",pKey->val, pKey->n);
    printf ("sKey : %lx , %lx \n",sKey->val, sKey->n);


    //Testing Key Serialization
    char* chaine = key_to_str (pKey) ;
    if (!chaine){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    printf("key_to_str:%s\n",chaine);
    Key* k = str_to_key(chaine);
    if (!k){
        free(pKey);
        free(sKey);
        free(chaine);
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    printf("str_to_key : %lx , %lx\n",k->val,k->n);


    //Testing signature
    //Candidate keys:
    Key* pKeyC = malloc(sizeof(Key));
    if (!pKeyC){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    Key* sKeyC = malloc(sizeof(Key)); init_pair_keys(pKeyC, sKeyC, 3, 7);
    if (!sKey){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}

    //Declaration:
    char* mess = key_to_str(pKeyC);
    if (!mess){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}

    char *key_str = key_to_str(pKey);
    if (!key_str){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    printf("%s vote pour %s\n", key_str, mess);
    free(key_str);

    Signature* sgn = sign(mess, sKey);
    if (!sgn){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    printf("signature : "); print_long_vector(sgn->content, sgn->size);
    free(chaine);
    chaine = signature_to_str(sgn);
    if (!chaine){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, NULL, k, pKeyC, sKeyC, mess, sgn->content, sgn, NULL, NULL, NULL, NULL, NULL, NULL);
	}

    printf(" signature to str : %s \n", chaine);
    free(sgn->content);
    free(sgn);
    sgn = str_to_signature(chaine);
    if (!sgn){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    printf("str to signature : "); print_long_vector(sgn->content,sgn->size);

    //Testing protected:
    Protected* pr = init_protected(pKey, mess, sgn);
    if (!pr){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, sgn->content, sgn, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    //Verification:
    if (verify(pr)){ 
        printf("Signature valide\n");
    }else{
        printf("Signature non valide\n");
    }
    
    free(chaine);
    chaine = protected_to_str(pr);
    if (!chaine){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, NULL, k, pKeyC, sKeyC, mess, sgn->content, sgn, NULL, NULL, NULL, NULL, pr, NULL);
	}
    printf(" protected to str : %s\n", chaine);
    free(pr);
    pr = str_to_protected(chaine);
    if (!pr){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, sgn->content, sgn, NULL, NULL, NULL, NULL, NULL, NULL);
	}
    key_str = key_to_str(pr->pKey);
    if (!key_str){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, sgn->content, sgn, pr->pKey, pr->mess, pr->sgn->content, pr->sgn, pr, NULL);
	}
    char *sgn_str = signature_to_str(pr->sgn);
    if (!sgn_str){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, sgn->content, sgn, pr->pKey, pr->mess, pr->sgn->content, pr->sgn, pr, key_str);
	}
    printf("str to protected : %s %s %s\n", key_str, pr->mess, sgn_str);
    free(key_str);
    free(sgn_str);

    //Testing generate_random_data
    if (! generate_random_data(10,3)){
		printf("Erreur allocation mémoire\n");
		return free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, sgn->content, sgn, pr->pKey, pr->mess, pr->sgn->content, pr->sgn, pr, NULL);
	}

    free_p2main(pKey, sKey, chaine, k, pKeyC, sKeyC, mess, sgn->content, sgn, pr->pKey, pr->mess, pr->sgn->content, pr->sgn, pr, NULL);

    return 0;
}

