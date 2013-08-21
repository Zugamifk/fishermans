typedef enum
_E_gui_window_state
{
	GUI_WINDOW_INACTIVE,
	GUI_WINDOW_ACTIVE,
	GUI_WINDOW_CONTAINSMOUSE
} gui_window_state;

typedef struct
_S_gui_window
{
	char* name;
	gui_window_state state;
	_gui_dimension* dim;
	vec2* pos;
	gui_cell* cell;
	void (*clickcb)(struct _S_gui_window*);
	#ifdef GUI_DEBUGCOLORS
	color* debugcolor;
	#endif
} gui_window;

gui_window*
gui_window_init
(
	const char* name,
	double x,
	double y,
	double w,
	double h
)
{
	gui_window* gw = malloc(sizeof(gui_window));
	gw->name = malloc(strlen(name));
	strcpy(gw->name, name);
	gw->state = GUI_WINDOW_ACTIVE;
	gw->dim = _gui_dimension_init(w, h);
	gw->pos = vec2_new(x, y);
	gw->cell = NULL;
	gw->clickcb = NULL;
	#ifdef GUI_DEBUGCOLORS
	gw->debugcolor = color_new4(0.0, 0.0, 1.0, 1.0);
	#endif
	return gw;
}

void
gui_window_setcell
(
	gui_window* w,
	gui_cell* cell
)
{
	w->cell = cell;
	gui_cell_resize(cell, w->dim->w, w->dim->h);
}

void
gui_window_reset
(
	gui_window* gw
)
{
	gw->state = GUI_WINDOW_ACTIVE;
	if (gw->cell != NULL) {
		gui_cell_reset(gw->cell);
	}
}

void
gui_window_update
(
	gui_window* w,
	double t,
	double dt
)
{

}

void
gui_window_resize
(
	gui_window* gw,
	int w,
	int h
)
{
	_gui_dimension_resize(gw->dim, w, h);
	if (gw->cell != NULL) {
		gui_cell_resize(gw->cell, w, h);
	}
}

bool
gui_window_contains
(
	gui_window* gw,
	double x,
	double y
)
{
	return (gw->pos->x < x &&
			gw->pos->y < y &&
			x < gw->pos->x + gw->dim->w &&
			y < gw->pos->y + gw->dim->h);
}

void
gui_window_mouseupdate
(
	gui_window* gw,
	int x,
	int y
)
{
	if (gui_window_contains(gw, x, y)) {
		if (gw->cell != NULL) {
			gui_cell_mouseupdate(gw->cell, x - gw->pos->x, y - gw->pos->y);
		}
		gw->state = GUI_WINDOW_CONTAINSMOUSE;
	} else {
		gw->state = GUI_WINDOW_ACTIVE;
	}
}

void
gui_window_click
(
	gui_window* gw,
	event_bus* bus
)
{
	if (gw->state == GUI_WINDOW_CONTAINSMOUSE) {
		if (gw->clickcb != NULL) gw->clickcb(gw);
		
		if (gw->cell != NULL) {
			gui_cell_click(gw->cell, bus);
		}
	}
}

void
gui_window_draw
(
	gui_window* gw,
	double t,
	double dt
)
{
	
	glPushMatrix();
	vec2_translate(gw->pos);
	gui_cell_draw(gw->cell, t, dt);
	
	#ifdef GUI_DEBUGDRAWGUI
	if (gw->state == GUI_WINDOW_CONTAINSMOUSE) {
		color_applyinverse(gw->debugcolor);
	} else {
		color_apply(gw->debugcolor);
	}
	shapes_box(gw->dim->w, gw->dim->h);
	#endif
	
	glPopMatrix();
}

void
gui_window_print
(
	gui_window* gw
)
{
	printf("WINDOW \'%s\':\n", gw->name);
	printf("\tPOS:\t");
	vec2_print(gw->pos);
	printf("\tDIM:\t");
	_gui_dimension_print(gw->dim);
	if(gw->cell != NULL) {
		gui_cell_print(gw->cell);
	}
}