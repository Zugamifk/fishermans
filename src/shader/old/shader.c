void
shader_update1f(shaderprogram *sp, char *var, float val)
{
	GLuint id = glGetUniformLocation(sp->program, var);
	if (id < 0) {
		printf("\'%s\' does not exist!\n", var);
	} else {
		glUniform1f(id, val);
	}
}

void
shader_update1fv(shaderprogram *sp, char *var, int n, float *val)
{
	GLuint id = glGetUniformLocation(sp->program, var);
	if (id < 0) {
		printf("\'%s\' does not exist!\n", var);
	} else {
		glUniform1fv(id, n, val);
	}
}

void
shader_update1i(shaderprogram *sp, char *var, int val)
{
	GLuint id = glGetUniformLocation(sp->program, var);
	if (id < 0) {
		printf("\'%s\' does not exist!\n", var);
	} else {
		glUniform1i(id, val);
	}
}

void
shader_gentex
(GLuint *tex)
{

	glDeleteTextures(1, tex);
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	int w = 256;
	int h = 256;
	BYTE *data = malloc(w*h*3);
	float r = rand()%1000;
	float g = rand()%1000;
	float b = rand()%1000;
	for(int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int i = (x*h + y)*3;
			int s = 0, d = 10;
			data[i] = 255*perlin_noise(x, y, w, h, r, s, d);
			data[i+1] = 255*perlin_noise(x, y, w, h, g, s, d);
			data[i+2] = 255*perlin_noise(x, y, w, h, b, s, d);
		}
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
}

shaderprogram *
shader_init(const GLchar **filename)
{
	shaderprogram *sp = malloc(sizeof(shaderprogram));
	sp->program = glCreateProgram();
	sp->shaders = malloc(sizeof(GLuint)*2);
	sp->shaderct = 2;
	
	GLuint *shaders = sp->shaders;
	shaders[0] = glCreateShader(GL_VERTEX_SHADER);
	shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar *ss[1];
	int len;
	
	load(filename[0], (char **)ss, &len);
	glShaderSource(shaders[0], 1, ss, &len);
	glCompileShader(shaders[0]);
	GLint success;
	glGetShaderiv(shaders[0], GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaders[0], sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog);
	}
	glAttachShader(sp->program, shaders[0]);
	
	load(filename[1], (char **)ss, &len);
	glShaderSource(shaders[1], 1, ss, &len);
	glCompileShader(shaders[1]);
	glAttachShader(sp->program, shaders[1]);
	
	glLinkProgram(sp->program);
	glGetProgramiv(sp->program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar ErrorLog[1024];
		glGetProgramInfoLog(sp->program, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}
	glValidateProgram(sp->program);
	glUseProgram(sp->program);
	
	GLuint *tex = &(sp->texture);
	shader_gentex(tex);	
	return sp;
}