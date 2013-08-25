typedef struct
_S_shaderprogram
{
	GLuint program;
	hashtable* shaders;
	hashtable* textures;
	hashtable* vars;
} shaderprogram;

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
shaderprogram_update
(
	shaderprogram* sp,
	float t,
	float dt
)
{
	
}