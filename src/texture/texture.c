typedef struct
_texture_gen_args_s
{
	unsigned int w;
	unsigned int h;
	float time;
	float dtime;
	int *numbers;
	int numnums;
} texture_gen_args;
typedef BYTE* (*texture_gen_cb)(texture_gen_args);

typedef struct
_texture_s
{
	char* name;
	GLuint textureid;
	texture_gen_cb gen_cb;
	unsigned int width;
	unsigned int height;
} texture;

texture*
texture_init
(
	const char* name,
	unsigned int w,
	unsigned int h
)
{
	texture* t = malloc(sizeof(texture));
	t->name = malloc(strlen(name));
	strcpy(t->name, name);
	GLuint id = t->textureid;
	
	glDeleteTextures(1, &id);
	glGenTextures(1, &id);

	t->width = w;
	t->height = h;
	
	return t;
}

void
texture_apply
(
	texture *t
)
{
	glBindTexture(GL_TEXTURE_2D, t->textureid);
}

void
texture_generate
(
	texture *t
)
{
	GLuint id = t->textureid;
	glDeleteTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	BYTE *data;
	texture_gen_args args;
	args.w = t->width;
	args.h = t->height;
	data = t->gen_cb(args);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, t->width, t->height, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	free(data);
}
	