#include "p4exo9.h"


void submit_vote(Protected* pr){
    if(!pr){
        return;
    }
    FILE * f=fopen("Pending.txt","w");
    if(!f){
        printf("probleme d'ouverture du fichier pending.txt");
        return;
    }
    char* c = str_to_protected(pr);
    fprintf("c",pr);
    free(c);
    fclose(f);
}

void create_block(CellTree* tree,Key* author,int d){
    CellProtected* votes = read_protected("Pending.txt");
    CellTree* dernier=last_node(tree);

    Block* b=(Block*)malloc(sizeof(Block));
    if(!b){
        delete_list_cell(vote);
        return;
    }
    b->author;
    b->votes=votes;
    if(dernier!=NULL){
        b->previous_hash=dernier->block->hash;
    }else{
        b->previous_hash=NULL;
    }
    b->nonce=0;
    compute_proof_of_work(b,d);
    dernier->firstChild=create_node(b);
    update_height(dernier,dernier->firstChild);
    ecrire_block("Pending_block",b);
    remove("pending.txt");
}

