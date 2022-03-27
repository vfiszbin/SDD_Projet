#include "p2exo3.h"
#include "p2exo4.h"

/*Libere la memoire et ferme les flux vers les fichiers*/
void free_generate_random_data(int nv, Citoyen *tab_citoyens, Citoyen *tab_candidats, FILE *f, FILE *f1, FILE *f2){
    int i = 0;
    for(i = 0; i < nv ; i++){ //libere les cles de tous les elements precedents dans le tab
        free(tab_citoyens[i].clepublic);
        free(tab_citoyens[i].cleprive);
    }
    free(tab_citoyens);
    fclose(f);
    if (tab_candidats != NULL)
        free(tab_candidats); //pas besoin de liberer les cles des citoyens, ce sont les memes pointeurs que ceux de tab_citoyens
    if (f1 != NULL)
        fclose(f1);
    if (f2 != NULL)
        fclose(f2);
}

/*Genere nv citoyens chacun avec une cle publique/prive aleatoire, ecrit ces cles dans keys.txt, choisit nc candidats parmi ces citoyens,
ecrit les cles publiques des candidats dans candidates.txt, genere une declaration de vote pour un candidat aleatoire pour chaque
citoyen, ecrit les declarations dans declarations.txt*/
int generate_random_data(int nv, int nc){
    /*Genere nv citoyens avec une cle publique et secrete aleatoires, ecrit ces cles dans le fichier keys.txt*/
    FILE *f;
    FILE *f1;
    FILE *f2;

    int i=0;

    Citoyen* tab_citoyens = (Citoyen*) malloc (sizeof(Citoyen) * nv);
	if (!tab_citoyens)
		return 0;

    f = fopen("keys.txt", "w");
    if (f == NULL){
        free(tab_citoyens);
        printf("Erreur d'ouverture du fichier keys.txt");
        return 0;
    }
    int cle_deja_presente = 1;
    while(i < nv){
		Key* pKey= malloc (sizeof (Key));
		if (!pKey){
			for(int j= 0; j < i; j++){ //libere les cles de tous les elements precedents dans le tab
				free(tab_citoyens[j].clepublic);
				free(tab_citoyens[j].cleprive);
			}
			free(tab_citoyens);
			fclose(f);
			printf("Erreur allocation mémoire\n");
			return 0;
		}

		Key* sKey= malloc (sizeof (Key));
		if (!sKey){
			free(pKey);
			for(int j= 0; j < i; j++){ //libere les cles de tous les elements precedents dans le tab
				free(tab_citoyens[j].clepublic);
				free(tab_citoyens[j].cleprive);
			}
			free(tab_citoyens);
			fclose(f);
			printf("Erreur allocation mémoire\n");
			return 0;
		}
        //Empêche que deux citoyens (leurs cles) soient identiques
        while (cle_deja_presente != 0){
			cle_deja_presente = 0;
		    init_pair_keys(pKey,sKey,3,7);
			for (int j = 0; j < i; j++){
				if (tab_citoyens[j].clepublic->val == pKey->val && tab_citoyens[j].clepublic->n == pKey->n
				&& tab_citoyens[j].cleprive->val == sKey->val && tab_citoyens[j].cleprive->n == sKey->n){
					cle_deja_presente = 1;
					j = i;
				}
			}
		}
        cle_deja_presente = 1;

		fprintf(f, "pKey: (%lx,%lx), sKey: (%lx,%lx)\n", pKey->val, pKey->n, sKey->val, sKey->n);
		tab_citoyens[i].clepublic = pKey;
		tab_citoyens[i].cleprive = sKey;
		i++;
    }
	
    /*Selectionne aleatoirement nc candidats parmi les electeurs*/
	Citoyen* tab_candidats = (Citoyen*) malloc(sizeof(Citoyen) * nc);
	if (!tab_candidats){
        free_generate_random_data(nv, tab_citoyens, NULL, f, NULL, NULL);
		return 0;
	}

    int random;
	Citoyen candidat_potentiel;
	int candidat_deja_present = 1;
	for(i = 0; i < nc ; i++){
		while (candidat_deja_present != 0){
			candidat_deja_present = 0;
			random = rand() % nv;
			candidat_potentiel = tab_citoyens[random];

			for (int j = 0; j < i; j++){
				if (tab_candidats[j].clepublic->val == candidat_potentiel.clepublic->val && tab_candidats[j].clepublic->n == candidat_potentiel.clepublic->n
				&& tab_candidats[j].cleprive->val == candidat_potentiel.cleprive->val && tab_candidats[j].cleprive->n == candidat_potentiel.cleprive->n){
					candidat_deja_present = 1;
					j = i;
				}
			}
		}
        candidat_deja_present = 1;
		//Remalloc des cles separees ?
		tab_candidats[i].clepublic = candidat_potentiel.clepublic;
		tab_candidats[i].cleprive = candidat_potentiel.cleprive;
	}

    /*Ecrit les cles publiques des candidats dans le fichier candidat.txt*/
    f1 = fopen("candidates.txt", "w");
    if (f1==NULL){
        free_generate_random_data(nv, tab_citoyens, tab_candidats, f, NULL, NULL);
        printf("Erreur d'ouverture du fichier candidates.txt");
        return 0;
    }

    for(i = 0; i < nc ; i++){
		fprintf(f1, "pKey: (%lx,%lx)\n",tab_candidats[i].clepublic->val, tab_candidats[i].clepublic->n);
	}

    /*Genere une declaration de vote (pour un candidat aleatoirement choisi) pour chaque citoyen
    Ecrit la declaration dans declarations.txt*/
    f2 = fopen("declarations.txt", "w");
    if (f2==NULL){
        free_generate_random_data(nv, tab_citoyens, tab_candidats, f, f1, NULL);
        printf("Erreur d'ouverture du fichier declarations.txt");
        return 0;
    }

    Protected * pr;
    Key* pKey;
    Key* sKey;
    char* mess;
    Signature *sgn;
    for (i = 0; i < nv; i++){
        pKey = tab_citoyens[i].clepublic;
        sKey = tab_citoyens[i].cleprive;
        random = rand() % nc;
        mess = key_to_str(tab_candidats[random].clepublic); //vote pour une candidat aleatoirement choisi
        if (!mess){
            free_generate_random_data(nv, tab_citoyens, tab_candidats, f, f1, f2);
            return 0;
        }
        sgn = sign(mess, sKey);
        if (!sgn){
            free(mess);
            free_generate_random_data(nv, tab_citoyens, tab_candidats, f, f1, f2);
            return 0;
        }
        pr = init_protected(pKey, mess, sgn);
        if (!pr){
            free(mess);
            free(sgn->content);
            free(sgn);
            free_generate_random_data(nv, tab_citoyens, tab_candidats, f, f1, f2);
            return 0;
        }
        char *pr_str = protected_to_str(pr);
        if (!pr_str){
            free(mess);
            free(sgn->content);
            free(sgn);
            free(pr);
            free_generate_random_data(nv, tab_citoyens, tab_candidats, f, f1, f2);
            return 0;
        }
        fprintf(f2, "%s\n", pr_str); //ecrit la declaration dans le fichier declarations.txt

        //Libere toute memoire allouee a la fin du tour de boucle
        free(pr_str);
        free(mess);
        free(sgn->content);
        free(sgn);
        free(pr);
    }
    
    /*Libere la memoire et ferme les flux vers les fichiers*/
    free_generate_random_data(nv, tab_citoyens, tab_candidats, f, f1, f2);
    return 1;
}