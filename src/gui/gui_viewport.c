typedef enum
_E_gui_viewport_state
{
	GUI_VIEWPORT_INACTIVE,
	GUI_VIEWPORT_ACTIVE,
	GUI_VIEWPORT_HOVER,
	GUI_VIEWPORT_CLICKED
} gui_viewport_state;

typedef struct
_T_gui_viewport
{
	char* name;
	vec2* pos;
	_gui_dimension* dim;
	vec2* scale;
	gui_viewport_state state;
	event_id clickeventid;
	double mousex;
	double mousey;
} gui_viewport;

gui_viewport*
gui_viewport_init
(
	char* name,
	double x,
	double y,
	double w,
	double h
)
{

	gui_viewport* gv = malloc(sizeof(gui_viewport));
	gv->name = malloc(strlen(name));
	strcpy(gv->name, name);
	gv->state = GUI_VIEWPORT_ACTIVE;
	gv->dim = _gui_dimension_init(w, h);
	gv->pos = vec2_new(x, y);
	gv->scale = vec2_new(w, h);
	gv->clickeventid = EVENT_ID_NULL;
	gv->mousex = 0.0;
	gv->mousey = 0.0;
	return gv;
}

void
gui_viewport_setclickcb
(
	gui_viewport* gv,
	event_bus* bus,
	char* name
)
{
	gv->clickeventid = bus_getidbyname(bus, name);
}

void
gui_viewport_reset
(
	gui_viewport* gv
)
{
	gv->state = GUI_VIEWPORT_ACTIVE;
}

bool
gui_viewport_contains
(
	gui_viewport* gv,
	double x,
	double y
)
{
	return (gv->pos->x < x &&
			gv->pos->y < y &&
			x < gv->pos->x + gv->dim->w &&
			y < gv->pos->y + gv->dim->h);
}

void
gui_viewport_click
(
	gui_viewport* gv,
	event_bus* bus
)
{
	if (gv->state == GUI_VIEWPORT_HOVER && gv->clickeventid != EVENT_ID_NULL) {
		gv->state = GUI_VIEWPORT_CLICKED;
		bus_triggerevent(bus, gv->clickeventid, gv);
	}
}

void
gui_viewport_mouseupdate
(
	gui_viewport* gv,
	int x,
	int y
)
{
	if (gui_viewport_contains(gv, x, y) && gv->state == GUI_VIEWPORT_ACTIVE) {
		gv->state = GUI_VIEWPORT_HOVER;
		gv->mousex = ((double)x - gv->pos->x)/gv->dim->w;
		gv->mousey = ((double)y - gv->pos->y)/gv->dim->h;
	}
}

void
gui_viewport_resize
(
	gui_viewport* gv,
	double w,
	double h
)
{
	_gui_dimension_resize(gv->dim, w, h);
}	

void
gui_viewport_print
(
	gui_viewport* gv
)
{
	printf("VIEWPORT \'%s\':\n", gv->name);
	printf("\tPOS:\t");
	vec2_print(gv->pos);
	printf("\tDIM:\t");
	_gui_dimension_print(gv->dim);
	printf("\tSCALE:\t");
	vec2_print(gv->scale);
}