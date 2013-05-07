typedef struct
t_mouse_state
{
	int x;
	int y;
	int button;
	int buttonstate;
} mouse_state;

event_bus	*mouse_eventbus;
event_id 	mouse_event_down;
event_id 	mouse_event_up;
event_id	mouse_event_move;

mouse_state	*mouse_laststate;

void
mouse_testevent(mouse_state *s)
{
	printf("%d %d\n", s->x, s->y);
}

void
mouse_recordstate(int x, int y, int button, int buttonstate)
{
	mouse_state *ms = mouse_laststate;
	ms->x = x;
	ms->y = y;
	ms->button = button;
	ms->buttonstate = buttonstate;
}

mouse_state *
mouse_init(event_bus *eb)
{
	mouse_eventbus = eb;
	mouse_event_down = bus_newevent(eb);
	mouse_event_up = bus_newevent(eb);
	mouse_event_move = bus_newevent(eb);
	
	mouse_laststate = malloc(sizeof(mouse_state));
	mouse_recordstate(0, 0, 0, 1);
	
	return mouse_laststate;
}

void
mouse_clickevent(int button, int state, int x, int y)
{
	if (mouse_eventbus == NULL) return;
	mouse_recordstate(x, y, button, state);
	if (state == GLUT_UP) {
		bus_triggerevent(mouse_eventbus, mouse_event_up, mouse_laststate);
	} else
	if (state == GLUT_DOWN) {
		bus_triggerevent(mouse_eventbus, mouse_event_down, mouse_laststate);
	}
}

void
mouse_moveevent(int x, int y)
{
	if (mouse_eventbus == NULL) return;
	mouse_recordstate(x, y, mouse_laststate->button, mouse_laststate->buttonstate);
	bus_triggerevent(mouse_eventbus, mouse_event_move, mouse_laststate);
}