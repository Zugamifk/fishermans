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
	SpriteGUIstyle = gui_debugstyle_init();
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
	shaderprogram_activate(Spriteshaders, Spritelog);
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