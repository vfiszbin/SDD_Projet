#include "p4exo9.h"


/*Un cityoyen soumet son vote, c'est a dire qu'il est ajoute en fin du fichier Pending_votes.txt*/
void submit_vote(Protected* pr){
    if(!pr)
        return;

    FILE * f = fopen("Pending_votes.txt", "a"); //ouverture du flux en mode append
    if(!f){
        printf("Erreur d'ouverture du fichier Pending_votes.txt");
        return;
    }
    char* pr_str = protected_to_str(pr);
    if (!pr_str){
        fclose(f);
        return;
    }
    fprintf(f, "%s\n", pr_str);
    free(pr_str);
    fclose(f);
}

// void create_block(CellTree* tree, Key* author, int d){
//     CellProtected* votes = read_protected("Pending.txt");
//     CellTree* dernier=last_node(tree);

//     Block* b=(Block*)malloc(sizeof(Block));
//     if(!b){
//         delete_list_cell(votes);
//         return;
//     }
//     b->author;
//     b->votes=votes;
//     if(dernier!=NULL){
//         b->previous_hash=dernier->block->hash;
//     }else{
//         b->previous_hash=NULL;
//     }
//     b->nonce=0;
//     compute_proof_of_work(b,d);
//     dernier->firstChild=create_node(b);
//     update_height(dernier,dernier->firstChild);
//     ecrire_block("Pending_block",b);
//     remove("pending.txt");
// }

