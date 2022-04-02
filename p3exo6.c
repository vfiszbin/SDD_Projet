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

/*Alloue une cellule de la table de hachage, et qui initialise ses champs en mettant la valeur a zero*/
HashCell* create_HashCell(Key* key){
	HashCell*c=(HashCell*)malloc(sizeof(HashCell));
	if (!c)
		return NULL;
	c->key=key;
	c->val=0;
	return c;
}

/*Libere la memoire allouee pour la HashCell c*/
void delete_HashCell(HashCell* c){
	free(c);
}

/*Fonction de hachage qui retourne la position d'un element dans la table de hachage selon les valeurs de sa cle*/
int hash_function(Key* key, int size){
	return (key->val + key->n) % size;
}

/*Cherche dans la table t s’il existe un element dont la cle publique est key, retourne sa position dans la table si l'element
est trouve, la position a laquelle il devrait se trouver sinon*/
int find_position(HashTable* t, Key* key){
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

/*Cree et initialise une table de hachage de taille size contenant une cellule pour chaque cle de la liste chainee keys*/
HashTable* create_HashTable(CellKey* keys, int size){
	HashTable *t = (HashTable*) malloc(sizeof(HashTable));
	t->size = size;
	t->tab = (HashCell**) malloc(sizeof(HashCell*) * size);
	int i = 0;
	while (i < size){
		t->tab[i] = NULL;
		i++;
	}
	i = 0;
	HashCell *c;
	int pos;
	while (keys){
		c = create_HashCell(keys->data);
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

/*Supprime une table de hachage.*/
void delete_hashtable(HashTable* t){
	for (int i = 0; i < t->size; i++){
		if (t->tab[i] !=  NULL)
			delete_HashCell(t->tab[i]);
	}
	free(t->tab);
	free(t);
}

int equal_keys(Key * k1, Key *k2){
	return (k1->val == k2->val && k1->n == k2->n);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
	//Cree la table de hachage des candidats
	HashTable *hc = create_HashTable(candidates, sizeC);
	if (!hc)
		return NULL;
	//Cree la table de hachage des votants
	HashTable *hv = create_HashTable(voters, sizeV);
	if (!hv){
		delete_hashtable(hv);
		return NULL;
	}

	/*Parcourt de la liste de declarations. 
	On fait l'hypothese que les declarations invalides ont deja ete retires de la liste*/
	int pos_v, pos_c;
	Key *pKey_c;
	while (decl){
		//Verifie si la personne a le droit de voter
		pos_v = find_position(hv, decl->data->pKey);
		if (equal_keys(hv->tab[pos_v]->key, decl->data->pKey) && hv->tab[pos_v]->val == 0){ //la cle publique est bien dans la table la personne n'a pas vote
			pKey_c = str_to_key(decl->data->mess);
			if (!pKey_c){
				delete_hashtable(hc);
				delete_hashtable(hv);
				return NULL;
			}
			pos_c = find_position(hc, pKey_c);
			if (equal_keys(hc->tab[pos_c]->key, pKey_c)){ //la pkey choisie est bien celle d'un candidat
				hc->tab[pos_c]->val++; //vote comptabilisé pour le candidat
				hv->tab[pos_v]->val++; //le citoyen a vote
			}
			free(pKey_c);
		}
		decl = decl->next;
	}

	//Toutes les declarations ont etes depouillees
	//Cherche le gagnant de l'election en parcourant hc
	int max_votes = 0;
	pos_c = 0;
	for (int i = 0; i < hc->size; i++){
		if (hc->tab[i] != NULL && hc->tab[i]->val > max_votes){
			max_votes = hc->tab[i]->val;
			pos_c = i;
		}
	}
	pKey_c = hc->tab[pos_c]->key; //cle du gagnant
	delete_hashtable(hc);
	delete_hashtable(hv);
	return pKey_c;

}