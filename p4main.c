#include "p2exo4.h"
#include "p4exo7.h"
#include "p4exo8.h"
#include "p4exo9.h"

/*Cree un bloc pour pouvoir tester les fonctions de la partie 4
nb est le numero de bloc et doit etre compris entre 1 et 9*/
Block * create_test_block(unsigned char * previous_hash, int d, int num){
    Key *k = malloc(sizeof(Key));
    if (!k)
        return NULL;
    k->n = 123 + num; //num sert juste a differencier les blocks pour ne pas avoir la meme valeur hachee
    k->val = 456;

    CellProtected *lcp = read_protected("declarations.txt");
	if (!lcp){
		free(k);
		return NULL;
	}

    CellProtected* tmp = lcp;
	int nb_votes = 0;
	while(tmp){ //calcule la taile de la liste
		nb_votes++;
		tmp = tmp->next;
	}

    //duplique le previous_hash si non NULL
    unsigned char *new_previous_hash;
    if (previous_hash == NULL)
        new_previous_hash = NULL;
    else{
        new_previous_hash = malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH + 1);
        if (!new_previous_hash){
            free(k);
            delete_list_cell(lcp);
            return NULL;
        }
        int i;
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
            new_previous_hash[i] = previous_hash[i];
        new_previous_hash[i] = '\0';
    }

    Block *b = init_block(k, lcp, NULL, new_previous_hash, 0, nb_votes);
    if (!b){
        free(k);
        delete_list_cell(lcp);  
        return NULL;
    }

    //Calcule la proof of work du bloc
    if (compute_proof_of_work(b, d) == 0){ //la valeur hachée du bloc est maj dans la fonction
        full_delete_block(b);
        return NULL;
    }

    return b;
}


int main(){
    //Genere citoyens, candidats et declarations de votes
    if (! generate_random_data(10,3)){
		printf("Erreur allocation mémoire\n");
		return 1;
	}
    
    //////////////////////Tests Exercice 7/////////////////////////////////////
    printf("---TESTS EXERCICE 7---\n");
    //Tests init_block, creation d'un bloc test
    Block *b1 = create_test_block(NULL, 1, 1); //bloc initial
    if (!b1){
        return 1;
    }
    printf("\nHASH avant ecriture=\n");
    print_hash_sha256(b1->hash);
    
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
    printf("\nHASH apres lecture=\n");
    print_hash_sha256(b_read->hash);

    
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
    int d = 1; // nombre de zeros demande
    if (compute_proof_of_work(b_read, d) == 0){
        full_delete_block(b1);
        full_delete_block(b_read);
        return 1;  
    }

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
    Block *b2 = create_test_block(b1->hash, 1, 2);
    if (!b2){
        full_delete_block(b1);
        return 1;
    }

    Block *b3 = create_test_block(b2->hash, 1, 3);
    if (!b3){
        full_delete_block(b1);
        full_delete_block(b2);
        return 1;
    }

    Block *b4 = create_test_block(b3->hash, 1, 4);
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
    printf("\nhighest_child=");
    print_hash_sha256(highest_child(node1)->block->hash);
    printf("\nlast_node=");
    print_hash_sha256(last_node(node1)->block->hash);
    

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

    full_delete_tree(node1); //supprime l'entierete de l'arbre

    CellTree * test_tree = NULL;
    //Cree un auteur
    Key *test_k = malloc(sizeof(Key));
    if (!test_k){
        full_delete_tree(node1);
        return 1;
    }
        
    test_k->n = 123;
    test_k->val = 456;
    

    if (create_block(&test_tree, test_k, 2) == 0){ //Auteur supprimer dans la fonction en meme temps que son block
        free(test_k);
        return 1;
    }
    free(test_k);
    full_delete_tree(test_tree);
    
    if (add_block(2, "block1") == 0)
        return 1;
    
    CellTree *blockchain_tree = read_tree();
    if (!blockchain_tree)
        return 1;
    print_tree2D(blockchain_tree, 0);

    full_delete_tree(blockchain_tree);

    return 0;

}