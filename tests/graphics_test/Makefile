CC = gcc
INC = -I../../includes/
OBJ = *.o
LIB = -lSDL2main -lSDL2 -lGL -lGLU -lGLEW

window:
	$(CC) $(INC) -c window_test.c
	$(CC) -o out $(OBJ) $(LIB)
	rm $(OBJ)
	./out

gl:
	$(CC) $(INC) -c opengl_tri_test.c
	$(CC) -o out $(OBJ) $(LIB)
	rm $(OBJ)
	./out
