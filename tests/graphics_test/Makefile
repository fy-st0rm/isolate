CC = gcc
INC = -I../../includes/
OBJ = *.o
LIB = -lSDL2main -lSDL2 -lSDL2_image -lGL -lGLU -lGLEW -lm

window:
	$(CC) $(INC) -c window_test/window_test.c
	$(CC) -o window_test/out $(OBJ) $(LIB)
	rm $(OBJ)
	./window_test/out

tri:
	$(CC) $(INC) -c tri_test/tri_test.c
	$(CC) -o tri_test/out $(OBJ) $(LIB)
	rm $(OBJ)
	./tri_test/out

tex:
	$(CC) $(INC) -c texture_test/texture_test.c
	$(CC) -o texture_test/out $(OBJ) $(LIB)
	rm $(OBJ)
	./texture_test/out

cam:
	$(CC) $(INC) -c cam_test/cam_test.c
	$(CC) -o cam_test/out $(OBJ) $(LIB)
	rm $(OBJ)
	./cam_test/out

