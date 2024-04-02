#include "grep.h"

#include <regex.h>
#include <string.h>

void collect(int n, char *argv[], flags *flags, int *files_index, char *pattern,
             FILE *pat, char *f_file);
FILE *search_print(char *pattern, flags flags, FILE *f, int number,
                   int *num_matched_str, char *filename);
int flag_o(char *str, char *pattern, regex_t *regex);
void flag_f_e(char *filename, char *pattern);

int main(int argc, char *argv[]) {
  flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int files_index[argc];
  char pattern[10000] = "";
  char f_file[9000] = "";
  FILE *e_file = fopen("e_file.txt", "w");
  collect(argc, argv, &flags, files_index, pattern, e_file, f_file);
  fclose(e_file);

  if (flags.e) {
    flag_f_e("e_file.txt", pattern);
  } else if (flags.f) {
    flag_f_e(f_file, pattern);
  }
  remove("e_file.txt");

  for (int i = 1; i < argc; i++) {
    if (files_index[i]) {
      FILE *f = fopen(argv[i], "r");
      char c = getc(f);
      if (c == '\0') {
        fclose(f);
        continue;
      }
      ungetc(c, f);
      int number = 1, num_matched_str = 0;
      while (!feof(f)) {
        f = search_print(pattern, flags, f, number, &num_matched_str, argv[i]);
        number++;
      }
      if (num_matched_str) {
        if (flags.c) {
          if (!flags.h && flags.number_of_files > 1) printf("%s:", argv[i]);
          printf("%d\n", num_matched_str - (num_matched_str - 1) * flags.l);
        }
        if (flags.l) printf("%s\n", argv[i]);
      } else if (flags.c)
        printf("0\n");
      fclose(f);
    }
  }
  return 0;
}

void collect(int n, char *argv[], flags *flags, int *files_index, char *pattern,
             FILE *pat, char *f_file) {
  files_index[0] = 0;
  int pattern_found = 0, jump = 0;
  for (int i = 1; i < n; i++) {
    files_index[i] = 0;
    if (argv[i][0] == '-') {
      for (size_t j = 1; j < strlen(argv[i]); j++) {
        char flag = argv[i][j];
        if (flag == 'e') {
          flags->e++;
          j++;
          if (flags->e > 1) fprintf(pat, "\n");
          if (j < strlen(argv[i])) {
            for (; j < strlen(argv[i]); j++) fprintf(pat, "%c", argv[i][j]);
          } else {
            if (i + 1 < n) {
              fprintf(pat, "%s", argv[i + 1]);
              jump = 1;
            }
          }
        } else if (flag == 'i')
          flags->i = 1;
        else if (flag == 'v')
          flags->v = 1;
        else if (flag == 'c')
          flags->c = 1;
        else if (flag == 'l')
          flags->l = 1;
        else if (flag == 'n')
          flags->n = 1;
        else if (flag == 'h')
          flags->h = 1;
        else if (flag == 's')
          flags->s = 1;
        else if (flag == 'f') {
          flags->f = 1;
          j++;
          if (j < strlen(argv[i])) {
            for (; j < strlen(argv[i]); j++) sprintf(f_file, "%c", argv[i][j]);
          } else {
            if (i + 1 < n) {
              sprintf(f_file, "%s", argv[i + 1]);
              jump = 1;
            }
          }
          FILE *buf = fopen(f_file, "r");
          if (buf == NULL) {
            printf("grep: %s: No such file or directory\n", f_file);
            fclose(buf);
            exit(1);
          }
          fclose(buf);
        } else if (flag == 'o')
          flags->o = 1;
      }
    } else {
      if (jump) {
        jump = 0;
        continue;
      }
      if (!pattern_found && !flags->e && !flags->f) {
        pattern_found = 1;
        strcpy(pattern, argv[i]);
      } else {
        FILE *f = fopen(argv[i], "r");
        if (f != NULL) {
          flags->number_of_files++;
          files_index[i] = 1;
          fclose(f);
        } else if (!flags->s)
          printf("grep: %s: No such file or directory\n", argv[i]);
      }
    }
  }
}

FILE *search_print(char *pattern, flags flags, FILE *f, int number,
                   int *num_matched_str, char *filename) {
  regex_t regex;
  int reg_flag = 0;
  if (flags.i) reg_flag = REG_ICASE;
  if (flags.e || flags.f) reg_flag |= REG_EXTENDED;
  regcomp(&regex, pattern, reg_flag);
  char buf[9000] = "";
  char c = getc(f);
  int i = 0;
  while (c != '\n' && c != EOF) {
    buf[i] = c;
    c = getc(f);
    i++;
  }
  int status = regexec(&regex, buf, 0, NULL, 0);
  if (flags.v) status = !status;
  if (!status) {
    if (flags.c || flags.l) {
      (*num_matched_str)++;
    } else {
      if (flags.number_of_files > 1 && !flags.h) printf("%s:", filename);
      if (flags.n) printf("%d:", number);
      if (flags.o) {
        int count = flag_o(buf, pattern, &regex);
        for (int j = 0; j < count; j++) printf("%s\n", pattern);
      } else {
        printf("%s\n", buf);
      }
    }
  }

  regfree(&regex);
  return f;
}

int flag_o(char *str, char *pattern, regex_t *regex) {
  size_t shift = strlen(pattern), size = strlen(str);
  int count = 0;
  size_t i = 0;
  while (i + shift <= size) {
    char buf[9000] = "";
    for (size_t j = 0; j < shift; j++) buf[j] = str[i + j];
    if (!regexec(regex, buf, 0, NULL, 0)) count++;
    i++;
  }
  return count;
}

void flag_f_e(char *filename, char *pattern) {
  FILE *file = fopen(filename, "r");
  char buffer[9000] = {0};
  int count = 0;
  while (fgets(buffer, 9000, file) != NULL) {
    if (buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = 0;
    if (count > 0) strcat(pattern, "|");
    if (*buffer == '\0') {
      strcat(pattern, ".");
    } else {
      strcat(pattern, buffer);
    }
    ++count;
  }
  fclose(file);
}