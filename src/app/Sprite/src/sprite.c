double SPRITE_GRAPHICS = 0;
int* SPRITE_SCREENW;
int* SPRITE_SCREENH;

gui* SpriteGUI;
gui_style* SpriteGUIstyle;
event_bus* Spriteevents;
errorlog* Spritelog;

shaderprogram* Spriteshaders;

void
Sprite_init
(
	errorlog* log,
	event_bus* bus,
	hashtable* vars
)
{
	hashtable_insert(vars, "GRAPHICS", &SPRITE_GRAPHICS);
	SPRITE_SCREENW = hashtable_get(vars, GUIVAR_GUIWIDTH);
	SPRITE_SCREENH = hashtable_get(vars, GUIVAR_GUIHEIGHT);

	SpriteGUI = guilang_compile(SPRITE_GUISPECPATH, SPRITE_GUIPATH, log, bus, vars);
	gui_print(SpriteGUI);
	SpriteGUIstyle = Sprite_guistyle_init();
	Spriteevents = bus;
	Spritelog = log;
}

void
Sprite_initshaders
(
	void
)
{
	Spriteshaders = Spriteshader_init(Spritelog);
	hashtable_insert(Spriteshaders->vars, "time", &TIME);
	shaderprogram_activate(Spriteshaders, Spritelog);
	shaderprogram_addtexture(Spriteshaders, "PERLIN", Spritetex_perlin);
	shaderprogram_addtexture(Spriteshaders, "GRADIENT", Spritetex_2dgradient);
	GLuint* gradient = hashtable_get(Spriteshaders->textures, "GRADIENT");
	glActiveTexture(GL_TEXTURE0);
	GLuint id = glGetUniformLocation(Spriteshaders->program, "tex");
	if (id < 0) {
		//errorlog_logdef(Spritelog, "\'%s\' does not exist!\n", var);
	} else {
		glUniform1i(id, 0);
	}
	glBindTexture(GL_TEXTURE_2D, *gradient);
}

void
Sprite_update
(
	float t,
	float dt
)
{
	SPRITE_GRAPHICS = SPRITE_GRAPHICS + dt;
	gui_update(SpriteGUI, t, dt);
	
	shaderprogram_update(Spriteshaders, t, dt);
}

void
Sprite_draw
(
	float t,
	float dt
)
{

	gui_style_draw(SpriteGUIstyle, SpriteGUI, t, dt);
}

void
Sprite_resize
(
	int w,
	int h
)
{
	gui_resize(SpriteGUI, w, h);
}

void
Sprite_mousemove(mouse_state *ms)
{
	gui_mouseupdate(SpriteGUI, ms->x, *SPRITE_SCREENH - ms->y);
}

void
Sprite_mouseup(mouse_state *ms)
{
	gui_click(SpriteGUI, Spriteevents);
}