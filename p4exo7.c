#include "p4exo7.h"

#define BUFFLEN 1024

void ecrire_fichier(char* nom, Block* b){
    FILE* f;
    f=fopen(nom,"w");
    if (!f){
		printf("Erreur d'ouverture de %s\n", nom);
		return NULL;
	}
    Protected * pr;
    char*pr_str;
    if (b){
        //tu pense que on peut avoir plusieurs vote ?
        fprintf("(%lx,%lx)\n",b->author->val, b->author->n);
        fprintf("%d\n",b->nb_votes);
        while(b->votes){
            pr=init_protected(b->votes->data->pKey,b->votes->data->mess,b->votes->data->sgn);
            if (!pr){
                fclose(f);
                return NULL;
            }
            pr_str=protected_to_str(pr);
            if (!pr_str){
                fclose(f);
                free(pr);
                return NULL;
            }
            fprintf("%s\n", pr_str);
            b->votes->next;
        }
        fprintf("%s\n",b->hash);
        fprintf("%s\n",b->previous_hash);
        fprintf("%d\n",b->nonce);
    }
    fclose(f);
}

Block * create_Block(Key *author, CellProtected *votes, unsigned char *hash, unsigned char *previous_hash, int nonce){
    Block* b =(Block*)malloc(sizeof(Block));
    if(!b){
        return NULL;
    }
    b->author=author;
    b->votes=votes;
    b->hash=hash;
    b->previous_hash=previous_hash;
    b->nonce=nonce;
    return b;
}

Block* lire_fichier(char* nom){
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
    if (sscanf("%s",&hash) != 1){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }

    //lit le hash du bloc precedent
    if (fgets(buffer, BUFFLEN, f) == NULL){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }
    if (sscanf("%s", &hash_precedent) != 1){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }

    //lit nonce
    if (fgets(buffer, BUFFLEN, f) == NULL){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }
    if (sscanf("%d", &nonce) != 1){
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
        return NULL;
    }


    Block *b = create_Block(cle,lcp,hash,hash_precedent,nonce);
    if(!b){       
        delete_list_cell(lcp);
        free(cle);
        fclose(f);
    }
    return b;
}

char* block_to_str(Block* block){
    char* key = key_to_str(block->author);
    if (!key)
        return NULL;
    char* listevotes = protected_to_str(block->votes->data);
    if (!listevotes)
        free(key);
        return NULL;
    int cpt=0;
    while (block->votes){
        cpt++;
        block=block->votes->next;
    }
    int size = strlen(key)+strlen(listevotes)*cpt+strlen(block->previous_hash)+strlen(block->nonce)+3+cpt;
    char* caractere = (char*)malloc(size*sizeof(char));
    if(!caractere){
        free(key);
        free(listevotes);
        return NULL;
    }
    int i=0;
    int j=0;
    while(key[j]){
        caractere[i]=key[j];
        i++;
        j++;
    }
    caractere[i] = ' ';
    i++;
    j=0;
    while(listevotes[j]){
        caractere[i]=listevotes[j];
        i++;
        j++;
    }
    caractere[i] = ' ';
    i++;
    j=0;
    while(block->previous_hash[j]){//pas sur de pouvoir copie ca 
        caractere[i]=block->previous_hash[j];
        i++;
        j++;
    }
    caractere[i] = ' ';
    i++;
    caractere[i]=block->nonce;
    i++;
    caractere[i]='\0';
    free(key);
    free(listevotes);
    return caractere;
}

unsigned char* crypteensha256 (char* message){
    unsigned char *d = SHA256(message,strlen(message),0);
    int i;
    for(i=0;i<SHA256_DIGEST_LENGTH;i++){
        printf("%02x",d[i]);
    }
    putchar('\n');
    return d;
}