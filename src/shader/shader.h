typedef struct
t_shaderprogram
{
	GLuint program;
	GLuint *shaders;
	int shaderct;
	GLuint texture;
} shaderprogram;

#define SDR_NUMDIALS 7

#include "shader.c"