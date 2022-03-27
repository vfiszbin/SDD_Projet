#include "p3exo5.h"

#define BUFFLEN 1024

/*Alloue et initialise une cellule de liste chainee*/
CellKey* create_cell_key(Key* key){
	CellKey *c = (CellKey *) malloc(sizeof(CellKey));
	if (!c)
		return NULL;
	c->data = key;
	c->next = NULL;
	return c;
}

/*Ajoute une cle en tete de liste*/
void add_cell_key_head_of_list(CellKey **lck, CellKey *c){
	c->next = *lck;
	*lck = c;
}

/*Supprime une cellule de liste chainee de cles*/
void delete_cell_key(CellKey *c){
	free(c->data);
	free(c);
}

/*Libere une liste chainee de cles*/
void delete_list_keys(CellKey *lck){
	CellKey *tmp;

	while (lck){
		tmp = lck;
		lck = lck->next;
		delete_cell_key(tmp);
	}
}

/*Affiche une liste chainee de cles*/
void print_list_keys(CellKey *lck){
	while (lck){
		printf("(%lx,%lx)\n", lck->data->val, lck->data->n);
		lck = lck->next;
	}
}

/*Prend en entree le fichier keys.txt ou le fichier candidates.txt,
et retourne une liste chainee contenant toutes les cles publiques du fichier*/
CellKey* read_public_keys(char *filename){
	FILE *f = fopen(filename, "r");
	if (!f)
		return NULL;

	char buffer[BUFFLEN];
	char key_str[BUFFLEN];
	Key *pKey;
	CellKey *c;
	CellKey *lck = NULL;
	int i,j;

	while(fgets(buffer, BUFFLEN, f)){
		i = 6; //indice a partir duquel s'ouvre la parenthese de la pKey
		j = 0;
		while (buffer[i] != ')'){ //on arrete de recopier quand on a atteint la prenthese fermante
			key_str[j] = buffer[i]; //copie la pkey dans key_str
			i++;
			j++;
		}
		key_str[j] = '\0';

		pKey = str_to_key(key_str);
		if (!pKey){
			delete_list_keys(lck);
			fclose(f);
			return NULL;
		}
		c = create_cell_key(pKey);
		if (!c){
			free(pKey);
			delete_list_keys(lck);
			fclose(f);
			return NULL;
		}
		add_cell_key_head_of_list(&lck, c);
	}
	if (feof(f) == 0){ //si fgets s'est arretee de lire avant la fin de fichier
		delete_list_keys(lck);
		fclose(f);
		return NULL;
	}
	
	return lck;

}