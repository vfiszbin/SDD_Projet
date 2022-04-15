
#include "p4exo7.h"
#include "p4exo8.h"



int main(){
    //Creation d'un bloc 1
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

    Block *b = create_Block(k, lcp, h, prev_h, 111, nb_votes);
    if (!b){
        free(k);
        delete_list_cell(lcp);
        free(h);
        return 1;
    }

    /////////
    //Creation d'un bloc 2

    Key *k2 = malloc(sizeof(Key));
    if (!k2)
        return 1;
    k2->n = 123;
    k2->val = 456;

    CellProtected *lcp1 = read_protected("declarations.txt");
	if (!lcp1){
		free(k2);
		return 1;
	}
    unsigned char *h2 = malloc(sizeof(unsigned char) * 3);
    if (!h2){
        free(k2);
        delete_list_cell(lcp1);
        return 1;
    }
    h2[0] = 'h'; 
    h2[1] = '2'; 
    h2[2] = '\0'; 
    unsigned char *prev_h2 = malloc(sizeof(unsigned char) * 3);
    if (!prev_h2){
        free(k2);
        delete_list_cell(lcp1);
        free(h2);
        return 1;
    }
    prev_h2[0] = 'h'; 
    prev_h2[1] = '0'; 
    prev_h2[2] = '\0'; 


    CellProtected* tmp2 = lcp1;
	int nb_votes2 = 0;
	while(tmp2){ //calcule la taile de la liste
		nb_votes2++;
		tmp2 = tmp2->next;
	}

    Block *b2 = create_Block(k2, lcp1, h2, prev_h2, 111, nb_votes2);
    if (!b2){
        free(k2);
        delete_list_cell(lcp1);
        free(h2);
        return 1;
    }

    //////////
    //Creation d'un bloc 3

    Key *k3 = malloc(sizeof(Key));
    if (!k3)
        return 1;
    k3->n = 123;
    k3->val = 456;

    CellProtected *lcp2 = read_protected("declarations.txt");
	if (!lcp2){
		free(k3);
		return 1;
	}
    unsigned char *h3 = malloc(sizeof(unsigned char) * 3);
    if (!h3){
        free(k3);
        delete_list_cell(lcp2);
        return 1;
    }
    h3[0] = 'h'; 
    h3[1] = '3'; 
    h3[2] = '\0'; 
    unsigned char *prev_h3 = malloc(sizeof(unsigned char) * 3);
    if (!prev_h3){
        free(k3);
        delete_list_cell(lcp2);
        free(h3);
        return 1;
    }
    prev_h3[0] = 'h'; 
    prev_h3[1] = '0'; 
    prev_h3[2] = '\0'; 


    CellProtected* tmp3 = lcp2;
	int nb_votes3 = 0;
	while(tmp3){ //calcule la taile de la liste
		nb_votes3++;
		tmp3 = tmp3->next;
	}

    Block *b3 = create_Block(k3, lcp2, h3, prev_h3, 111, nb_votes3);
    if (!b3){
        free(k3);
        delete_list_cell(lcp2);
        free(h3);
        return 1;
    }
    ////////

    CellTree* node1 = create_node(b);
    CellTree* node2 = create_node(b2);
    CellTree* node3 = create_node(b3);

    // print_tree(tree);
    // print_tree(tree2);
    // print_tree(tree3);
    add_child(node1,node2);
    add_child(node2,node3);
    // tree->nextBro=tree3;
    // print_tree(tree);

    // printf("racine=%s\n", tree->block->hash);
    // printf("firstChild=%s\n", tree->firstChild->block->hash);
    // printf("nextBro=%s\n", tree->firstChild->nextBro->block->hash);
    // printf("firstChildOfFirstChild=%s\n", tree->firstChild->firstChild->block->hash);


    print_tree2D(node1, 0);

    delete_tree(node1);

    return 0;


 
}