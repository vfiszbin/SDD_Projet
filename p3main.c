#include "p3exo5.h"

int main(){
	//Test de read_public_keys
	CellKey *lck = read_public_keys("candidates.txt");
	if (!lck)
		return 1;
	printf("\nListe chaînée de déclarations clées lues avec read_public_keys :\n");
	print_list_keys(lck);

	delete_list_keys(lck);

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

	delete_list_cell(lcp);
	return 0;
}