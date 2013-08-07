typedef struct
_S_gui_window
{
	char* name;
	_gui_dimension* dim;
	vec2* pos;
	gui_cell* cell;
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
	gw->dim = _gui_dimension_init(w, h);
	gw->pos = vec2_new(x, y);
	gw->cell = NULL;
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
gui_window_update
(
	gui_window* w,
	double t,
	double dt
)
{

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
	color_apply(gw->debugcolor);
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