#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

// int main(){
// 	const char *s = "Rosetta code";
// 	unsigned char *d =SHA256(s,strlen(s),0);
// 	int i;
// 	for(i=0;i<SHA256_DIGEST_LENGTH;i++){
//     	printf("%02x",d[i]);    
// 	}
// 	putchar('\n');

// 	printf("%hhn\n", d);
// 	return 0;}

#include <dirent.h>
int main(){
	DIR *rep = opendir("./Blockchain/");
	if (rep != NULL){
		struct	dirent	* dir;
		while ((dir = readdir(rep))){
			if (strcmp(dir->d_name, ".") != 0 && strcmp (dir->d_name, "..") != 0){
				printf("Chemin du fichier : /Blockchain/%s \n", dir->d_name);
			}
		}
		closedir(rep);
	}
}
