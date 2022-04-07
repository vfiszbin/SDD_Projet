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
    while(b){
        //tu pense que on peut avoir plusieurs vote ?
        fprintf("%lx,%lx\n",b->autor->n,b->autor->val);
        while(b->votes){
            pr=init_protected(b->votes->data->pKey,b->votes->data->mess,b->votes->data->sgn);
            char*pr_str=protected_to_str(pr);
            fprintf("%s\n",pr_str);
            b->votes->next;
        }
        fprintf("%s\n",b->hash);
        fprintf("%s\n",b->previous_hash);
        fprintf("%d\n",b->nonce);
    }
    fclose(f);
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
    char* hash;
    char* hash_precedent;
    int nonce;
    fgets(buffer, BUFFLEN, f);
    sscanf(buffer,"(%lx,%lx)",&val,&n);
    init_key(val,n,cle);
    //si plusieurs vote je ne sais pas comment analyse plusieurs ligne de vote
    fgets(buffer, BUFFLEN, f);
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
    fgets(buffer, BUFFLEN, f);
    sscanf("%s",&hash);
    fgets(buffer, BUFFLEN, f);
    sscanf("%s",&hash_precedent);
    fgets(buffer, BUFFLEN, f);
    sscanf("%d",&nonce);
    Block* b =(Block*)malloc(sizeof(Block));
        if(!b){
            delete_list_cell(lcp);
            free(cle);
            fclose(f);
        }
    b->autor=cle;
    b->votes->data=lcp;
    b->hash=hash;
    b->previous_hash=hash_precedent;
    b->nonce=nonce;

    return b;
}

char* block_to_str(Block* block){
    char* key = key_to_str(block->autor);
    if (!key)
        return NULL;
    char* listevotes = protected_to_str(block->votes->data);
    if (!listevotes)
        return NULL;
    int cpt=0;
    while (block->votes){
        cpt++;
        block=block->votes->next;
    }
    int size = strlen(key)+strlen(listevotes)*cpt+strlen(block->previous_hash)+strlen(block->nonce)+3+cpt;
    char* caractere = (char*)malloc(size*sizeof(char));
    if(!caractere){
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