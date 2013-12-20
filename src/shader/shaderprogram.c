typedef struct
_S_shaderprogram
{
	GLuint program;
	hashtable* shaders;
	hashtable* textures;
	hashtable* vars;
} shaderprogram;

typedef enum
_E_shaderprogram_vartype
{
	SHADER_ERR = -10000,
	SHADER_NULL,
	SHADER_FLOAT1,
	SHADER_FLOAT1V,
	SHADER_INT1,
	SHADER_INT1V
} shaderprogram_vartype;

typedef union
_E_shaderprogram_var
{
	double* f;
	int* i;
} shaderprogram_var;

typedef struct
_S_shaderprogram_varbucket
{
	shaderprogram_vartype type;
	unsigned int len;
	shaderprogram_var val;
} shaderprogram_varbucket;

shaderprogram*
shaderprogram_init
(
	const char** vshaders,
	int vsnum,
	const char** fshaders,
	int fsnum,
	errorlog* log
)
{
	shaderprogram *sp = malloc(sizeof(shaderprogram));

	sp->program = glCreateProgram();
	
	sp->shaders = hashtable_init(0);
	for (int i = 0; i < vsnum; i++) {
		char* name = malloc(SHADER_STRLEN);
		sprintf(name, "VERTEX SHADER %d", i);
		shader* s = shader_load(vshaders[i], GL_VERTEX_SHADER, log);
		
		glAttachShader(sp->program, s->id);
		
		hashtable_insert(sp->shaders, name, s);
	}
	for (int i = 0; i < fsnum; i++) {
		char* name = malloc(SHADER_STRLEN);
		sprintf(name, "FRAGMENT SHADER %d", i);
		shader* s = shader_load(fshaders[i], GL_FRAGMENT_SHADER, log);
		
		glAttachShader(sp->program, s->id);
		
		hashtable_insert(sp->shaders, name, s);
	}
	
	sp->textures = hashtable_init(0);
	sp->vars = hashtable_init(0);
	
	return sp;
}

shaderprogram*
shaderprogram_init1
(
	const char* vs,
	const char* fs,
	errorlog* log
)
{
	const char *vsa[] = {vs}; 
	const char *fsa[] = {fs}; 
	return shaderprogram_init(vsa, 1, fsa, 1, log);
}

void
shaderprogram_activate
(
	shaderprogram* sp,
	errorlog* log
)
{
	glLinkProgram(sp->program);
	GLint success;
	glGetProgramiv(sp->program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar ErrorLog[SHADER_STRLEN];
		glGetProgramInfoLog(sp->program, sizeof(ErrorLog), NULL, ErrorLog);
		errorlog_logdef(log, "SHADER PROGRAM LINKING (glLinkProgram)", (char*)ErrorLog);
	}
	glValidateProgram(sp->program);
	glUseProgram(sp->program);
}

void
shaderprogram_addtexture
(
	shaderprogram* sp,
	char* name,
	void (*gencb)(GLuint*)
)
{
	GLuint* id = malloc(sizeof(GLuint));
	gencb(id);
	hashtable_insert(sp->textures, name, id);
}

void
shaderprogram_addtexturedata
(
	shaderprogram* sp,
	texture* tex
)
{
	hashtable_insert(sp->textures, tex->name, &(tex->textureid));
}

void
shaderprogram_addvar
(
	shaderprogram* sp,
	char* name,
	void* value,
	shaderprogram_vartype type,
	unsigned int len
)
{
	shaderprogram_varbucket* vb = malloc(sizeof(shaderprogram_varbucket));
	vb->type = type;
	switch(vb->type) {
		case SHADER_FLOAT1:
		case SHADER_FLOAT1V: 
			vb->val.f = (double*)value;
			break;
		case SHADER_INT1:
		case SHADER_INT1V:
			vb->val.i = (int*)value;
			break;
		default:
			break;
	}
	vb->len = len;
	hashtable_insert(sp->vars, name, vb);
}

void
shaderprogram_update
(
	shaderprogram* sp,
	float t,
	float dt
)
{
	char* k;
	HASHTABLEDATA* v;
	for (hashtable_begin(sp->vars, &k, &v); hashtable_end(sp->vars); hashtable_next(sp->vars, &k, &v)) {
		GLuint id = glGetUniformLocation(sp->program, k);
		if (id < 0) {
			// Ignore
		} else {
			shaderprogram_varbucket* vb = (shaderprogram_varbucket*)v;
			switch(vb->type) {
				case SHADER_FLOAT1: glUniform1f(id, *(vb->val.f)); break;
				case SHADER_FLOAT1V: glUniform1fv(id, vb->len, (GLfloat*)vb->val.f); break;
				case SHADER_INT1: glUniform1i(id, *(vb->val.i)); break;
				case SHADER_INT1V: glUniform1iv(id, vb->len, vb->val.i); break;
				default: break;
			}
		}
	}
}