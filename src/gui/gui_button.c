typedef enum
_E_gui_button_state
{
	GUI_BUTTON_INACTIVE,
	GUI_BUTTON_ACTIVE,
	GUI_BUTTON_HOVER,
	GUI_BUTTON_CLICKED
} gui_button_state;

typedef struct
_S_gui_button
{
	char* name;
	gui_button_state state;
	char* text;
	_gui_dimension* dim;
	vec2* pos;
	void (*clickcb)(struct _S_gui_button*);
	#ifdef GUI_DEBUGCOLORS
	color* debugcolor;
	#endif
} gui_button;

void guitestclick(gui_button* g) {
	_gui_dimension_print(g->dim);
}

gui_button*
gui_button_init
(
	char* name,
	double x,
	double y,
	double w,
	double h
)
{
	gui_button* gb = malloc(sizeof(gui_button));
	gb->name = malloc(strlen(name));
	strcpy(gb->name, name);
	gb->state = GUI_BUTTON_ACTIVE;
	gb->dim = _gui_dimension_init(w, h);
	gb->pos = vec2_new(x, y);
	gb->text = "TEST";
	gb->clickcb = guitestclick;
	#ifdef GUI_DEBUGCOLORS
	gb->debugcolor = color_new4(0.0, 6.0, 3.0, 1.0);
	#endif
	return gb;
}

void
gui_button_reset
(
	gui_button* gb
)
{
	gb->state = GUI_BUTTON_ACTIVE;
}
 
bool
gui_button_contains
(
	gui_button* gb,
	double x,
	double y
)
{
	return (gb->pos->x < x &&
			gb->pos->y < y &&
			x < gb->pos->x + gb->dim->w &&
			y < gb->pos->y + gb->dim->h);
}

void
gui_button_click
(
	gui_button* gb,
	event_bus* bus
)
{
	if (gb->state == GUI_BUTTON_HOVER && gb->clickcb != NULL) {
		gb->state = GUI_BUTTON_CLICKED;
		gb->clickcb(gb);
	}
}

void
gui_button_mouseupdate
(
	gui_button* gb,
	int x,
	int y
)
{
	if (gui_button_contains(gb, x, y) && gb->state == GUI_BUTTON_ACTIVE) {
		gb->state = GUI_BUTTON_HOVER;
	}
}

void
gui_button_draw
(
	gui_button* gb,
	double t,
	double dt
)
{
	
	glPushMatrix();
	vec2_translate(gb->pos);
	
	#ifdef GUI_DEBUGDRAWGUI
	if (gb->state == GUI_BUTTON_HOVER) {
		color_applyinverse(gb->debugcolor);
	} else {
		color_apply(gb->debugcolor);
	}
	shapes_box(gb->dim->w, gb->dim->h);
	#endif
	
	glPopMatrix();
}
void
gui_button_resize
(
	gui_button* gb,
	double w,
	double h
)
{
	_gui_dimension_resize(gb->dim, w, h);
}

void
gui_button_print
(
	gui_button* gb
)
{
	printf("BUTTON \'%s\':\n", gb->name);
	printf("\tPOS:\t");
	vec2_print(gb->pos);
	printf("\tDIM:\t");
	_gui_dimension_print(gb->dim);
	printf("\tTEXT: \"%s\"", gb->text);
}