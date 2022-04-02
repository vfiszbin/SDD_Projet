#include "p3exo5.h"
#include "p3exo6.h"

int main(){
	//Test de read_public_keys
	CellKey *lck_c = read_public_keys("candidates.txt");
	if (!lck_c)
		return 1;

	printf("\nListe chaînée de clés lues avec read_public_keys :\n");
	print_list_keys(lck_c);


    printf("\n");

	//Test de read_protect
	CellProtected *lcp = read_protected("declarations.txt");
	if (!lcp){
		return 1;
	}
	printf("\nListe chaînée de déclarations signées lues avec read_protected :\n");
	print_list_cell_protected(lcp);

	//Test de supprime_declarations_non_valides
	supprime_declarations_non_valides(&lcp);
	printf("\nListe chaînée de déclarations signées après la suppression des déclarations dont la signature est invalide :\n");
	print_list_cell_protected(lcp);

	

	//Test de create_HashTable et delete_hashtable
	CellKey *tmp = lck_c;
	int list_size_c = 0;
	while(tmp){ //calcule la taile de la liste
		list_size_c++;
		tmp = tmp->next;
	}
	HashTable *t = create_HashTable(lck_c, list_size_c * 2);
	printf("%d\n", find_position(t, lck_c->data));

	
	delete_hashtable(t);

	//Test de compute_winner
	CellKey *lck_v = read_public_keys("keys.txt");
	if (!lck_v)
		return 1;

	tmp = lck_v;
	int list_size_v = 0;
	while(tmp){ //calcule la taile de la liste
		list_size_v++;
		tmp = tmp->next;
	}

	Key *winner_pkey = compute_winner(lcp, lck_c, lck_v, list_size_c * 2, list_size_v * 2);
	printf("Winner pkey : (%lx,%lx)\n", winner_pkey->val, winner_pkey->n);
	
	delete_list_keys(lck_c);
	delete_list_keys(lck_v);
	delete_list_cell(lcp);

	return 0;
}