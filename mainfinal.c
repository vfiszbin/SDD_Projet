#include "p4exo7.h"
#include "p4exo8.h"
#include "p4exo9.h"


int main(){
    int sizeC=5;
    int sizeV=1000;
    if(generate_random_data(int sizeV, int sizeC)==1){
        CellKey *lck_c = read_public_keys("candidates.txt");
	    if (!lck_c)
		    return 1;

	    printf("\nListe chaînée de clés lues avec read_public_keys :\n");
	    print_list_keys(lck_c);


        printf("\n");

	    //Test de read_protect
	    CellProtected *lcp = read_protected("declarations.txt");
	    if (!lcp){
		    delete_list_keys(lck_c);
		    return 1;
	    }
	    printf("\nListe chaînée de déclarations signées lues avec read_protected :\n");
	    print_list_cell_protected(lcp);
        CellKey *lck_v = read_public_keys("keys.txt");
	    if (!lck_v){
		    delete_list_keys(lck_c);
		    delete_list_cell(lcp);
		    return 1;
	    }
        int cpt=0;
        CellTree* tree=NULL;
        while(lcp){
            while(cpt<10){
                submit_vote(lcp->data);
                cpt++;
                lcp->next;
            }
            create_block(tree,lck_v->data,0);
            cpt=0;
            
        }
        add_block(0,"Block_valide");
        Cell_tree* tree_valide=read_tree();
        print_tree(arbre_valide);
        Key* gagnant = compute_winner_BT(tree_valide,lck_c,lck_v,sizeC,sizeV);
        printf("le gagnant est (%lx,%lx"),key_to_str(gagnant));
    }
    
}