#include "p1exo1.h"

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

/*Retourne la valeur a^m mod n par la methode naive*/
long modpow_naive(long a,long m, long n){
    int res=1;
    for (int i=0; i<m; i++){
        res = (res * a % n);
    }
    return res;
}

/*Retourne la valeur a^m mod n*/
long modpow(long a,long m,long n){
    long b;
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

/*Teste si a est un temoin de Miller pour p, pour un entier a donne*/
int witness (long a, long b, long d, long p){
    long x = modpow(a, d, p);
    if (x == 1){
        return 0;
    }
    for(long i = 0; i < b; i++){ 
        if(x == p-1){
            return 0; 
        }
        x = modpow(x,2,p); 
    }
    return 1; 
}

/*Retourne un entier long genere aleatoirement entre low et up inclus*/
long rand_long(long low, long up){ 
    return rand() % (up - low + 1) + low;
}

/*Realise le test de Miller-Rabin en generant k valeurs
de a au hasard, et en testant si chaque valeur de a est un temoin de Miller pour p.
La fonction retourne 0 des qu’un temoin de Miller est trouve (p n’est pas premier), 
et retourne 1 si aucun temoin de Miller n’a ete trouve (p est probablement premier)*/
int is_prime_miller(long p, int k) { 
    if (p == 2) {
        return 1; 
    }
    if (!(p & 1) || p <= 1) { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1)){ //tant que d n’est pas impair
        d = d / 2;
        b = b + 1; 
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a;
    int i;
    for(i = 0; i < k; i++){
        a = rand_long(2, p-1);
        if (witness(a,b,d,p)){ 
            return 0;
        }
    }
    return 1; 
}

/*Retourne un nombre premier de taille comprise entre low size et up size, avec k le nombre de tests de Miller a realiser*/
long random_prime_number(int low_size, int up_size, int k){ 

    if (low_size <= 0 || up_size <= 0 || low_size >= 32 || up_size >= 32){ //un long est codé sur 32 bits et signé, valeur max = 2^(k-1) - 1
        printf("La taille en bits choisie est trop petite ou trop grande\n");
        return -1;
    }
    long low = 1 << (low_size - 1); // 2^(low_size - 1) entier minimum codé sur low_size bits
    long up = (1 << (up_size)) - 1; // 2^(up_size) - 1 entier maximum codé sur up_size bits

    int is_prime = 0;
    long p;
    while (is_prime != 1){
        p = rand_long(low, up);
        is_prime = is_prime_miller(p, k);
    }

    return p;
}


// int main(){
//     srand(time(NULL));

//     // long res;
//     // long res2;
//     // int m;
//     // int m_max = 10000;
//     // long a = 2;
//     // long n = 2;

//     // plus_grand_premier_is_prime_naive_en_temps_donne(3, 0.002);

//     long p = random_prime_number(1,31,10);
//     printf("Nombre premier généré aléatoirement : %ld\n", p);
//     printf("Teste si c'est bien un nombre premier avec is_prime_naive: %d\n", is_prime_naive(p));


//     // FILE * f = fopen("sortie_vitesse.txt", "w");
// 	// if (f == NULL){
// 	// 	printf("Erreur lors de l'ouverture de %s\n", "sortie_vitesse.txt");
// 	// 	return 1;
// 	// }

//     // for (m = 1; m <= m_max; m++){
// 	// 	printf("Pour m = %d :\n", m);
// 	// 	printf("Pour la fonction modpow_naive :\n");
// 	// 	temps_initial = clock();
//     //     res=modpow_naive(a, m, n);
// 	// 	temps_final = clock();
// 	// 	temps_cpu1 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
// 	// 	printf("Valeur retournée par la foncton = %ld, temps_cpu = %f\n", res, temps_cpu1);

// 	// 	printf("===============\n");
// 	// 	printf("Pour la fonction modpow :\n");
// 	// 	temps_initial = clock();
// 	// 	res2=modpow(a, m, n);
// 	// 	temps_final = clock();
// 	// 	temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
// 	// 	printf("Valeur retournée par la foncton = %ld, temps_cpu = %f\n", res2, temps_cpu2);
// 	// 	printf("==============================================================\n");

// 	// 	fprintf(f,"%d %f %f\n", m, temps_cpu1, temps_cpu2); //écriture des résultats dans le fd f
// 	// }
// 	// fclose(f);

// }