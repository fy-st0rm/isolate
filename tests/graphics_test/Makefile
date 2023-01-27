CC = gcc
INC = -I../../includes/
SRC = *.c
OBJ = *.o
LIB = -lSDL2main -lSDL2

window:
	$(CC) $(INC) -c window_test.c
	$(CC) -o out $(OBJ) $(LIB)
	rm $(OBJ)
	./out
