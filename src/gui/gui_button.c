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
	gui_text* text;
	_gui_dimension* dim;
	vec2* pos;
	event_id clickeventid;
} gui_button;

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
	
	double ts = h/2.0 > 10.0?10.0:h/2.0;
	double tx = x + 10.0;
	double ty = y + h/2.0 - ts/2.0;
	gb->text = gui_text_init(tx, ty, ts);
	gui_text_settext(gb->text, "TEST", 0);
	gb->clickeventid = EVENT_ID_NULL;
	return gb;
}

void
gui_button_setclickcb
(
	gui_button* gb,
	event_bus* bus,
	char* name
)
{
	gb->clickeventid = bus_getidbyname(bus, name);
}

void
gui_button_reset
(
	gui_button* gb
)
{
	gb->state = GUI_BUTTON_ACTIVE;
}

void
gui_button_update
(
	gui_button* gb
)
{
	gui_text_update(gb->text);
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
	if (gb->state == GUI_BUTTON_HOVER && gb->clickeventid != EVENT_ID_NULL) {
		gb->state = GUI_BUTTON_CLICKED;
		bus_triggerevent(bus, gb->clickeventid, gb);
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
	printf("\tTEXT: \"%s\"\n", gb->text->text);
}