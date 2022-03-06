#include "p1exo1.h"
#include "p1exo2.h"

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

int main(){
	long s = 477;
	long t = 344;
	long u, v;
	long pgcd = extended_gcd(s,t,&u,&v);
	printf("s=%ld * u=%ld + t=%ld * v=%ld = pgcd(s,t)=%ld\n", s, u, t, v, pgcd);

	int k = 10; //nb de tests de Miller a realiser pour generer un nombre premier
	int low_size = 4; //nb de bits min des nb premiers generes aleatoirement
	int up_size = 10; //nb de bits max des nb premiers generes aleatoirement
	long p = random_prime_number(low_size, up_size, k);
	long q = random_prime_number(low_size, up_size, k);
	long n;
	generate_key_values(p, q, &n, &s, &u);
	printf("pKey=(s=%ld,n=%ld), sKey=(u=%ld,n=%ld\n", s, n, u, n);
	
}