target main:
	gcc main.c -lSDL2 -lm

target mac:
	clang -o ../raycast.app main.c -lSDL2 -lm