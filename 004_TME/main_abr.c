#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "liste.h"
#include "abr.h"


unsigned long search_n_times_and_count(int n, char *searched) {
  Lm_mot *liste_mots = lire_dico_Lmot(DICT_FILE_NAME);
  Nd_mot *abr_mots = Lm2abr(liste_mots);
  struct timeval stop, start;

  gettimeofday(&start, NULL);
  for (int i = 0; i < n; ++i) {
    chercher_Nd_mot(abr_mots, searched);
  }
  gettimeofday(&stop, NULL);

  detruire_abr_mot(abr_mots);
  return (unsigned long) (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage : ./main_abr <search> <nbr_iter>\n");
    return EXIT_FAILURE;
  }
  int nbr_searches = atoi(argv[2]);

  unsigned long time_elapsed = search_n_times_and_count(nbr_searches, argv[1]);
  printf("%d, %lu\n", nbr_searches, time_elapsed);

  return 0;
}
