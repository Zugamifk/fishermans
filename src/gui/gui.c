typedef struct
t_gui
{
	gui_dimensions*	dim;
	gui_position*	pos;
	gui_window**	windows;
	int				windownum;
} gui;

void
gui_drawdefault
(
	void* data,
	float t,
	float dt
)
{}


gui*
gui_init
(
	event_bus	*events,
	gui_dimensions*	dims,
	gui_position*	pos
)
{
	gui*	newgui = malloc(sizeof(gui));
	
	newgui->dim 	= dims;
	newgui->pos 	= pos;
	newgui->windows = malloc(sizeof(gui_window *)*GUI_MAXWINDOWS);
	newgui->windownum = 0;
	
	return newgui;
}

gui_WindowID
gui_addwindow
(
	gui*		g,
	gui_window*	w
)
{
	if (g->windownum == GUI_MAXWINDOWS) {
		return -1;
	} else {
		gui_WindowID id = g->windownum;
		g->windows[id] = w;
		g->windownum++;
		return id;
	}
}

void
gui_draw
(
	gui*	g,
	float 	t,
	float	dt
)
{
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluOrtho2D(
		0, g->dim->x,
		0, g->dim->y
		);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	for (int i = 0; i < g->windownum; i++) {
		gui_window *w = g->windows[i];
		gui_window_draw(w, t, dt);
	}
}