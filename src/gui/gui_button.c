typedef struct
_S_gui_button
{
	char* name;
	char* text;
	_gui_dimension* dim;
	vec2* pos;
	#ifdef GUI_DEBUGCOLORS
	color* debugcolor;
	#endif
} gui_button;

gui_button*
gui_button_init
(
	char* name,
	double x,
	double y,
	double w,
	double h
)
{
	gui_button* gb = malloc(sizeof(gui_button));
	gb->name = malloc(strlen(name));
	strcpy(gb->name, name);
	gb->dim = _gui_dimension_init(w, h);
	gb->pos = vec2_new(x, y);
	gb->text = "TEST";
	#ifdef GUI_DEBUGCOLORS
	gb->debugcolor = color_new4(0.0, 6.0, 3.0, 1.0);
	#endif
	return gb;
}


void
gui_button_draw
(
	gui_button* gb,
	double t,
	double dt
)
{
	
	glPushMatrix();
	vec2_translate(gb->pos);
	
	#ifdef GUI_DEBUGDRAWGUI
	color_apply(gb->debugcolor);
	shapes_box(gb->dim->w, gb->dim->h);
	#endif
	
	glPopMatrix();
}
void
gui_button_resize
(
	gui_button* gb,
	double w,
	double h
)
{
	_gui_dimension_resize(gb->dim, w, h);
}

void
gui_button_print
(
	gui_button* gb
)
{
	printf("BUTTON \'%s\':\n", gb->name);
	printf("\tPOS:\t");
	vec2_print(gb->pos);
	printf("\tDIM:\t");
	_gui_dimension_print(gb->dim);
	printf("\tTEXT: \"%s\"", gb->text);
}