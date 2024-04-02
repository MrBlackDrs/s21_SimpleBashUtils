#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readFile(char *filename);
void manager(char c, int *flags, int *line, int *enter_in_row, char next);
void cat(char *filename, int *flags);
void from_char_to_flags(int n, int *flags, char *argv[]);
void find_files(int n, int *files_index, char *argv[]);

int main(int argc, char *argv[]) {
  if (argc == 2)
    readFile(argv[1]);
  else if (argc > 2) {
    int flags[6];
    int files[6];
    for (int i = 0; i < 6; i++) flags[i] = 0;
    from_char_to_flags(argc, flags, argv);
    find_files(argc, files, argv);
    for (int i = 0; i < argc; i++) {
      if (files[i]) cat(argv[i], flags);
    }
  }

  return 0;
}

void manager(char c, int *flags, int *line, int *enter_in_row, char next) {
  if (c == '\n') {
    (*enter_in_row)++;
    if (((*enter_in_row < 2 || next != '\n') && flags[2]) || !(flags[2])) {
      if (flags[0]) {
        if (flags[4]) printf("$");
        printf("\n%6d\t", ++(*line));
      } else if (flags[1]) {
        if (flags[4]) printf("$");
        printf("\n");
        if (next != '\n') printf("%6d\t", ++(*line));
      } else {
        if (flags[4]) printf("$");
        printf("\n");
      }
    }
  } else if (c == '\t' && flags[5]) {
    printf("^I");
    *enter_in_row = 0;
  }

  else if ((c < (char)32 || c == (char)127) && flags[3] && c != '\t') {
    printf("^");
    if (c < 32) printf("%c", c + 64);
    if (c == 127) printf("%c", c - 64);
    *enter_in_row = 0;
  } else {
    *enter_in_row = 0;
    printf("%c", c);
  }
}

void cat(char *filename, int *flags) {
  FILE *f = fopen(filename, "r");
  if (f != NULL) {
    char c = getc(f);
    if (c != -1) {
      if (flags[0] || flags[1]) printf("%6d\t", 1);
    } else
      printf("empty file");
    int line = 1, enter_in_row = 0;
    while (c != -1) {
      char next = getc(f);
      manager(c, flags, &line, &enter_in_row, next);
      ungetc(next, f);
      c = getc(f);
    }
    fclose(f);
  } else
    printf("cat: %s: No such file or directory", filename);
}

void from_char_to_flags(int n, int *flags, char *argv[]) {
  for (int i = 1; i < n; i++) {  // to n
    if (argv[i][0] != '-') continue;
    if (strcmp(argv[i], "--number") == 0) {
      flags[0] = 1;
      continue;
    }
    if (strcmp(argv[i], "--number-nonblank") == 0) {
      flags[1] = 1;
      continue;
    }
    if (strcmp(argv[i], "--squeeze-blank") == 0) {
      flags[2] = 1;
      continue;
    }
    if (strcmp(argv[i], "-E") == 0) flags[4] = 1;
    if (strcmp(argv[i], "-T") == 0) flags[5] = 1;
    for (size_t j = 1; j < strlen(argv[i]); j++) {
      if (argv[i][j] == 'n') flags[0] = 1;
      if (argv[i][j] == 'b') flags[1] = 1;
      if (argv[i][j] == 's') flags[2] = 1;
      if (argv[i][j] == 'e') {
        flags[3] = 1;
        flags[4] = 1;
      }
      if (argv[i][j] == 't') {
        flags[3] = 1;
        flags[5] = 1;
      }
    }
  }
  if (flags[0] && flags[1]) flags[0] = 0;
}

void find_files(int n, int *files_index, char *argv[]) {
  files_index[0] = 0;
  for (int i = 1; i < n; i++) {
    FILE *f = fopen(argv[i], "r");
    if (f != NULL) {
      files_index[i] = 1;
      fclose(f);
    } else {
      files_index[i] = 0;
      if (argv[i][0] != '-')
        printf("cat: %s: No such file or directory\n", argv[i]);
    }
  }
}

void readFile(char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL)
    printf("file does not exist");
  else {
    char c = getc(f);
    if (c == -1) printf("empty file");
    while (c != -1) {
      printf("%c", c);
      c = getc(f);
    }
    fclose(f);
  }
}