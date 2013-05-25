all:
	@gcc -Wall -std=c99 -o audioaugery src/main.c -lfmodex -lopengl32 -lfreeglut -lGLU32
	@echo "ALAKAZAM!"
	@./audioaugery
