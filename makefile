all:
	@gcc -Wall -std=c99 -o audioaugery src/main.c -lopengl32 -lfreeglut -lGLU32
	@echo "ALAKAZAM!"
	@./audioaugery
