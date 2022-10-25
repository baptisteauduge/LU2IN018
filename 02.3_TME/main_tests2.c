#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"

#define NBR_PROIE_PRED 20
#define MAX_ENERGY 100

void generate_rand_x_y_energy(int *x, int *y, int *energy) {
  *x = rand() % SIZE_X;
  *y = rand() % SIZE_Y;
  *energy = rand() % MAX_ENERGY;
}

void add_proie_pred (Animal **liste_proie, Animal **liste_pred) {
  int x = 0;
  int y = 0;
  int energy = 0;

  for (int i = 0; i < NBR_PROIE_PRED; ++i) {
    generate_rand_x_y_energy(&x, &y, &energy);
    ajouter_animal(x, y, energy, liste_proie);
    generate_rand_x_y_energy(&x, &y, &energy);
    ajouter_animal(x, y, energy, liste_pred);
  }
}

void check_size_proie_pred(Animal *liste_proie, Animal *liste_pred) {
  assert(compte_animal_it(liste_proie) == NBR_PROIE_PRED);
  assert(compte_animal_it(liste_pred) == NBR_PROIE_PRED);
}

/**
 * USED TO TEST ENLEVE ANIMAL
 * We will free element at the begging of liste proie, at the end of liste pred
 * and in the middle of liste pred
 */
void find_del_animal_in_list(Animal **liste_proie, Animal **liste_pred) {
  Animal *last_pred = NULL;
  Animal *middle_pred = NULL;
  Animal *first_proie = *liste_proie;
  Animal *iter_liste_pred = *liste_pred;
  int nbr_pred = compte_animal_it(*liste_pred);

  for (int i = 0; i < nbr_pred && iter_liste_pred; ++i) {
    if (i == nbr_pred / 2)
      middle_pred = iter_liste_pred;
    
    if (!iter_liste_pred->suivant)
      last_pred = iter_liste_pred;
    iter_liste_pred = iter_liste_pred->suivant;
  }
  enlever_animal(liste_proie, first_proie);
  enlever_animal(liste_pred, middle_pred);
  enlever_animal(liste_pred, last_pred);
}

int main(void) {
  Animal *liste_proie = NULL;
  Animal *liste_predateurs = NULL;
  srand(time(NULL));

  add_proie_pred(&liste_proie, &liste_predateurs);
  check_size_proie_pred(liste_proie, liste_predateurs);
  afficher_ecosys(liste_predateurs, liste_proie);
  ecrire_ecosys("test_ecosys.conf", liste_predateurs, liste_proie);
  liberer_liste_animaux(liste_proie);
  liberer_liste_animaux(liste_predateurs);
  liste_proie = NULL;
  liste_predateurs = NULL;

  lire_ecosys("test_ecosys.conf", &liste_predateurs, &liste_proie);
  afficher_ecosys(liste_predateurs, liste_proie);
  find_del_animal_in_list(&liste_proie, &liste_predateurs);
  afficher_ecosys(liste_predateurs, liste_proie);
  liberer_liste_animaux(liste_proie);
  liberer_liste_animaux(liste_predateurs);
  return EXIT_SUCCESS;
}
