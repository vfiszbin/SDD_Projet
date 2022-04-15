#include "p4exo7.h"
#include "p4exo8.h"

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
    //Creation des blocs
    Block *b1 = create_test_block(1);
    if (!b1){
        return 1;
    }
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
    CellTree* node2 = create_node(b2);
    CellTree* node3 = create_node(b3);
    CellTree* node4 = create_node(b4);


    //Tests add_child et print_tree2D
    add_child(node1,node2);
    add_child(node1,node3);
    add_child(node3,node4);

    print_tree2D(node1, 0);

    //Tests highest_child
    printf("\nhighest_child=%s\n", highest_child(node1)->block->hash);

    delete_tree(node1);

    return 0;


 
}