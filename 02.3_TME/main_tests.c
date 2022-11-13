#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"

void create_liste_proie(Animal **liste_proie) {
  int energie=10;
  Animal *a1 = creer_animal(0,10, energie);
  Animal *a2 = creer_animal(15,35, energie);
  Animal *a3 = creer_animal(2,42, energie);
  Animal *a4 = creer_animal(18,13, energie);
  
  a1->suivant = a2;
  a2->suivant = a3;
  a3->suivant = a4;
  *liste_proie = a1;
}

void create_liste_pred(Animal **liste_pred) {
  int energie=10;
  Animal *a5 = creer_animal(2,5, energie);
  Animal *a6 = creer_animal(15,35, energie);
  Animal *a7 = creer_animal(9,22, energie);
  Animal *a8 = creer_animal(17,3, energie);
  
  a5->suivant=a6;
  a6->suivant=a7;
  a7->suivant=a8;
  *liste_pred=a5;
}

void add_pred_proie (Animal **liste_pred, Animal **liste_proie) {
  ajouter_animal(5, 5, 10, liste_pred);
  ajouter_animal(6, 6, 10, liste_pred);
  ajouter_animal(6, 6, 40, liste_proie);
  ajouter_animal(19, 11, 6, liste_proie);
}

int main_partie_1(void) {
  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;
  srand(time(NULL));

  create_liste_proie(&liste_proie);
  create_liste_pred(&liste_predateur);
  add_pred_proie(&liste_predateur, &liste_proie);
  afficher_ecosys(liste_proie,liste_predateur);  
  return EXIT_SUCCESS;
}

int main(void) {
  srand(time(NULL));
  Animal *liste_pred = NULL;
  int init_nbr_pred = 400;
  Animal *liste_proie = NULL;
  int init_nbr_proie = 200;
  int monde[SIZE_X][SIZE_Y];
  FILE *file_ecosys = open_and_clean_file_ecosys();


  init_monde(monde, 0);

  for (int i = 0; i < init_nbr_proie; ++i){
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, (rand() % 50) + 50, &liste_proie);
  }

  for (int i = 0; i < init_nbr_pred; ++i){
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, (rand() % 50) + 50, &liste_pred);
  }

  for (int i = 0; i < 200 && liste_proie && liste_pred; ++i) {
    rafraichir_proies(&liste_proie, monde);
    rafraichir_predateurs(&liste_pred, &liste_proie);
    afficher_ecosys(liste_pred, liste_proie);
    fprintf(file_ecosys, "%d %d %d\n", i, compte_animal_it(liste_proie), compte_animal_it(liste_pred));
    usleep(500000);
    clear_screen();
  }
  afficher_ecosys(liste_pred, liste_proie);
  fclose(file_ecosys);
  liberer_liste_animaux(liste_pred);
  liberer_liste_animaux(liste_proie);

  return EXIT_SUCCESS;
}
