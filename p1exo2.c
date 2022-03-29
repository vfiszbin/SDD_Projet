#include "p1exo1.h"
#include "p1exo2.h"

/*Retourne pgcd(s,t) tel que s * u + t * v = pgcd(s,t) (algorithme d'Euclide etendu)*/
long extended_gcd (long s, long t, long *u, long *v) {
	if (s == 0){
		*u = 0;
		*v = 1;
		return t;
	}
	long uPrim, vPrim;
	long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
	*u = vPrim - (t/s) * uPrim;
	*v = uPrim;
	return gcd;
}

/*Genere la cle publique pKey=(s,n) et la cle secrete sKey=(u,n) a partir des nombres premiers p et q
selon le protocole RSA*/
void generate_key_values(long p, long q, long *n, long *s, long*u){
	*n = p * q;
	long t = (p - 1) * (q - 1);
	long v;
	long pgcd = 0;
	while (pgcd != 1){
		*s = rand_long(1, t - 1); //entier aléatoire inferieur a t (peut etre negatif ???)
		pgcd = extended_gcd(*s,t,u,&v);
	}
}

/*Chiffre la chaine de caracteres chaine avec la cle publique pKey(s,n)*/
long *encrypt(char *chaine, long s, long n){
	long *crypted = (long*) malloc(sizeof(long) * strlen(chaine)); //tab contenant les entiers chiffres correspondants a chaque char de chaine
	if (!crypted)
		return NULL;
	int i = 0;
	long m;
	while(chaine[i]){
		m = chaine[i];
		crypted[i] = modpow(m, s, n); //encrypte le char. m doit etre inferieur a n !
		i++;
	}
	return crypted;
}

/*Retourne la chaine dechiffree a l'aide de la cle secrete sKey(u,n), en connaissant la taille size
du tableau crypted contenant les entiers chiffres*/
char *decrypt (long *crypted, int size, long u, long n){
	char *chaine = (char*) malloc(sizeof(char) * size + 1);
	if (!chaine)
		return NULL;
	int i;
	long c;
	for (i = 0; i < size; i++){
		c = crypted[i];
		chaine[i] = modpow(c, u, n); //decrypte le char
	}
	chaine[i] = '\0';
	return chaine;
}

// int main(){
// 	srand(time(NULL));

// 	//Test extended_gcd
// 	long s = 477;
// 	long t = 344;
// 	long u, v;
// 	long pgcd = extended_gcd(s,t,&u,&v);
// 	printf("s=%ld * u=%ld + t=%ld * v=%ld = pgcd(s,t)=%ld\n", s, u, t, v, pgcd);

// 	//Test generate_key_values
// 	int k = 10; //nb de tests de Miller a realiser pour generer un nombre premier
// 	int low_size = 4; //nb de bits min des nb premiers generes aleatoirement
// 	int up_size = 10; //nb de bits max des nb premiers generes aleatoirement
// 	long p = random_prime_number(low_size, up_size, k);
// 	long q = random_prime_number(low_size, up_size, k);
// 	long n;
// 	generate_key_values(p, q, &n, &s, &u);
// 	printf("pKey=(s=%ld,n=%ld), sKey=(u=%ld,n=%ld)\n", s, n, u, n);

// 	char *chaine="tourte";
// 	int len_chaine = strlen(chaine);
// 	long *crypted = encrypt(chaine,s,n);
// 	if (!crypted){
// 		printf("Erreur allocation mémoire\n");
// 		return 1;
// 	}
// 	printf("Chaîne à encrypter=%s\nTableau des caractères encryptés de la chaîne :\n",chaine);
// 	for (int i = 0; i < len_chaine; i++)
// 		printf("%ld ", crypted[i]);
// 	printf("\n");

// 	char *chaine_decryptee = decrypt(crypted, len_chaine, u, n);
// 	if (!chaine_decryptee){
// 		printf("Erreur allocation mémoire\n");
// 		free(crypted);
// 		return 1;
// 	}
// 	printf("Chaîne décryptée=%s\n", chaine_decryptee);

// 	free(crypted);
//	free(chaine_decryptee)
// 	return 0;
// }