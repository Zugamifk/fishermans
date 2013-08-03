typedef struct
_S_gui_window
{
	char* name;
	_gui_dimension* dim;
	vec2* pos;
	list* cells;
	bool ishorz;
	#ifdef GUI_DEBUGCOLORS
	color* debugcolor;
	#endif
} gui_window;

gui_window*
gui_window_init
(
	double x,
	double y,
	double w,
	double h
)
{
	gui_window* gw = malloc(sizeof(gui_window));
	gw->name = "FIX THIS SHIT";
	gw->dim = _gui_dimension_init(w, h);
	gw->pos = vec2_new(x, y);
	gw->cells = list_new();
	#ifdef GUI_DEBUGCOLORS
	gw->debugcolor = color_new4(1.0, 0.0, 0.0, 1.0);
	#endif
	return gw;
}

void
gui_window_update
(
	gui_window* g,
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
	#ifdef GUI_DEBUGDRAWGUI
	color_apply(gw->debugcolor);
	shapes_box(gw->dim->w, gw->dim->h);
	#endif
	
	glPopMatrix();
}
