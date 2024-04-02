CC = gcc 
FLAG = -Wall -Wextra -Werror -std=c11

all: s21_cat

rebuild: clean s21_cat

s21_cat:
	$(CC) $(FLAG) cat.c -o s21_cat

clean:
	rm -rf *.o s21_cat