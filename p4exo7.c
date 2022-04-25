#include "p4exo7.h"

#define BUFFLEN 1024

/*Ecrit la representation d'un Block b dans un fichier nom*/
int ecrire_block(char* nom, Block* b){
    FILE* f;
    f=fopen(nom,"w");
    if (!f){
		printf("Erreur d'ouverture de %s\n", nom);
		return 0;
	}
    CellProtected *vote;
    char*pr_str;
    if (b){
        fprintf(f, "(%lx,%lx)\n", b->author->val, b->author->n);
        fprintf(f, "%d\n",b->nb_votes);
        vote = b->votes;
        while(vote){
            pr_str=protected_to_str(vote->data);
            if (!pr_str){
                fclose(f);
                return 0;
            }
            fprintf(f, "%s\n", pr_str);

            free(pr_str);
            vote = vote->next;
        }
        write_hash_sha256(f, b->hash);
        write_hash_sha256(f, b->previous_hash);
        fprintf(f, "%d\n",b->nonce);
    }
    fclose(f);
    return 1;
}

/*Alloue et initialise les champs d'une structure Block*/
Block * init_block(Key *author, CellProtected *votes, unsigned char *hash, unsigned char *previous_hash, int nonce, int nb_votes){
    Block* b =(Block*)malloc(sizeof(Block));
    if(!b){
        return NULL;
    }
    b->author=author;
    b->votes=votes;
    b->hash=hash;
    b->previous_hash=previous_hash;
    b->nonce=nonce;
    b->nb_votes = nb_votes;
    return b;
}

/*Lit la representation d'un Block depuis un fichier nom. Alloue, initialise et retourne le block*/
Block* lire_block(char* nom){
    FILE* f = fopen(nom,"r");
    if (f==NULL){
		printf("Erreur d'ouverture de %s\n", nom);
		return NULL;
	}
    Key* cle=(Key*)malloc(sizeof(Key));
    if(!cle){
        fclose(f);
        return NULL;
    }
    char buffer[BUFFLEN];
    long val,n;
    Protected* pr;
    CellProtected* c;
	CellProtected* lcp = NULL;
    unsigned char* hash;
    unsigned char* hash_precedent;
    int nonce;
    int nb_votes;
    if (fgets(buffer, BUFFLEN, f) == NULL){
        free(cle);
        fclose(f);
        return NULL;
    }
    if (sscanf(buffer,"(%lx,%lx)",&val, &n) != 2){
        free(cle);
        fclose(f);
        return NULL;   
    }
    
    init_key(cle, val, n);
    
    //lit nb_votes
    if (fgets(buffer, BUFFLEN, f) == NULL){
        free(cle);
        fclose(f);
        return NULL;
    }
    if (sscanf(buffer,"%d",&nb_votes) != 1){
        free(cle);
        fclose(f);
        return NULL;   
    }

    //lit les déclarations de vote
    for (int i = 0; i < nb_votes; i++){
        if (fgets(buffer, BUFFLEN, f) == NULL){
            delete_list_cell(lcp);
            free(cle);
            fclose(f);
            return NULL;   
        }
        pr = str_to_protected(buffer);
        if (!pr){
            delete_list_cell(lcp);
            free(cle);
            fclose(f);
            return NULL;
        }
        c = create_cell_protected(pr);
        if (!c){
            free(pr);
            delete_list_cell(lcp);
            free(cle);
            fclose(f);
            return NULL;
            }
        add_cellProtected_to_tail(&lcp, c); //ajout de la declaration en fin de liste pour preserver la hash value des blocs contenant les declarations
    }

    //lit le hash du bloc
    if (fgets(buffer, BUFFLEN, f) == NULL){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }


    int len_buffer = strlen(buffer);
    hash = (unsigned char*)malloc(sizeof(unsigned char) * len_buffer + 1); //alloue une chaine d'unsigned char de la bonne taille
    if (!hash){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }
    if (read_hash_sha256(buffer, hash, len_buffer) == 0){
        delete_list_cell(lcp);
        free(cle);
        free(hash);
        fclose(f);
        return NULL;
    }

    //lit le hash du bloc precedent
    if (fgets(buffer, BUFFLEN, f) == NULL){
        delete_list_cell(lcp);
        free(cle);
        free(hash);
        fclose(f);
        return NULL;
    }

    if (strcmp(buffer, "NULL\n") == 0){ //cas de la racine dont le previous_hash est NULL
        hash_precedent = NULL;
    }
    else{
        len_buffer = strlen(buffer);
        hash_precedent = (unsigned char*)malloc(sizeof(unsigned char) * len_buffer + 1); //alloue une chaine d'unsigned char de la bonne taille
        if (!hash_precedent){
            delete_list_cell(lcp);
            free(cle);
            free(hash);
            fclose(f);
            return NULL;
        }
        if (read_hash_sha256(buffer, hash_precedent, len_buffer) == 0){
            delete_list_cell(lcp);
            free(cle);
            free(hash);
            free(hash_precedent);
            fclose(f);
            return NULL;
        }
    }

    //lit nonce
    if (fgets(buffer, BUFFLEN, f) == NULL){
        delete_list_cell(lcp);
        free(cle);
        free(hash);
        free(hash_precedent);
        fclose(f);
        return NULL;
    }
    if (sscanf(buffer,"%d", &nonce) != 1){
        delete_list_cell(lcp);
        free(cle);
        free(hash);
        free(hash_precedent);
        fclose(f);
        return NULL;
    }


    Block *b = init_block(cle,lcp,hash,hash_precedent,nonce, nb_votes);
    if(!b){
        delete_list_cell(lcp);
        free(cle);
        free(hash);
        free(hash_precedent);
        fclose(f);
    }

    fclose(f);
    return b;
}

/*Concatenation de deux chaines de caracteres s1 et s2. s1 est liberee si free_s1 ne vaut pas 0*/
char *strjoin(char *s1, char const *s2, int free_s1){
	size_t	i;
	size_t	j;
	char	*ret_str;

	if (!s1 || !s2)
		return (NULL);
	i = strlen(s1) + strlen(s2);
	ret_str = malloc(sizeof(char) * (i + 2));
	if (!ret_str){
        free(s1);
        return (NULL);
    }
	i = 0;
	j = -1;
	while (s1[++j])
	{
		ret_str[i] = s1[j];
		i++;
	}

    if (free_s1){ //si on free s1 c'est que strjoin est utilisee pour bloc_to_str
        //place un retour a la ligne entre les chaines
        ret_str[i] = '\n';
        i++;
    }

	j = -1;
	while (s2[++j])
	{
		ret_str[i] = s2[j];
		i++;
	}
	ret_str[i] = '\0';

    if (free_s1){
        free(s1); //libere la premiere chaine
    }
	return (ret_str);
}

/*Compte le nombre de chiffres composants un nombre (entier) positif*/
int	nb_of_digits(int n)
{
    int len = 0;
	while (n > 0)
	{
		len++;
		n /= 10;
	}
    return len;
}

/*Convertit un entier n sous sa representation en chaine de caracteres*/
char *int_to_str(int n){
    int nb_digits = nb_of_digits(n);
    char *n_str = (char*)malloc(sizeof(char)* nb_digits + 1);
    if (!n_str)
        return NULL;
    snprintf(n_str, nb_digits + 1, "%d", n);
    return n_str;
}



/*Genere une chaine de caracteres representant un bloc*/
char* block_to_str(Block* block){
    char *block_str;
    char* key = key_to_str(block->author);
    if (!key)
        return NULL;

    //Recupere le previous_hash
    char *previous_hash;
    if (block->previous_hash == NULL)
        previous_hash = "NULL"; //le bloc racine n'a pas de bloc precedent, on donne une valeur par defaut de 0
    else  
        previous_hash = (char *)block->previous_hash;

    block_str = strjoin(key, previous_hash, 1); //key est libéré dans la fonction
    if (!block_str){
        free(key);
        return NULL;
    }
    
    //passage des votes en chaine
    CellProtected *vote = block->votes;
    char * pr_str;
    while(vote){
        pr_str = protected_to_str(vote->data);
        if (!pr_str){
            free(block_str);
            return NULL;
        }
        block_str = strjoin(block_str, pr_str,1); // le block_str precedent est libere dans la fonction
        if(!block_str)
        {
            free(pr_str);
            return NULL;
        }
        free(pr_str);
        vote = vote->next;
    }

    char *nonce_str = (char*) malloc(sizeof(char) * 11); //alloue une chaine pouvant contenir jusqu'a INT_MAX + caractere nul de fin
    if (!nonce_str){
        free(block_str);
        return NULL;
    }
    int nb_char = sprintf(nonce_str, "%d", block->nonce);
    nonce_str[nb_char]= '\0';


    block_str = strjoin(block_str, nonce_str, 1);
    if (!block_str){
        free(nonce_str);
        return NULL;
    }
    free(nonce_str);

    return block_str;

}

/*Retourne la valeur hachee obtenue par l’algorithme SHA256 d'une chaine de caracteres message*/
unsigned char* crypt_to_sha256 (char* message){
    unsigned char *d = SHA256((unsigned char *)message, strlen(message), 0);
    unsigned char *hashed_value_of_block = malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH + 1); //alloue notre propre valeur hachee pour eviter des problemes
    if (!hashed_value_of_block)
        return NULL;
    int i;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        hashed_value_of_block[i] = d[i];
    hashed_value_of_block[i] = '\0';
    return hashed_value_of_block;
}

/*Supprime un bloc, mais ne libere pas son author et sa liste de votes*/
void delete_block(Block *b){
    if (b != NULL){
        free(b->hash);
        if (b->previous_hash != NULL)
            free(b->previous_hash); 
        free(b);
    }
}

/*Supprime un bloc completement*/
void full_delete_block(Block *b){
    if (b != NULL){
        free(b->author);
        delete_list_cell(b->votes);
        free(b->hash);
        if (b->previous_hash != NULL)
            free(b->previous_hash); 
        free(b);
    }
}

/*Teste si la valeur hachee commence par d zeros successifs*/
int d_succesive_zeros(unsigned char *hashed_value_of_block, int d){
    for(int i=0; i < d; i++){
        if (hashed_value_of_block[i] != 0)
            return 0;
    }
    return 1;
}

/*Incremente la valeur nonce du bloc B jusqu'a ce que la valeur hachee du bloc commence par d zeros successifs*/
int compute_proof_of_work(Block *B, int d){
    char *block_str = block_to_str(B);
    if (!block_str)
        return 0;

    unsigned char *hashed_value_of_block = crypt_to_sha256(block_str);
    if (!hashed_value_of_block){
        free(block_str);
        return 0;
    }
    
    if (B->hash != NULL)
        free(B->hash);
    B->hash = hashed_value_of_block;

    while(d_succesive_zeros(hashed_value_of_block, d) == 0){
        free(block_str);
        B->nonce++;
        block_str = block_to_str(B);
        if (!block_str)
            return 0;

        free(hashed_value_of_block);
        hashed_value_of_block = crypt_to_sha256(block_str);
        if (!hashed_value_of_block){
            free(block_str);
            return 0;
        }
        B->hash = hashed_value_of_block;
    }
    free(block_str);
    return 1;
}

/*Lit la valeur hachee (donnee en hexadecimale) d'un bloc*/
int read_hash_sha256(char*buffer, unsigned char * dest, int len_buffer){
    int i = 0;
    int j = 0;
    
    while (j < len_buffer - 1){
        // printf("\nj=%d\n", j);
        // printf("sscanf=%d\n",sscanf(buffer + j, "%02hhx", &dest[i]));
        if (sscanf(buffer + j, "%02hhx", &dest[i]) != 1)
            return 0;
        i++;
        j+=2 ;
    }
    dest[i] = '\0';
    return 1;
}

/*Ecrit la valeur hachee (donnee en hexadecimale) d'un bloc*/
void write_hash_sha256(FILE *f, unsigned char *hashed_value_of_block){
    if (hashed_value_of_block == NULL){ //cas ou on ecrit la racine
        fprintf(f, "NULL\n");
        return;
    }
    for(int i=0; i < SHA256_DIGEST_LENGTH; i++){
        fprintf(f, "%02x", hashed_value_of_block[i]);
    }
    fprintf(f, "\n");
}

/*Affiche la valeur hachee (donnee en hexadecimale) d'un bloc*/
void print_hash_sha256(unsigned char *hashed_value_of_block){
    for(int i=0; i < SHA256_DIGEST_LENGTH; i++){
        printf("%02x",hashed_value_of_block[i]);
    }
    putchar('\n');
}

/*Verifie qu'un bloc est valide : sa valeur hachee commence bien par d zeros successifs*/
int verify_block(Block *b, int d){
    char *block_str = block_to_str(b);
    if (!block_str)
        return 0;
    unsigned char *hashed_value_of_block = crypt_to_sha256(block_str);
    if (!hashed_value_of_block){
        free(block_str);
        return 0;
    }
    free(block_str);

    int res = d_succesive_zeros(hashed_value_of_block, d);
    free(hashed_value_of_block);
    return res;
}

void temps_moyen_computeproofofwork (Block *B) {
 //fonction qui evalue le temps_moyen de la fonction compute_proof_of_work en fonction de d 

     FILE* fic = fopen("temps_proof_of_work.txt","w");
     if(fic == NULL){
         printf("erreur à l’ouverture du fichier ");
         return;
     }
     clock_t temps_initial;
     clock_t temps_final;
     double temps_cpu = 0;

     for(int i = 0; i < 4; i++){
         temps_initial = clock();
         compute_proof_of_work(B,i);
         temps_final = clock();
         temps_cpu = ((double)(temps_final - temps_initial )) / CLOCKS_PER_SEC;
         printf("time : %lf \n",temps_cpu);
         fprintf(fic ,"%d %lf \n ",i,temps_cpu);
     }
     fclose(fic);
}