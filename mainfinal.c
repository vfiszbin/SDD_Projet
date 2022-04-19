#include "p2exo3.h"
#include "p2exo4.h"
#include "p4exo7.h"
#include "p4exo8.h"
#include "p4exo9.h"

#define NB_CITOYENS 1000
#define NB_CANDIDATS 5
#define NB_ASSESSEURS 20
#define D 1 //le nombre de zeros par lequel la valeur hachee d'un bloc doit demarrer
#define NB_VOTES_PAR_BLOC 10 
#define SIZE_C 10 //taille table de hachage des candidats
#define SIZE_V 2000 //taille table de hachage des votants


/*Selectione les nb premiers citoyens de la liste citoyens pour officier en tant qu'assesseurs.
Retourne un tableau contenant les cles publiques des assesseurs*/
Key ** tab_assesseurs(CellKey *citoyens, int nb){
    Key **assesseurs = (Key**)malloc(sizeof(Key*) * nb);
    if (!assesseurs)
        return NULL;

    int i = 0;
    CellKey * citoyen = citoyens;
    while(citoyen && i < nb){
        assesseurs[i] = citoyen->data;
        citoyen = citoyen->next;
        i++;
    }
    return assesseurs;
}

/*Choisit aléatoirement un assesseur parmi ceux disponibles*/
Key *random_assesseur(Key **assesseurs, int len){
    int random = rand() % len;
    return assesseurs[random];
}


int main(){
    srand(time(NULL));
    //Genere citoyens et candidats et ecrit leurs cles dans keys.txt et candidates.txt
    //Chaque citoyens vote pour un candidat, les declarations de votes sont ecrites dans declarations.txt
    if(generate_random_data(NB_CITOYENS, NB_CANDIDATS) == 0)
        return 1;

    //Lit keys.txt pour obtenir une liste chainee des cles des citoyens
    CellKey *citoyens = read_public_keys("keys.txt");
    if (!citoyens)
        return 1;
    // printf("\nListe chaînée des clés des citoyens :\n");
    // print_list_keys(citoyens);

    //Lit keys.txt pour obtenir une liste chainee des cles des candidats
    CellKey *candidats = read_public_keys("candidates.txt");
    if (!candidats){
		delete_list_keys(citoyens);
        return 1;
    }
    // printf("\nListe chaînée des clés des candidats :\n");
    // print_list_keys(candidats);

    //Lit declarations.txt pour constituer la liste chainees des declarations de vote
    CellProtected *votes = read_protected("declarations.txt");
    if (!votes){
        delete_list_keys(citoyens);
        delete_list_keys(candidats);
        return 1;
    }
    // printf("\nListe chaînée des déclarations de vote :\n");
    // print_list_cell_protected(votes);

    //Selectionne des assesseurs parmi les citoyens
    Key **assesseurs = tab_assesseurs(citoyens, NB_ASSESSEURS);
    if (!assesseurs){
        delete_list_keys(citoyens);
        delete_list_keys(candidats);
        delete_list_cell(votes);
        return 1;
    }

    //Soumissions des votes
    int cpt = 0;
    int nb_blocks = 1;
    char *nb_blocks_str;
    char *blockname;
    CellTree* tree = NULL;
    CellProtected *vote = votes;
    while(vote){
        while(cpt < NB_VOTES_PAR_BLOC && vote){
            submit_vote(vote->data);
            cpt++;
            vote = vote->next;
        }
        //Tous les 10 votes soumis, un bloc contenant ces votes est créé
        if (create_block(&tree, random_assesseur(assesseurs, NB_ASSESSEURS), D) == 0){
            delete_list_keys(citoyens);
            delete_list_keys(candidats);
            delete_list_cell(votes);
            free(assesseurs);
            full_delete_tree(tree);
            return 1;
        }

        nb_blocks_str = int_to_str(nb_blocks);
        if (!nb_blocks_str){
            delete_list_keys(citoyens);
            delete_list_keys(candidats);
            delete_list_cell(votes);
            free(assesseurs);
            full_delete_tree(tree);
            return 1;
        }
        blockname = strjoin("block", nb_blocks_str, 0);
        if (!blockname){
            delete_list_keys(citoyens);
            delete_list_keys(candidats);
            delete_list_cell(votes);
            free(assesseurs);
            free(nb_blocks_str);
            full_delete_tree(tree);
            return 1;
        }
        free(nb_blocks_str);
        
        if (add_block(D, blockname) == 0){
            delete_list_keys(citoyens);
            delete_list_keys(candidats);
            delete_list_cell(votes);
            free(assesseurs);
            free(blockname);
            full_delete_tree(tree);
            return 1;
        }
        free(blockname);
        cpt=0;
        nb_blocks++;
        
    }
    free(assesseurs);
    full_delete_tree(tree);
    
    //Lecture du repertoire Blockchain et construction de l'arbre des blocs de declarations de votes
    CellTree *arbre_blocs = read_tree();
    if (!arbre_blocs){
            delete_list_keys(citoyens);
            delete_list_keys(candidats);
            delete_list_cell(votes);
            return 1;
    }

    //print_tree2D(arbre_blocs, 0);

    //Calcul du vainqueur de l'election en comptabilisant les votes de la plus longue branche de l'arbre de blocs
    int nb_votes_vainqueur;
    int nb_votes;
    Key* gagnant = compute_winner_BT(arbre_blocs, candidats, citoyens, SIZE_C, SIZE_V, &nb_votes_vainqueur, &nb_votes);
    if (!gagnant){
        delete_list_keys(citoyens);
        delete_list_keys(candidats);
        delete_list_cell(votes);
        full_delete_tree(arbre_blocs);
        return 1;
    }
 
    printf("Le candidat (%lx,%lx) remporte l'élection avec %d voix sur %d, soit un score de %.02f%% !\n",
    gagnant->val, gagnant->n, nb_votes_vainqueur, nb_votes, ((double)nb_votes_vainqueur/nb_votes)*100);

    //Libere la memoire
    delete_list_keys(citoyens);
    delete_list_keys(candidats);
    delete_list_cell(votes);
    full_delete_tree(arbre_blocs);
}