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

int hash_function(Key* key, int size){
	return (key->val + val->n) % size;
}

int find_position(Hashtable* t, Key* key){
	int pos = hash_function(key, t->size);
	if (t->tab[pos]->key->val == key->val && t->tab[pos]->key->n == key->n ){//si l'element est a la bonne position dans la table
		printf("Element trouvé à la bonne position\n");
		return pos;
	}
	int i = pos + 1;
	if (i >= t->size) //si i depasse la taille du tableau
		i = 0;
	while (t->tab[i]->key->val != key->val && t->tab[i]->key->n != key->n && i != pos){
		i++;
		if (i >= t->size) //si i depasse la taille du tableau
			i = 0; 
	}
	return i;
}

HashTable* create_hashtable(CellKey* keys, int size){
	HashTable *t = (HashTable*) malloc(sizeof(HashTable));
	t->tab = (Hashcell**) malloc(sizeof(Hashcell*) * size);
	int i = 0;
	while (i < size){
		t-tab[i] = NULL;
		i++;
	}
	i = 0;
	Hashcell *c;
	int pos;
	while (keys){
		c = create_hashcell(keys->data);
		pos = hash_function(c->key, size);
		while (t->tab[pos] != NULL){ //probing pour trouver la premiere case libre dans la table
			pos++;
			if (i >= size) //si pos depasse la taille du tableau
				i = 0;
		}
		t->tab[pos] = c;

		keys = keys->next;
	}
	return t;
}