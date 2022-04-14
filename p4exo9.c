#include "p4exo9.h"


void submit_vote(Protected* pr){
    if(!pr){
        return;
    }
    FILE * f=fopen("Pending.txt","w");
    if(!f){
        printf("probleme d'ouverture du fichier pending.txt");
        return;
    }
    char* c = str_to_protected(pr);
    fprintf("c",pr);
    free(c);
    fclose(f);
}

