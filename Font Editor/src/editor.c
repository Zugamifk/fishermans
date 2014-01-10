int* editor_width;
int* editor_height;
float* editor_time;
float* editor_dtime;
char* editor_directory;
char* editor_file;
const unsigned int texside = 512;
const float editor_texs = 512.0;

gui* editor_gui;
gui_style* editor_guistyle;
texture *perlin, *gradient;
event_bus* editor_events;
errorlog* editor_log;

audiosystem* editor_audio;
lang_grammar* editor_audiogrammar;

shaderprogram* editor_shaders;

void
editor_error
(
	const char* message,
	const char* word
)
{
	char errorstring[1<<12]; 
	sprintf(errorstring, message, word);
	errorlog_logdef(editor_log, "FONT EDITOR", errorstring);
}


void
editor_init
(
	errorlog* log,
	event_bus* bus,
	hashtable* vars
)
{
	editor_log = log;
	editor_events = bus;
	editor_width = hashtable_get(vars, "WIDTH");
	editor_height = hashtable_get(vars, "HEIGHT");
	editor_time = hashtable_get(vars, "TIME");
	editor_dtime = hashtable_get(vars, "DTIME");
	
	// Init gui
	char* editor_guifile = "editor.gui";
	char editor_guipath[1024];
	sprintf(editor_guipath, "%s%s", EDITOR_DATAPATH, editor_guifile);
	editor_gui = guilang_load(editor_guipath, GUILANGSPEC1, log, bus, vars);
	
	editor_guistyle = gui_style_initdefault();
	hashtable_insert(editor_guistyle->args, "TEXW", (HASHTABLEDATA*)&editor_texs);
	hashtable_insert(editor_guistyle->args, "TEXH", (HASHTABLEDATA*)&editor_texs);
	
	// graphics
	char* vsfile = "shader.vs";
	char* fsfile = "shader.fs";
	char vspath[1024], fspath[1024];
	sprintf(vspath, "%s%s", EDITOR_DATAPATH, vsfile);
	sprintf(fspath, "%s%s", EDITOR_DATAPATH, fsfile);
	editor_shaders = shaderprogram_init1(vspath, fspath, editor_log);

	shaderprogram_addvar(editor_shaders, "time", &TIME, SHADER_FLOAT1, 1);
	shaderprogram_activate(editor_shaders, editor_log);
	
	// textures
tick();
	hashtable* perlinargs = hashtable_init(1);
	hashtable_insert(perlinargs, "TIME", editor_time);
	perlin = texture_initatom(texside, texside, TEXTURE_PERLIN, perlinargs);
tock();
	gradient = texture_initatom(texside, texside, TEXTURE_GRADIENT, perlinargs);
	shaderprogram_addvar(editor_shaders, "pxside", editor_width, SHADER_INT1, 1);
	shaderprogram_addtexturedata(editor_shaders, perlin);
	shaderprogram_addtexturedata(editor_shaders, gradient);
	
	// Set current tex
	GLuint* g = hashtable_get(editor_shaders->textures, "PERLIN");
	glActiveTexture(GL_TEXTURE0);
	GLuint id = glGetUniformLocation(editor_shaders->program, "tex");
	if (id < 0) {
		//errorlog_logdef(Spritelog, "\'%s\' does not exist!\n", var);
	} else {
		glUniform1i(id, 0);
	}
	glBindTexture(GL_TEXTURE_2D, *g);
}

void
editor_update
(
	float t,
	float dt
)
{
	gui_update(editor_gui, t, dt);	
	shaderprogram_update(editor_shaders, t, dt);
	}

void
editor_draw
(
	float t,
	float dt
)
{

	gui_style_draw(editor_guistyle, editor_gui, t, dt);
}

void
editor_resize
(
	int w,
	int h
)
{
	gui_resize(editor_gui, w, h);
}

application_data*
editor_getappdata
(
	errorlog* log,
	event_bus* bus,
	hashtable* vars
)
{
	application_data *E = application_data_init(log, bus, vars);
	
	E->init_cb = editor_init;
	E->update_cb = editor_update;
	E->draw_cb = editor_draw;
	E->resize_cb = editor_resize;
	
	return E;
}
