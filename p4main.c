#include "p4exo7.h"
#include "p4exo8.h"
#include "p4exo9.h"

/*Cree un bloc pour pouvoir tester les fonctions de la partie 4
nb est le numero de bloc et doit etre compris entre 1 et 9*/
Block * create_test_block(int nb){
    Key *k = malloc(sizeof(Key));
    if (!k)
        return NULL;
    k->n = 123;
    k->val = 456;

    CellProtected *lcp = read_protected("declarations.txt");
	if (!lcp){
		free(k);
		return NULL;
	}
    unsigned char *h = malloc(sizeof(unsigned char) * 3);
    if (!h){
        free(k);
        delete_list_cell(lcp);
        return NULL;
    }
    h[0] = 'h'; 
    h[1] = '0' + nb % 10; //on ne cree pas plus de 10 blocks dans les tests, apres ca la valeur de hachage reboucle
    h[2] = '\0'; 
    unsigned char *prev_h = malloc(sizeof(unsigned char) * 3);
    if (!prev_h){
        free(k);
        delete_list_cell(lcp);
        free(h);
        return NULL;
    }
    prev_h[0] = 'h'; 
    prev_h[1] = '0' + nb % 10 - 1;; 
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
        return NULL;
    }

    return b;
}


int main(){
    //////////////////////Tests Exercice 7/////////////////////////////////////
    printf("---TESTS EXERCICE 7---\n");
    //Tests create_Block, creation d'un bloc test
    Block *b1 = create_test_block(1);
    if (!b1){
        return 1;
    }

    //Tests ecrire_block
    if (ecrire_block("blocks.txt", b1) == 0){
        full_delete_block(b1);
        return 1;
    }

    //Tests lire_block
    Block *b_read = lire_block("blocks.txt");
    if (!b_read){
        full_delete_block(b1);
        return 1;
    }
        

    if (ecrire_block("blocks.txt", b_read) == 0){ //doit ecrire la meme chose que b dans blocks.txt
        full_delete_block(b1);
        full_delete_block(b_read);
        return 1;
    }

    //Tests block_to_str
    char *block_str = block_to_str(b_read);
    if (!block_str){
        full_delete_block(b1);
        full_delete_block(b_read);
        return 1;
    }
    printf("block_str=\n%s\n", block_str);

    free(block_str);

    //Tests crypt_to_sha256
    unsigned char *hashed_value_of_block = crypt_to_sha256("Rosetta code");
    if (!hashed_value_of_block){
        full_delete_block(b1);
        full_delete_block(b_read);
        return 1;  
    }
    printf("\n'Rosetta code' crypté en sha256 =\n");
    print_hash_sha256(hashed_value_of_block);

    free(hashed_value_of_block);

    //Tests compute_proof_of_work
    int d = 2; // nombre de zeros demande
    compute_proof_of_work(b_read, d);
    printf("\nPour une proof of work avec %d zéros\n", d);
    printf("nonce=%d\n", b_read->nonce);
    printf("valeur hachée du bloc=\n");
    print_hash_sha256(b_read->hash);

    //Tests verify_block
    printf("\nverify_block sur block valide=%d\n", verify_block(b_read, d));
    b_read->nonce++; //modifie une champ du block, ce qui compromet son integrite
    printf("verify_block sur block modifié=%d\n", verify_block(b_read, d));

    full_delete_block(b_read);



    //////////////////////Tests Exercice 8/////////////////////////////////////
    printf("\n---TESTS EXERCICE 8---\n");

    //Création de blocs supplémentaires
    Block *b2 = create_test_block(2);
    if (!b2){
        full_delete_block(b1);
        return 1;
    }

    Block *b3 = create_test_block(3);
    if (!b3){
        full_delete_block(b1);
        full_delete_block(b2);
        return 1;
    }

    Block *b4 = create_test_block(4);
    if (!b4){
        full_delete_block(b1);
        full_delete_block(b2);
        full_delete_block(b3);
        return 1;
    }

    //Creation des noeuds
    CellTree* node1 = create_node(b1);
    if (!node1){
        full_delete_block(b1);
        full_delete_block(b2);
        full_delete_block(b3);
        full_delete_block(b4);
        return 1;
    }
    CellTree* node2 = create_node(b2);
    if (!node2){
        full_delete_block(b1);
        full_delete_block(b2);
        full_delete_block(b3);
        full_delete_block(b4);
        full_delete_node(node1);
        return 1;
    }
    CellTree* node3 = create_node(b3);
    if (!node3){
        full_delete_block(b1);
        full_delete_block(b2);
        full_delete_block(b3);
        full_delete_block(b4);
        full_delete_node(node1);
        full_delete_node(node2);
        return 1;
    }
    CellTree* node4 = create_node(b4);
    if (!node4){
        full_delete_block(b1);
        full_delete_block(b2);
        full_delete_block(b3);
        full_delete_block(b4);
        full_delete_node(node1);
        full_delete_node(node2);
        full_delete_node(node3);
        return 1;
    }

    //Tests add_child et print_tree2D
    add_child(node1,node2);
    add_child(node1,node3);
    add_child(node3,node4);

    print_tree2D(node1, 0);

    //Tests highest_child et last_node
    printf("\nhighest_child=%s\n", highest_child(node1)->block->hash);
    printf("\nlast_node=%s\n", last_node(node1)->block->hash);
    

    //Tests fusion_liste_protected et fusion_votes_arbre
    CellProtected *liste_votes = fusion_votes_arbre(node1);
    if (!liste_votes){
        full_delete_block(b1);
        full_delete_block(b2);
        full_delete_block(b3);
        full_delete_block(b4);
        full_delete_tree(node1);
        return 1;
    }
    printf("\nListe des votes de la plus longue branche de l'arbre :\n");
    print_list_cell_protected(liste_votes);

    delete_list_cell(liste_votes);




    //////////////////////Tests Exercice 9/////////////////////////////////////
    printf("\n---TESTS EXERCICE 9---\n");
    submit_vote(b1->votes->data);


    full_delete_tree(node1);
    return 0;
}