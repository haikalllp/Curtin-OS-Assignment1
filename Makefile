CC = gcc
CFLAGS = -ansi -pedantic -Wall -Werror -g -Wextra -I include -std=c89
LDFLAGS = -pthread
SRC = src/main.c src/sort.c src/fileHandling.c src/sync.c
OBJ = $(SRC:.c=.o)
# To use sample input file when running with Valgrind, file can be changed below:
INPUT = sample1.txt

sss: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) sss

valgrind: sss
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./sss $(INPUT)
