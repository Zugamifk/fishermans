typedef struct
_S_shader
{
	GLuint id;
	
} shader;

shader*
shader_load
(
	const char* filename,
	GLenum shadertype,
	errorlog* log
)
{
	shader* s = malloc(sizeof(shader));
	
	s->id = glCreateShader(shadertype);
	
	char* source;
	int sourcelen;
	load(filename, &source, &sourcelen);
	
	glShaderSource(s->id, 1, ((const GLchar**)&source), &sourcelen);
	glCompileShader(s->id);
	
	GLint success;
	glGetShaderiv(s->id, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[SHADER_STRLEN];
		glGetShaderInfoLog(s->id, sizeof(InfoLog), NULL, InfoLog);
		errorlog_logdef(log, "SHADER COMPILATION", (char*)InfoLog);
	}
	
	return s;
}