#include "p3exo5.h"

int main(){
	//Test de read_public_keys
	CellKey *lck = read_public_keys("candidates.txt");
	if (!lck)
		return 1;
	print_list_keys(lck);

	delete_list_keys(lck);
	return 0;
}