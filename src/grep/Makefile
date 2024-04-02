CC = gcc 
FLAG = -Wall -Wextra -Werror -std=c11

all: s21_grep

rebuild: clean s21_grep

s21_grep:
	$(CC) $(FLAG) grep.c -o s21_grep

clean:
	rm -rf *.o s21_grep