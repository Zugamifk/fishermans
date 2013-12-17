typedef struct
_S_gui_textbox
{
	char* name;
	_gui_box* bounds;
	gui_text* text;
} gui_textbox;

gui_textbox*
gui_textbox_init
(
	char* name,
	double x,
	double y,
	double w,
	double h,
	double s
)
{
	gui_textbox* gt = malloc(sizeof(gui_textbox));
	gt->name = malloc(strlen(name));
	strcpy(gt->name, name);
	gt->bounds = _gui_box_init(x, y, w, h);
	gt->text = gui_text_init(x, y+h-s, s);
	return gt;
}