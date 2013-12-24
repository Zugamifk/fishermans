
typedef struct
_texture_s
{
	char* name;
	GLuint textureid;
	BYTE* (*gen_cb)(struct _texture_s*);
	unsigned int width;
	unsigned int height;
	hashtable* vars;
} texture;
typedef BYTE* (*texture_gen_cb)(texture*);

texture*
texture_init
(
	const char* name,
	unsigned int w,
	unsigned int h,
	hashtable* vars
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
	
	if (vars == NULL) {
		t->vars = hashtable_init(1);
	} else {
		t->vars = vars;
	}
	
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
	//DeleteTextures(1, &id);
	//GenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	BYTE *data;
	
	data = t->gen_cb(t);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, t->width, t->height, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	free(data);
}
	