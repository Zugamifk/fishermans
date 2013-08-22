#define GUI_STYLE_DATA void*
typedef struct
_S_gui_style
{
	colorpack* palette;
	fontinfo* font;
	void (*gui)(struct _S_gui_style*, gui*, double, double);
	void (*window)(struct _S_gui_style*, gui_window*, double, double);
	void (*cell)(struct _S_gui_style*, gui_cell*, double, double);
	void (*button)(struct _S_gui_style*, gui_button*, double, double);
} gui_style;

gui_style*
gui_style_init
(
	void
)
{
	gui_style* gs = calloc(1, sizeof(gui_style));

	return gs;
}

void
gui_style_setpalette
(
	gui_style* gs,
	color** colors,
	int len
)
{
	gs->palette = color_newpack(colors, len);
}

void
gui_style_setfont
(
	gui_style* gs,
	fontinfo* f
)
{
	gs->font = f;
}

color*
gui_style_getcolor
(
	gui_style* style,
	int i
)
{
	return style->palette->colors[i];
}

void
_gui_style_drawcell
(
	gui_cell* gc,
	gui_style* style,
	double t,
	double dt
)
{
	if (gc == NULL) return;
	glPushMatrix();
	vec2_translate(gc->pos);
	if (style->cell != NULL) style->cell(style, gc, t, dt);

	switch (gc->content) {
		case GUI_CELL_EMPTY: break;
		case GUI_CELL_HORIZONTALCELLS:
		case GUI_CELL_VERTICALCELLS: {
			list* l = gc->cells;
			_gui_style_drawcell(l->data, style, t, dt);
			for(l = l->next; l->data!= NULL; l = l->next) {
				gui_cell* cell = l->data;	
				_gui_style_drawcell(cell, style, t, dt);
			}
		} break;
		case GUI_CELL_BUTTON: {
			if (style->button != NULL) {
				glPushMatrix();
				vec2_translate(gc->object.button->pos);
				style->button(style, gc->object.button, t, dt);
				glPopMatrix();
			}
		} break;
	}
	glPopMatrix();
}

void
gui_style_draw
(
	gui_style* style,
	gui* g,
	double t,
	double dt
)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	vec2_translate(g->pos);
	
	if (style->gui != NULL) style->gui(style, g, t, dt);
	
	gui_window* gw;
	for(set_begin(g->activewindows, (SETDATA**)(&gw)); set_end(g->activewindows); set_next(g->activewindows, (SETDATA**)(&gw))) {
		glPushMatrix();
		vec2_translate(gw->pos);
		
		if (style->window != NULL) style->window(style, gw, t, dt);
		
		_gui_style_drawcell(gw->cell, style, t, dt);
		
		glPopMatrix();
	}
	
	glPopMatrix();
}