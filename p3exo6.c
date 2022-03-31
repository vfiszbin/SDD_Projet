#include "p3exo6.h"

/*Supprime toutes les declarations signees dont la signature n'est pas valide dans la liste chainee lcp*/
void supprime_declarations_non_valides(CellProtected **lcp){
	CellProtected *c = *lcp;
	CellProtected *tmp;
	CellProtected *prec = NULL;

	while(c){
		if (verify(c->data) == 0){ //la signature est invalide			
			//Suppression de l'element de la liste chainee sans rompre la liste
			//L'element est en tete de liste
			if (prec == NULL){
				tmp = c;
				c = c->next;
				prec = NULL;
				*lcp = c; //nouvelle tete de liste
				delete_cell_protected(tmp);
			}
			//L'element est dans la liste
			else{
				tmp = c;
				prec->next = c->next;
				c = c->next;
				delete_cell_protected(tmp);
			}
		}
		else{ //la signature est valide
			prec = c;
			c = c->next;
		}
	}
}

Hashcell* create_hashcell(Key* key){
	Hashcell*c=(Hashcell*)malloc(sizeof(Hashcell));
	if (!c)
		return NULL;
	c->key=key;
	c->val=0;
	return c;
}

//int hash_function(Key* key,int size){
//
//}

int find_position(Hashtable* t,Key* key){
	int i;
	if(t!=NULL){
		if(t->tab!=NULL){
			for(i=0;i<t->size;i++){
				if()
			}
		}
	}
	return 0;
}