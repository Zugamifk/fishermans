typedef struct
_S_gui_slider
{
	char* name;
	gui_button* clickarea;
	_gui_dimension* dim;
	vec2* pos;
	double lastp;
	double value;
} gui_slider;

gui_slider*
gui_slider_init
(
	char* name,
	double x,
	double y,
	double w,
	double h
)
{
	gui_slider* gs = malloc(sizeof(gui_slider));
	gs->name = malloc(strlen(name));
	strcpy(gs->name, name);
	
	char buttonname[1024];
	strcpy(buttonname, name);
	strcat(buttonname, " CLICK BOX");
	gs->clickarea = gui_button_init(buttonname, 0.0, 0.0, w, h);
	
	gs->dim = _gui_dimension_init(w, h);
	gs->pos = vec2_new(x, y);
	gs->lastp = 0.0;
	gs->value = 0.0;
	return gs;
}

bool
gui_slider_contains
(
	gui_slider* gs,
	double x,
	double y
)
{
	return (gs->pos->x < x && gs->pos->y < y && x < gs->pos->x + gs->dim->w && y < gs->pos->y + gs->dim->h);
}

void
gui_slider_click
(
	gui_slider* gs,
	event_bus* bus
)
{
	if (gs->clickarea->state == GUI_BUTTON_HOVER) {
		gs->clickarea->state = GUI_BUTTON_CLICKED;
		gs->value = gs->lastp;
		if (gs->clickarea->clickeventid != EVENT_ID_NULL)
			bus_triggerevent(bus, gs->clickarea->clickeventid, gs);
	}
}


void
gui_slider_mouseupdate
(
	gui_slider* gs,
	int x,
	int y
)
{
	if (gui_slider_contains(gs, x, y) && gs->clickarea->state == GUI_BUTTON_ACTIVE) {
		gs->lastp = ((double)x-gs->pos->x)/gs->dim->w;
		gs->clickarea->state = GUI_BUTTON_HOVER;
	}
}

void
gui_slider_resize
(
	gui_slider* gs,
	double w,
	double h
)
{
	_gui_dimension_resize(gs->dim, w, h);
	gui_button_resize(gs->clickarea, w, h);
}

void
gui_slider_print
(
	gui_slider* gs
)
{
	printf("SLIDER \'%s\':\n", gs->name);
	printf("\tPOS:\t");
	vec2_print(gs->pos);
	printf("\tDIM:\t");
	_gui_dimension_print(gs->dim);
	printf("VALUE: %.4f\t", gs->value);
	printf("LASTP: %.4f\n", gs->lastp);
	gui_button_print(gs->clickarea);
}