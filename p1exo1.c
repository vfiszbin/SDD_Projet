#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

clock_t temps_initial;
clock_t temps_final;
double temps_cpu1 , temps_cpu2;

/*Determine si un entier p est premier ou non*/
int is_prime_naive(long p){
    int i;
	if (p == 2) //cas particulier, 2 est le seul nombre premier pair
		return 1;
	if (p < 3 || p % 2 == 0) //si le nombre est inférieur a 3 ou n'est pas impair, il ne peut pas etre premier
		return 0;

	for(i=3; i<p; i++){
		if(p%i==0) //si l'un de ces entiers est diviseur de p, alors p n'est pas premier
			return 0;
	}
	return 1;

}
long modpow_naive(long a,long m, long n){
    int res=1;
    for (int j=0;j<m;j++){
        res=(res*a%n);
    }
    return res;
}
long modpow(long a,long m,long n){
    long double b;
    if(m==1){
        return a%n;
    }
    else{
        if(m%2==0){
            //b=fmod(powl(a,(m/2)),n);
            b = modpow(a, m/2, n);
            return fmod(b*b, n);
            //return modpow(fmod(b * b, n),m-1,n);
        }else{
            //b=fmod(powl(a,floor(m/2)),n);
            //return modpow(fmod(a* b * b, n),m-1,n);
            b = modpow(a, (int)(m/2), n);
            return fmod(a*b*b, n);
        }
    }
}

/*Calcul du plus grand nombre premier trouvable en moins de s secondes avec is_prime_naive en partant de l'entier impair p*/
void plus_grand_premier_is_prime_naive_en_temps_donne(long p, double s){
	long dernier_premier;
	double dernier_premier_CPU;
    int res=0;
    
	temps_cpu1 = 0;
    while (temps_cpu1 < s){
		temps_initial = clock () ; //Nombre de ‘‘ticks’’ consomm’es par le programme jusqu’ici
		res = is_prime_naive(p);
		temps_final = clock () ; //Nombre de ‘‘ticks’’ consomm’es par le programme jusqu’ici
		//On convertit les ‘‘ticks’’ consomm’es par fct en secondes :
		temps_cpu1 = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
        printf("p=%ld, premier=%d, temps CPU=%f\n", p, res, temps_cpu1);
		if (res == 1 && (temps_cpu1 < s)){
			dernier_premier = p; //sauvegarde le dernier nombre premier trouve
            dernier_premier_CPU = temps_cpu1;
        }
		p+=2;
        res = 0;
    }
	printf("Dernier nombre premier trouvé=%ld, temps CPU=%f\n", dernier_premier, dernier_premier_CPU);
}

int main(){
    // int max =10000;
    // int res=0;
    // int res2=0;

    plus_grand_premier_is_prime_naive_en_temps_donne(3, 0.002);

    // res=modpow_naive(142,7,187);
    // printf("%d\n",res);
    // res2=modpow(142,7,187);
    // printf("%d\n",res2);

}