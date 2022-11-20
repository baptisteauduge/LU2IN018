#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 1000000
#define T_WAIT_TEST 500000
#define T_WAIT_MESSAGES 2000000

void test_deplacements() {
  Animal *liste_pred = NULL;
  Animal *liste_proie = NULL;
  int dir[] = {1, 1};

  ajouter_animal_with_dir(10, 10, 500, dir, &liste_proie);
  afficher_ecosys(liste_pred, liste_proie);
  printf("[Test] Toricité du monde\n");
  usleep(T_WAIT_MESSAGES);

  for (int i = 0; i < 30; ++i) {
    clear_screen();
    bouger_animaux(liste_proie);
    afficher_ecosys(liste_pred, liste_proie);
    usleep(T_WAIT_TEST);
  }

  liste_proie = liberer_liste_animaux(liste_proie);
  liste_pred = liberer_liste_animaux(liste_pred);
}

void test_reproduction() {
  Animal *liste_pred = NULL;
  int last_nbr_pred = 0;

  ajouter_animal(10, 10, 500, &liste_pred);
  ++last_nbr_pred;
  printf("[Test] Reproduction des animaux :\n");
  usleep(T_WAIT_MESSAGES);

  for (int i = 0; i < 20; ++i) {
    reproduce(&liste_pred, 1);
    last_nbr_pred *= 2;
    assert(compte_animal_it(liste_pred) == last_nbr_pred);
  }
  printf("[Test] Reproduction passé avec succès.\n");
  usleep(T_WAIT_MESSAGES);
  liste_pred = liberer_liste_animaux(liste_pred);
}

void create_default_animals(Animal **liste_pred, Animal **liste_proie) {
  for (int i = 0; i < NB_PROIES; ++i){
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, (rand() % 50) + 50, liste_proie);
  }

  for (int i = 0; i < NB_PREDATEURS; ++i){
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, (rand() % 50) + 50, liste_pred);
  }
}

void run_ecosys(Animal **liste_proie, Animal **liste_pred, int monde[SIZE_X][SIZE_Y], FILE* file) {
  // Exercice 5, question 2 & Exercice 6, question 3
  for (int i = 0; i < 200 && *liste_proie && *liste_pred; ++i) {
    rafraichir_proies(liste_proie, monde);
    clear_screen();
    rafraichir_predateurs(liste_pred, liste_proie);
    // Exercice 9
    fprintf(file, "%d %d %d\n", i, compte_animal_it(*liste_proie), compte_animal_it(*liste_pred));
    afficher_ecosys(*liste_pred, *liste_proie);
    usleep(T_WAIT);
  }
}

int main(void) {
  Animal *liste_pred = NULL;
  Animal *liste_proie = NULL;
  int monde[SIZE_X][SIZE_Y];
  srand(time(NULL));
  // Exercice 8
  FILE *file_ecosys = open_and_clean_file_ecosys();
  // Exercie 7, question 1
  init_monde(monde, 0);
  // Exercice 4, question 1, 2
  test_deplacements();
  test_reproduction();
  // Exercice 5, question 2
  create_default_animals(&liste_pred, &liste_proie);
  // Main loop
  run_ecosys(&liste_proie, &liste_pred, monde, file_ecosys);
  fclose(file_ecosys);
  liste_pred = liberer_liste_animaux(liste_pred);
  liste_proie = liberer_liste_animaux(liste_proie);
  return EXIT_SUCCESS;
}
