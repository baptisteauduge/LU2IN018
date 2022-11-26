#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_ITER 100000000
#define MAX_LEN_ITER_STR 20
#define STEP *2

void run_abr(char *search) {
  int file = open("abr_exit", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  char nbrIter[MAX_LEN_ITER_STR];
  for (unsigned long i = 1; i < MAX_ITER; i = i STEP) {
    sprintf(nbrIter, "%lu", i);
    if (fork() == 0) {
      dup2(file, 1);
      if (execl("./main_abr", "./main_abr", search, nbrIter, NULL) == -1) {
        printf("Error execl\n");
      }
    }
  }
  close(file);
}

void run_arbre(char *search) {
  int file = open("arbre_exit", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  char nbrIter[MAX_LEN_ITER_STR];

  for (unsigned long i = 1; i < MAX_ITER; i = i STEP) {
    dup2(file, 1);
    sprintf(nbrIter, "%lu", i);
    if (fork() == 0) {
      if (execl("./main_arbre", "./main_arbre", search, nbrIter, NULL) == -1) {
        printf("Error execl\n");
      }
    }
  }
  close(file);
}

int main (int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./ex5 <searched>");
  }
  run_abr(argv[1]);
  run_arbre(argv[1]);

  for(unsigned long i = 1; i < MAX_ITER; i = i STEP)
    wait(NULL);

  return EXIT_SUCCESS;
}