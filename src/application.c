typedef void (*application_init_cb)(errorlog*, event_bus*, hashtable*);
typedef void (*application_update_cb)(float, float);
typedef void (*application_draw_cb)(float, float);
typedef void (*application_resize_cb)(int, int);
typedef void (*application_mousemove_cb)(mouse_state*);
typedef void (*application_mouseup_cb)(mouse_state*);
typedef void (*application_mousedown_cb)(mouse_state*);
typedef void (*application_keyboarddown_cb)(keyboard_state*, int);
typedef void (*application_keyboardup_cb)(keyboard_state*, int);
typedef void (*application_quit_cb)(void);

typedef struct
_application_data_s
{
	errorlog *log;
	event_bus *bus;
	hashtable *vars;
	hashtable *fonts;
	application_init_cb init_cb;
	application_update_cb update_cb;
	application_draw_cb draw_cb;
	application_resize_cb resize_cb;
	application_mousemove_cb mousemove_cb;
	application_mouseup_cb mouseup_cb;
	application_mousedown_cb mousedown_cb;
	application_keyboarddown_cb keyboarddown_cb;
	application_keyboardup_cb keyboardup_cb;
	application_quit_cb quit_cb;
} application_data;

application_data*
application_data_init
(
	errorlog* log,
	event_bus* bus,
	hashtable* vars
)
{
	application_data *app = calloc(1, sizeof(application_data));
	
	app->log = log;
	app->bus = bus;
	app->vars = vars;
	
	return app;
}