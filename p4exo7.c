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
        fprintf(f, "%s\n",b->hash);
        fprintf(f, "%s\n",b->previous_hash);
        fprintf(f, "%d\n",b->nonce);
    }
    fclose(f);
    return 1;
}

/*Alloue et initialise les champs d'une structure Block*/
Block * create_Block(Key *author, CellProtected *votes, unsigned char *hash, unsigned char *previous_hash, int nonce, int nb_votes){
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
        add_cellProtected_to_front(&lcp, c);
    }

    //lit le hash du bloc
    if (fgets(buffer, BUFFLEN, f) == NULL){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }
    hash = (unsigned char*)malloc(sizeof(unsigned char) * strlen(buffer)); //alloue une chaine d'unsigned char de la bonne taille
    if (!hash){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }
    if (sscanf(buffer,"%s",hash) != 1){
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
    hash_precedent = (unsigned char*)malloc(sizeof(unsigned char) * strlen(buffer)); //alloue une chaine d'unsigned char de la bonne taille
    if (!hash_precedent){
        delete_list_cell(lcp);
        free(cle);
        free(hash);
        fclose(f);
        return NULL;
    }
    if (sscanf(buffer,"%s", hash_precedent) != 1){
        delete_list_cell(lcp);
        free(cle);
        free(hash);
        free(hash_precedent);
        fclose(f);
        return NULL;
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


    Block *b = create_Block(cle,lcp,hash,hash_precedent,nonce, nb_votes);
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

/*Concatenation de deux chaines de caracteres s1 et s2. s1 est liberee*/
char *strjoin(char *s1, char const *s2)
{
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

    //place un retour a la ligne entre les chaines
    ret_str[i] = '\n';
    i++;

	j = -1;
	while (s2[++j])
	{
		ret_str[i] = s2[j];
		i++;
	}
	ret_str[i] = '\0';

    free(s1); //libere la premiere chaine
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

/*Genere une chaine de caracteres representant un bloc*/
char* block_to_str(Block* block){
    char *block_str;
    char* key = key_to_str(block->author);
    if (!key)
        return NULL;

    block_str = strjoin(key, (char *)block->previous_hash); //key est libéré dans la fonction
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
        block_str = strjoin(block_str, pr_str); // le block_str precedent est libere dans la fonction
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


    block_str = strjoin(block_str, nonce_str);
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
    int i;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        hashed_value_of_block[i] = d[i];
    hashed_value_of_block[i] = '\0';
    return hashed_value_of_block;
}

/*Supprime un bloc, mais ne libere pas son author et les protected de sa liste de votes*/
void delete_block(Block *b){
    if (b != NULL){
        CellProtected* tmp;
        CellProtected* vote;
        vote = b->votes;
        while(vote){
            tmp = vote;
            vote = vote->next;
            free(tmp);
        }

        free(b->hash);
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
void compute_proof_of_work(Block *B, int d){
    char *block_str = block_to_str(B);
    if (!block_str)
        return;
    
    unsigned char *hashed_value_of_block = crypt_to_sha256(block_str);
    if (!hashed_value_of_block){
        free(block_str);
        return;
    }
    if (B->hash != NULL)
        free(B->hash);
    B->hash = hashed_value_of_block;

    while(d_succesive_zeros(hashed_value_of_block, d) == 0){
        free(block_str);
        B->nonce++;
        block_str = block_to_str(B);
        if (!block_str)
            return;

        free(hashed_value_of_block);
        hashed_value_of_block = crypt_to_sha256(block_str);
        if (!hashed_value_of_block){
            free(block_str);
            return;
        }
        B->hash = hashed_value_of_block;
    }
    free(block_str);
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



int main(){
    //Tests create_Block et ecricre_block
    Key *k = malloc(sizeof(Key));
    if (!k)
        return 1;
    k->n = 123;
    k->val = 456;

    CellProtected *lcp = read_protected("declarations.txt");
	if (!lcp){
		free(k);
		return 1;
	}
    unsigned char *h = malloc(sizeof(unsigned char) * 3);
    if (!h){
        free(k);
        delete_list_cell(lcp);
        return 1;
    }
    h[0] = 'h'; 
    h[1] = '1'; 
    h[2] = '\0'; 
    unsigned char *prev_h = malloc(sizeof(unsigned char) * 3);
    if (!prev_h){
        free(k);
        delete_list_cell(lcp);
        free(h);
        return 1;
    }
    prev_h[0] = 'h'; 
    prev_h[1] = '0'; 
    prev_h[2] = '\0'; 


    CellProtected* tmp = lcp;
	int nb_votes = 0;
	while(tmp){ //calcule la taile de la liste
		nb_votes++;
		tmp = tmp->next;
	}

    Block *b = create_Block(k, lcp, h, prev_h, 0, nb_votes);
    if (!b){
        free(k);
        delete_list_cell(lcp);
        free(h);
        free(prev_h);
        return 1;
    }

    if (ecrire_block("blocks.txt", b) == 0){
        free(b->author);
        delete_list_cell(b->votes);
        b->votes = NULL;
        delete_block(b);
        return 1;
    }

    free(b->author);
    delete_list_cell(b->votes);
    b->votes = NULL;
    delete_block(b);

    //Tests lire_block
    Block *b2 = lire_block("blocks.txt");
    if (!b2)
        return 1;

    if (ecrire_block("blocks.txt", b2) == 0){ //doit ecrire la meme chose que b dans blocks.txt
        free(b2->author);
        delete_list_cell(b2->votes);
        b2->votes = NULL;
        delete_block(b2);
        return 1;
    }

    //Tests block_to_str
    char *block_str = block_to_str(b2);
    printf("block_str=\n%s\n", block_str);

    free(block_str);

    //Tests crypt_to_sha256
    unsigned char *hashed_value_of_block = crypt_to_sha256("Rosetta code");
    printf("\n'Rosetta code' crypté en sha256 =\n");
    print_hash_sha256(hashed_value_of_block);

    free(hashed_value_of_block);

    //Tests compute_proof_of_work
    int d = 2; // nombre de zeros demande
    compute_proof_of_work(b2, d);
    printf("\nPour une proof of work avec %d zéros\n", d);
    printf("nonce=%d\n", b2->nonce);
    printf("valeur hachée du bloc=\n");
    print_hash_sha256(b2->hash);

    //Tests verify_block
    printf("\nverify_block sur block valide=%d\n", verify_block(b2, d));
    b2->nonce++; //modifie une champ du block, ce qui compromet son integrite
    printf("verify_block sur block modifié=%d\n", verify_block(b2, d));

    free(b2->author);
    delete_list_cell(b2->votes);
    b2->votes = NULL;
    delete_block(b2);
}