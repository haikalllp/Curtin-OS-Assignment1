CC = gcc
CFLAGS = -Wall -Wextra -I include -std=c11
LDFLAGS = -pthread
SRC = src/main.c src/sort.c src/file_utils.c src/sync.c
OBJ = $(SRC:.c=.o)

sss: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) sss
