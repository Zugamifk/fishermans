typedef struct
_S_gui_console
{
	char* name;
	_gui_box* bounds;
	gui_textbox* out;
	gui_textin* in;
	gui_button* submit;
	hashtable* settings;
} gui_console;
#define GUI_CONSOLE_TEXTPT (0.1)

gui_console*
gui_console_init
(
	char* name,
	double x,
	double y,
	double w,
	double h
)
{
	gui_console* gc = malloc(sizeof(gui_console));
	
	gc->name = malloc(strlen(name));
	strcpy(gc->name, name);
	
	gc->bounds = _gui_box_init(x, y, w, h);
	
	char tname[1024];
	sprintf(tname, "%s TEXTBOX", name);
	gc->out = gui_textbox_init(tname,x,y,w,h,GUI_CONSOLE_TEXTPT);
	
	sprintf(tname, "%s TEXT INPUT", name);
	gc->in = gui_textin_init(tname, x, y, w, h);
	
	sprintf(tname, "%s SUBMIT BUTTON", name);
	gc->submit = gui_button_init(name, x, y, w, h);
	
	gc->settings = hashtable_init(0);
	return gc;
}