#include "p3exo5.h"

#define BUFFLEN 1024

/*Alloue et initialise une cellule de liste chainee de type CellKey*/
CellKey* create_cell_key(Key* key){
	CellKey *c = (CellKey *) malloc(sizeof(CellKey));
	if (!c)
		return NULL;
	c->data = key;
	c->next = NULL;
	return c;
}

/*Ajoute une cle en tete de liste*/
void add_cellKey_to_front(CellKey **lck, CellKey *c){
	c->next = *lck;
	*lck = c;
}

/*Libere une cellule de liste chainee de cles*/
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
	if (!f){
		printf("Erreur d'ouverture de %s\n", filename);
		return NULL;
	}
		

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
		add_cellKey_to_front(&lck, c);
	}
	if (feof(f) == 0){ //si fgets s'est arretee de lire avant la fin de fichier
		delete_list_keys(lck);
		fclose(f);
		return NULL;
	}
	fclose(f);
	return lck;

}

/*Alloue et initialise une cellule de liste chainee de type CellProtected*/
CellProtected* create_cell_protected (Protected* pr){
	CellProtected* c = (CellProtected*) malloc (sizeof(CellProtected));
	if (c==NULL){
		return NULL;
	}
	c->data=pr;
	c->next=NULL;
	return c;
}

/*Ajoute une déclaration signee en tete de liste*/
void add_cellProtected_to_front(CellProtected** lcp, CellProtected* c){
	c->next = *lcp;
	*lcp = c;

}

/*Libere une cellule de liste chainee de declarations signees*/
void delete_cell_protected(CellProtected* c){
	free(c->data->mess);
	free(c->data->pKey);
	free(c->data->sgn->content);
	free(c->data->sgn);
	free(c->data);
	free(c);
}

/*Libere une liste chainee de declarations signees*/
void delete_list_cell(CellProtected* lcp){
	CellProtected* tmp;
	while (lcp){
		tmp = lcp;
		lcp = lcp->next;
		delete_cell_protected(tmp);
	}
	
}

/*Prend en entree le fichier declarations.txt, et retourne une liste chainee 
contenant toutes les declarations signees du fichier*/
CellProtected* read_protected(char *filename){
	FILE *f = fopen(filename, "r");
	if (f==NULL){
		printf("Erreur d'ouverture de %s\n", filename);
		return NULL;
	}
	Protected* pr;
	CellProtected* c;
	CellProtected* lcp = NULL;
	char buffer[BUFFLEN];


	while(fgets(buffer, BUFFLEN, f)){
		pr = str_to_protected(buffer);
		if (!pr){
			delete_list_cell(lcp);
			fclose(f);
			return NULL;
		}
		c = create_cell_protected(pr);
		if (!c){
			free(pr);
			delete_list_cell(lcp);
			fclose(f);
			return NULL;
		}
		add_cellProtected_to_front(&lcp, c);
	}
	if (feof(f) == 0){ //si fgets s'est arretee de lire avant la fin de fichier
		delete_list_cell(lcp);
		fclose(f);
		return NULL;
	}
	fclose(f);
	return lcp;
}

void print_list_cell_protected(CellProtected* lcp){
	char *str_pr;
	while(lcp){
		str_pr = protected_to_str(lcp->data);
		if (!str_pr)
			return;
		printf("%s\n", str_pr);
		lcp = lcp->next;
		free(str_pr);
	}
}



