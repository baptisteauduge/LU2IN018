#ifndef _ECOSYS_H_
#define _ECOSYS_H_

#define SIZE_X 20
#define SIZE_Y 20
#define MAX_LINE_SIZE 255
#define FILE_NAME_ITER_ECOSYS "ecosys_iter.log"

extern float p_ch_dir;
extern float p_reproduce_proie;
extern float p_reproduce_predateur;
extern int temps_repousse_herbe;

typedef struct _animal {
  int x;
  int y;
  int dir[2]; /* direction courante sous la forme (dx, dy) */
  float energie;
  struct _animal *suivant;
} Animal;



Animal *creer_animal(int x, int y, float energie);
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal);
unsigned int compte_animal_rec(Animal *la);
unsigned int compte_animal_it(Animal *la);

void ajouter_animal(int x, int y, float energie, Animal **liste_animal);
void ajouter_animal_with_dir(int x, int y, float energie, int* dir, Animal **liste_animal);
void enlever_animal(Animal **liste, Animal *animal);
Animal* liberer_liste_animaux(Animal *liste);
void afficher_ecosys(Animal *liste_predateur, Animal *liste_proie);

void ecrire_animal(FILE *file, Animal *liste_anim, char *balise);
void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie);
void set_open_close_balise (char *balise, char *open_balise, char *close_balise);
void lire_animal(FILE *file, Animal **liste_anim, char *balise);
void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie);
void bouger_animal(Animal *la);
void bouger_animaux(Animal *la);
void reproduce(Animal **liste_animal, float p_reproduce);
Animal *animal_en_XY(Animal *l, int x, int y);
void rafraichir_predateurs(Animal **liste_pred, Animal **liste_proie);
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) ;
void manger_proies(Animal **liste_predateur, Animal **liste_proie);
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]);
void clear_screen();
void init_monde(int monde[SIZE_X][SIZE_Y], int value);
FILE *open_and_clean_file_ecosys();

#endif
