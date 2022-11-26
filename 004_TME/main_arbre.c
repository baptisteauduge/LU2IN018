#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "arbre_lexicographique.h"

unsigned long search_n_times_and_count(unsigned long n, char *searched) {
  PNoeud dico = lire_dico("french_za");
  struct timeval stop, start;
  gettimeofday(&start, NULL);


  for (unsigned long i = 0; i < n; ++i) {
    rechercher_mot(dico, searched);
  }

  gettimeofday(&stop, NULL);
  detruire_dico(dico);
  return (unsigned long) (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage : ./main_arbre <search> <nbr_iter>\n");
    return EXIT_FAILURE;
  }
  int nbr_searches = atoi(argv[2]);

  unsigned long time_elapsed = search_n_times_and_count(nbr_searches, argv[1]);
  printf("%d, %lu\n", nbr_searches, time_elapsed);
  return 0;
}
