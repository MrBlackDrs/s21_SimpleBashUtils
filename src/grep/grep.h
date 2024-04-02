#ifndef GREP
#define GREP
#endif

#include <stdio.h>
#include <stdlib.h>

typedef struct flags {
  int e, i, v, c, l, n, h, s, f, o;
  int number_of_files;
} flags;
