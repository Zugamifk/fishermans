double SPRITE_ENERGY = 0;

gui* SpriteGUI;
gui_style* SpriteGUIstyle;

void
Sprite_init
(
	errorlog* log,
	event_bus* bus,
	hashtable* vars
)
{
	hashtable_insert(vars, "Sprite Energy", &SPRITE_ENERGY);

	SpriteGUI = guilang_compile("data/guilangspec1", "src/app/Sprite/data/sprite.gui", log, bus, vars);
	printf("?\n");
	gui_print(SpriteGUI);
	//SpriteGUIstyle = gui_debugstyle_init();
}

void
Sprite_update
(

)
{

}

void
Sprite_draw
(

)
{

}
