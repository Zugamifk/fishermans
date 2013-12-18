double SPRITE_GRAPHICS = 0;
int* SPRITE_SCREENW;
int* SPRITE_SCREENH;
float SPRITE_SPECTRUM[512];

gui* SpriteGUI;
gui_style* SpriteGUIstyle;
event_bus* Spriteevents;
errorlog* Spritelog;

audiosystem* Spriteaudio;
lang_grammar* Spritenoiselanggrammar;

shaderprogram* Spriteshaders;

void
Sprite_init
(
	errorlog* log,
	event_bus* bus,
	hashtable* vars
)
{

	// add vars
	hashtable_insert(vars, "GRAPHICS", &SPRITE_GRAPHICS);
	SPRITE_SCREENW = hashtable_get(vars, GUIVAR_GUIWIDTH);
	SPRITE_SCREENH = hashtable_get(vars, GUIVAR_GUIHEIGHT);
	
	// Init gui
	SpriteGUI = guilang_compile(SPRITE_GUISPECPATH, SPRITE_GUIPATH, log, bus, vars);
	gui_print(SpriteGUI);
	SpriteGUIstyle = Sprite_guistyle_init();
	
	Spriteevents = bus;
	Spritelog = log;

	// audio
	Spritenoiselanggrammar = noiselang_init(SPRITE_AUDIOSPECPATH, log);
	audio_sound* Spritetestsound = noiselang_loadsound(SPRITE_SOUNDPATH, Spritenoiselanggrammar, Spritelog);
	audio_sound_bank(Spritetestsound, 5.0, 44100.0);
	
	Spriteaudio = audiosystem_init("Sprite Audio", log);
	audiostream* poo = audiostream_init(Spriteaudio, Spritetestsound);
	FMOD_System_PlaySound(Spriteaudio->sys, 0, poo->fmodsound, false, NULL);

	// graphics
	Spriteshaders = Spriteshader_init(Spritelog);
	shaderprogram_addvar(Spriteshaders, "time", &TIME, SHADER_FLOAT1, 1);
	shaderprogram_addvar(Spriteshaders, "graphics", &SPRITE_GRAPHICS, SHADER_FLOAT1, 1);
	shaderprogram_addvar(Spriteshaders, "spectrum", &SPRITE_SPECTRUM, SHADER_FLOAT1V, 128);
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
	audiosystem_update(Spriteaudio, t, dt);
	
	
	AUDIO_ERROR err = FMOD_System_GetSpectrum(Spriteaudio->sys, SPRITE_SPECTRUM, 512, 0, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS);
	audiosystem_resultok(Spriteaudio, Spritelog, err);
	float max = 0.0;
	float val = 0.0;
	for (int i = 0; i<512;i++) {
		val = SPRITE_SPECTRUM[i];
		max = val>max?val:max;
	}
	for (int i = 0; i<512;i++) {
		SPRITE_SPECTRUM[i] = SPRITE_SPECTRUM[i]/max;
	}
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

void
Sprite_keyup(keyboard_state* ks, int key)
{
	gui_keyboardupdate(SpriteGUI, ks, key, KEYBOARD_KEY);
}

application_data*
Sprite_getappdata
(
	errorlog* log,
	event_bus* bus,
	hashtable* vars
)
{
	application_data* S = application_data_init(log, bus, vars);
	
	S->init_cb = Sprite_init;
	S->update_cb = Sprite_update;
	S->draw_cb = Sprite_draw;
	S->resize_cb = Sprite_resize;
	S->mousemove_cb = Sprite_mousemove;
	S->mouseup_cb = Sprite_mouseup;
	S->keyboardup_cb = Sprite_keyup;
	
	return S;
}