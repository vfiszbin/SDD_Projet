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
    for (int i=0; i<m; i++){
        res = (res * a % n);
    }
    return res;
}
long modpow(long a,long m,long n){
    long double b;
    if(m==1){
        return a%n;
    }
    else{
        if(m%2==0){ //si m est pair
            b = modpow(a, m/2, n);
            return fmod(b*b, n);
        }else{ //si m est impair
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
    long res;
    long res2;
    int m;
    int m_max = 10000;
    long a = 2;
    long n = 2;

    // plus_grand_premier_is_prime_naive_en_temps_donne(3, 0.002);

    FILE * f = fopen("sortie_vitesse.txt", "w");
	if (f == NULL){
		printf("Erreur lors de l'ouverture de %s\n", "sortie_vitesse.txt");
		return 1;
	}

    for (m = 1; m <= m_max; m++){
		printf("Pour m = %d :\n", m);
		printf("Pour la fonction modpow_naive :\n");
		temps_initial = clock();
        res=modpow_naive(a, m, n);
		temps_final = clock();
		temps_cpu1 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
		printf("Valeur retournée par la foncton = %ld, temps_cpu = %f\n", res, temps_cpu1);

		printf("===============\n");
		printf("Pour la fonction modpow :\n");
		temps_initial = clock();
		res2=modpow(a, m, n);
		temps_final = clock();
		temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
		printf("Valeur retournée par la foncton = %ld, temps_cpu = %f\n", res2, temps_cpu2);
		printf("==============================================================\n");

		fprintf(f,"%d %f %f\n", m, temps_cpu1, temps_cpu2); //écriture des résultats dans le fd f
	}
	fclose(f);

}