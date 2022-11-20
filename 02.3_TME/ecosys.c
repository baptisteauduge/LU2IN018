#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

float p_ch_dir=0.1;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.2;
int temps_repousse_herbe=-6;

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

Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

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
  assert(dir);
  assert(-1 <= dir[0] <= 1);
  assert(-1 <= dir[1] <= 1);
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
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

Animal* liberer_liste_animaux(Animal *liste) {
  Animal *tmp = NULL;

  while (liste) {
    tmp = liste->suivant;
    free(liste);
    liste = tmp;
  }
  return NULL;
}

unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}

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
  if (!f_eco_sys) {
    fprintf(stderr, "Erreur lors de l'ouverture en écriture du fichier %s !\n", nom_fichier);
    exit(EXIT_FAILURE);
  }
  ecrire_animal(f_eco_sys, liste_proie, "proies");
  ecrire_animal(f_eco_sys, liste_predateur, "predateurs");
  fclose(f_eco_sys);
}

void set_open_close_balise (char *balise, char *open_balise, char *close_balise) {
  assert(strlen(balise) < MAX_LINE_SIZE - 3);

  strcat(open_balise, "<");
  strcat(close_balise, "</");
  strcat(open_balise, balise);
  strcat(close_balise, balise);
  strcat(open_balise, ">\n");
  strcat(close_balise, ">\n");
}

void lire_animal(FILE *file, Animal **liste_anim, char *balise) {
  assert(file);
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
    if (!f_eco_sys) {
    fprintf(stderr, "Erreur lors de l'ouverture en lecture du fichier %s !\n", nom_fichier);
    exit(EXIT_FAILURE);
  }
  lire_animal(f_eco_sys, liste_proie, "proies");
  lire_animal(f_eco_sys, liste_predateur, "predateurs");
  fclose(f_eco_sys);
}

void bouger_animal(Animal *la) {
  assert(la);
  if (la->energie <= 0) return;
  int x, y;
  x = (la->x + la->dir[0]) % SIZE_X;
  while (x < 0)
    x += SIZE_X;
  y = (la->y + la->dir[1]) % SIZE_Y;
  while (y < 0)
    y += SIZE_Y;
  la->x = x;
  la->y = y;
  --(la->energie);
}

void bouger_animaux(Animal *la) {
  while (la) {
    bouger_animal(la);
    la = la->suivant;
  }
}

void reproduce(Animal **liste_animal, float p_reproduce) {
  assert(liste_animal);
  Animal *new_animals = NULL;
  Animal *iter_animals_prev = NULL;
  Animal *iter_animals = *liste_animal;

  while (iter_animals) {
    if (rand() / RAND_MAX < p_reproduce) {
      ajouter_animal(iter_animals->x, iter_animals->y, iter_animals->energie / 2, &new_animals);
      iter_animals->energie /= 2;
    }
    iter_animals_prev = iter_animals;
    iter_animals = iter_animals->suivant;
  }
  if (iter_animals_prev)
    iter_animals_prev->suivant = new_animals;
  else
    *liste_animal = new_animals;
}

void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
  assert(liste_proie);
  assert(monde);
  Animal *iter_proie = *liste_proie;
  Animal *temp = NULL;

  rafraichir_monde(monde);
  bouger_animaux(*liste_proie);
  while(iter_proie) {
    if (monde[iter_proie->x][iter_proie->y] > 0) {
      iter_proie->energie += monde[iter_proie->x][iter_proie->y];
      monde[iter_proie->x][iter_proie->y] = temps_repousse_herbe;
    }

    if (--(iter_proie->energie) < 0) {
      temp = iter_proie;
      iter_proie = iter_proie->suivant;
      enlever_animal(liste_proie, temp);
      continue;
    }
    iter_proie = iter_proie->suivant;
  }
  reproduce(liste_proie, p_reproduce_proie);
}

Animal *animal_en_XY(Animal *l, int x, int y) {
  while (l) {
    if (l->x == x && l->y == y)
      return l;
    l = l->suivant;
  }

  return NULL;
} 

void manger_proies(Animal **liste_predateur, Animal **liste_proie) {
  Animal *predateur = NULL;
  Animal *proie = NULL;
  float energie_proie = 0;

  for (int x = 0; x < SIZE_X; ++x) {
    for (int y = 0; y < SIZE_Y; ++y) {
      if (
        (predateur = animal_en_XY(*liste_predateur, x, y)) 
        && (proie = animal_en_XY(*liste_proie, x, y))
      ) {
        energie_proie = proie->energie;
        predateur->energie += energie_proie;
        enlever_animal(liste_proie, proie);
      }
    }
  }
}

void rafraichir_predateurs(Animal **liste_pred, Animal **liste_proie) {
  assert(liste_pred);
  assert(liste_proie);
  Animal *iter_pred = *liste_pred;
  Animal *temp = NULL;

  bouger_animaux(*liste_pred);
  manger_proies(liste_pred, liste_proie);
  while(iter_pred) {
    if (--(iter_pred->energie) < 0) {
      temp = iter_pred;
      iter_pred = iter_pred->suivant;
      enlever_animal(liste_pred, temp);
      continue;
    }
    iter_pred = iter_pred->suivant;
  }
  reproduce(liste_pred, p_reproduce_predateur);
}

void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
  assert(monde);
  for (int i = 0; i < SIZE_X; ++i) {
    for (int j = 0; j < SIZE_Y; ++j) {
      ++(monde[i][j]);
    }
  }
}

void init_monde(int monde[SIZE_X][SIZE_Y], int value) {
  assert(monde);
  for (int i = 0; i < SIZE_X; ++i)
    for (int j = 0; j < SIZE_Y; ++j)
      monde[i][j] = value;
}

FILE *open_and_clean_file_ecosys(){
  FILE *file = fopen(FILE_NAME_ITER_ECOSYS, "w");
  if (!file) {
    fprintf(stderr, "Error: connot open file %s", FILE_NAME_ITER_ECOSYS);
    exit(EXIT_FAILURE);
  }

  return file;
}
