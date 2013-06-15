typedef struct
t_gui_window
{
	gui_dimensions	*dim;
	gui_position	*pos;
	gui_cell		*content;
	gui_state 		state;
	gui_drawcallback drawcb;
} gui_window;

gui_window*
gui_window_init
(
	event_bus		*events,
	gui_dimensions	*d,
	gui_position	*p
)
{
	gui_window *window = malloc(sizeof(gui_window));
	
	window->dim = d;
	window->pos = p;
	window->content = NULL;
	window->state = GUI_STATE_ACTIVE;
	window->drawcb = gui_drawdefault;
	
	return window;
}

void
gui_window_draw
(
	gui_window	*w,
	float		t,
	float		dt
)
{
	glPushMatrix();

	vec2_translate(w->pos);
	w->drawcb(w, t, dt);
	
	gui_cell_draw(w->content, t, dt);
	
	glPopMatrix();
}