#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

/* PARTIE 1*/
/* Fourni: Part 1, exercice 3, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 3, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 5, question 1 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  assert(x < SIZE_X);
  assert(y < SIZE_Y);

  Animal *na = creer_animal(x, y, energie);
  na->suivant = *liste_animal;
  *liste_animal = na;
}

void ajouter_animal_with_dir(int x, int y, float energie, int* dir, Animal **liste_animal) {
  assert(x < SIZE_X);
  assert(y < SIZE_Y);
  Animal *na = (Animal *)malloc(sizeof(Animal));
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = dir[0];
  na->dir[1] = dir[1];
  na->suivant = *liste_animal;
  *liste_animal = na;
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
  Animal *prevAnimal = NULL;
  Animal *iterAnimal = *liste;

  while (iterAnimal) {
    if (iterAnimal == animal) {
      if (!prevAnimal) {
        *liste = (*liste)->suivant;
        free(animal);
        return;
      }
      prevAnimal->suivant = iterAnimal->suivant;
      free(iterAnimal);
      iterAnimal = prevAnimal->suivant;
    } else {
      prevAnimal = iterAnimal;
      iterAnimal = iterAnimal->suivant;
    }
  }
}

/* A Faire. Part 1, exercice 5, question 2 */
Animal* liberer_liste_animaux(Animal *liste) {
  Animal *tmp = NULL;

  while (liste) {
    tmp = liste->suivant;
    free(liste);
    liste = tmp;
  }
  return NULL;
}

/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}

/* Part 1. Exercice 4, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_predateur, Animal *liste_proie) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_X; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_Y; ++i) {
    printf("|");
    for (j = 0; j < SIZE_X; ++j) {
      putchar(ecosys[j][i]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_X; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);

  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

/*
* Balise is the name of the balise around animals passed in liste_animals
*/
void ecrire_animal(FILE *file, Animal *liste_anim, char *balise) {
  fprintf(file, "<%s>\n", balise);
  while (liste_anim) {
    fprintf(file, "x=%d y=%d dir=[%d %d] e=%.6f\n",
    liste_anim->x, liste_anim->y, liste_anim->dir[0], liste_anim->dir[1], liste_anim->energie);
    liste_anim = liste_anim->suivant;
  }
  fprintf(file, "</%s>\n", balise);
}


void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie) {
  FILE *f_eco_sys = fopen(nom_fichier, "w");
  assert(f_eco_sys);
  ecrire_animal(f_eco_sys, liste_proie, "proies");
  ecrire_animal(f_eco_sys, liste_proie, "predateurs");
  fclose(f_eco_sys);
}

void set_open_close_balise (char *balise, char *open_balise, char *close_balise) {
  assert(strlen(balise) < 250);

  strcat(open_balise, "<");
  strcat(close_balise, "</");
  strcat(open_balise, balise);
  strcat(close_balise, balise);
  strcat(open_balise, ">\n");
  strcat(close_balise, ">\n");
}

void lire_animal(FILE *file, Animal **liste_anim, char *balise) {
  char open_balise[MAX_LINE_SIZE] = "";
  char close_balise[MAX_LINE_SIZE] = "";
  char buffer[MAX_LINE_SIZE];
  int x = 0, y = 0, dir[2] = {0, 0};
  float e = 0;

  set_open_close_balise(balise, open_balise, close_balise);
  fgets(buffer, MAX_LINE_SIZE, file);
  while(strcmp(buffer, open_balise) != 0)
    fgets(buffer, strlen(open_balise) + 1, file);
  while(fscanf(file, "x=%d y=%d dir=[%d %d] e=%f\n", &x, &y, dir, dir + 1, &e) == 5)
    ajouter_animal_with_dir(x, y, e, dir, liste_anim);
  fgets(buffer, MAX_LINE_SIZE, file);
  if (strcmp(buffer, close_balise) != 0) {
    printf("Erreur de lecture du fichier, balise %s non fermée\n", balise);
    exit(EXIT_FAILURE);
  }
  rewind(file);
}

void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie) {
  FILE *f_eco_sys = fopen(nom_fichier, "r");
  assert(f_eco_sys);
  lire_animal(f_eco_sys, liste_proie, "proies");
  lire_animal(f_eco_sys, liste_predateur, "predateurs");
}

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
    /*A Completer*/

}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
   /*A Completer*/

}


/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    /*A Completer*/

}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
    /*A Completer*/

  return NULL;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
   /*A Completer*/

}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){

   /*A Completer*/
}
