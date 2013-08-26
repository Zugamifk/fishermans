typedef enum
_E_gui_viewport_state
{
	GUI_VIEWPORT_INACTIVE,
	GUI_VIEWPORT_ACTIVE,
	GUI_VIEWPORT_HOVER
} gui_viewport_state;

typedef struct
_T_gui_viewport
{
	char* name;
	vec2* pos;
	_gui_dimension* dim;
	vec2* scale;
	gui_viewport_state state;
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

	return gv;
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