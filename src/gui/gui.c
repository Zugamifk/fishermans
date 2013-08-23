typedef enum
_E_gui_state
{
	GUI_STATE_INACTIVE,
	GUI_STATE_ACTIVE,
	GUI_STATE_CONTAINSMOUSE
} gui_state;

typedef struct
_S_gui
{
	gui_state state;
	_gui_dimension* dim;
	vec2* pos;
	char* name;
	double aspectratio;
	hashtable* vars;
	hashtable* windows;
	set* activewindows;
	errorlog* log;
	void (*clickcb)(struct _S_gui*);
} gui;

gui*
gui_init
(
	double x,
	double y,
	double w,
	double h
)
{
	gui* g = malloc(sizeof(gui));
	g->state = GUI_STATE_ACTIVE;
	g->dim = _gui_dimension_init(w, h);
	g->pos = vec2_new(x, y);
	g->name = "GUI";
	g->aspectratio = 1.0;
	g->windows = hashtable_init(0);
	g->activewindows = set_init();
	g->clickcb = NULL;
	
	return g;
}

void
gui_reset
(
	gui* g
)
{
	g->state = GUI_STATE_ACTIVE;
	for(int i = 0; i < g->activewindows->size; i++)
	{
		gui_window_reset(g->activewindows->items[i]);
	}
}

void
gui_update
(
	gui* g,
	double t,
	double dt
)
{
}

void
gui_resize
(
	gui* g,
	int x,
	int y
)
{
	_gui_dimension_resize(g->dim, x, y);
}

void
gui_openwindow
(
	gui* g,
	gui_window* w
)
{
	hashtable_insert(g->windows, w->name, w);
	set_add(g->activewindows, w);
}

bool
gui_contains
(
	gui* g,
	double x,
	double y
)
{
	return (g->pos->x < x &&
			g->pos->y < y &&
			x < g->pos->x + g->dim->w &&
			y < g->pos->y + g->dim->h);
}

void
gui_mouseupdate
(
	gui* g,
	int x,
	int y
)
{
	gui_reset(g);
	if (gui_contains(g, x, y)) {
		for(int i = 0; i < g->activewindows->size; i++)
		{
			gui_window_mouseupdate(g->activewindows->items[i], x, y);
		}
		g->state = GUI_STATE_CONTAINSMOUSE;
	} else {
		g->state = GUI_STATE_ACTIVE;
	}
}

void
gui_click
(
	gui* g,
	event_bus* bus
)
{
	if (g->state == GUI_STATE_CONTAINSMOUSE) {
		if (g->clickcb != NULL) g->clickcb(g);
		
		void* gw;
		for(set_begin(g->activewindows, &gw); 
			set_end(g->activewindows); 
			set_next(g->activewindows, &gw))
		{
			gui_window_click(gw, bus);
		}
	
	}
}

void
gui_print
(
	gui* g
)
{
	printf("GUI:\n");
	printf("\tPOS:\t");
	vec2_print(g->pos);
	printf("\tDIM:\t");
	_gui_dimension_print(g->dim);
	
	char* k;
	void* v;
	for(	hashtable_begin(g->windows, &k, &v);
			hashtable_end(g->windows); 
			hashtable_next(g->windows, &k, &v)) 
	{
		printf("?\n");
		gui_window_print(v);
	}
}