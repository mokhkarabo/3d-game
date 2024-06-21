CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -Iinc -lSDL2 -lm
SRC = src/main.c src/raycasting.c src/utils.c
OBJ = $(SRC:.c=.o)
EXEC = 3d-maze-game

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean

