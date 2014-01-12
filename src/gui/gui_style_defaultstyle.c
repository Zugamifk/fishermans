#define _GUI_STYLE_FONTSIZE 12

typedef enum
_gui_style_colorid_e
{
	GUISTYLE_GUI,
	GUISTYLE_WINDOW,
	GUISTYLE_CELL,
	GUISTYLE_BOX,
	GUISTYLE_WTF,
	GUISTYLE_COLOR_COUNT
} gui_style_colorid;

static 
color*
_gui_style_encodecolor
(
	unsigned int id
)
{
	double fac = (double)id;
	double den = (double)GUISTYLE_COLOR_COUNT;
	color* c = color_new4(fac/den, 0.0, 0.0, 1.0);
	return c;
}

color**
_gui_style_gencolors
(
	int* len
)
{
	*len = GUISTYLE_COLOR_COUNT;
	color** pack = malloc(sizeof(color*)*GUISTYLE_COLOR_COUNT);
	pack[GUISTYLE_GUI] =  _gui_style_encodecolor(GUISTYLE_GUI);//gui
	pack[GUISTYLE_WINDOW] = _gui_style_encodecolor(GUISTYLE_WINDOW); //window
	pack[GUISTYLE_CELL] = _gui_style_encodecolor(GUISTYLE_CELL); //cell
	pack[GUISTYLE_BOX] = _gui_style_encodecolor(GUISTYLE_BOX); //button
	pack[GUISTYLE_WTF] = _gui_style_encodecolor(GUISTYLE_WTF); //button
	return pack;
}

void
_gui_style_textcb(gui_style* style, gui_text* gt, double t, double dt) {
	double x = gt->pos->x;
	double y = gt->pos->y;
	font_draw(gt->text, style->font, x, y, _GUI_STYLE_FONTSIZE, gt->width);
}

void
_gui_style_guicb(gui_style* style, gui* g, double t, double dt) {
	color* c = gui_style_getcolor(style, GUISTYLE_GUI);
	if (g->state == GUI_STATE_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	
	shapes_boxfilled(g->dim->w, g->dim->h);
}

void
_gui_style_windowcb(gui_style* style, gui_window* gw, double t, double dt) {

	font_draw(gw->name, style->font, 10.0, 10.0, _GUI_STYLE_FONTSIZE, FONT_LEN_INF);

	color* c = gui_style_getcolor(style, GUISTYLE_WINDOW);
	if (gw->state == GUI_WINDOW_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	
	shapes_boxfilled(gw->dim->w, gw->dim->h);
}

void
_gui_style_cellcb(gui_style* style, gui_cell* gc, double t, double dt) {
	if (gc->content == GUI_CELL_TEXT) {
		style->text(style, gc->object.text, t, dt);
	}
	
	color* c = gui_style_getcolor(style, GUISTYLE_CELL);
	if (gc->state == GUI_CELL_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}

	shapes_boxfilled(gc->dim->w, gc->dim->h);

}

void
_gui_style_buttoncb(gui_style* style, gui_button* gb, double t, double dt) {
	style->text(style, gb->text, t, dt); 
	 
	color* c = gui_style_getcolor(style, GUISTYLE_BOX);
	if (gb->state == GUI_BUTTON_HOVER) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	shapes_boxfilled(gb->dim->w, gb->dim->h);
}

void
_gui_style_slidercb(gui_style* style, gui_slider* gs, double t, double dt) {	 
	color* c = gui_style_getcolor(style, GUISTYLE_BOX);
	color_apply(c);
	double of = gs->bounds->dim->h/2.0;
	double sqr = gs->bounds->dim->w*0.2;
	double hsqr = sqr/2.0;
	glBegin(GL_LINES);
	glVertex2d(hsqr, of);
	glVertex2d(gs->bounds->dim->w-hsqr, of);
	glEnd();
	glTranslated(*(gs->value)*0.8*gs->bounds->dim->w, of-hsqr, 0.0);
	shapes_boxfilled(sqr, sqr);
}

void
_gui_style_textincb(gui_style* style, gui_textin* gt, double t, double dt) {
	style->text(style, gt->text, t, dt); 

	color* c = gui_style_getcolor(style, GUISTYLE_BOX);
	color_apply(c);
	_gui_box_defdraw(gt->bounds);
}

void
_gui_style_viewportcb(gui_style* style, gui_viewport* gv, double t, double dt) {
	color* c = gui_style_getcolor(style, GUISTYLE_WTF);
	float w = gv->dim->w;
	float h = gv->dim->h;
	
	float tw = 1.0;
	float th = 1.0;
	color_apply(c);

	glEnable(GL_TEXTURE_2D); 
		
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2d(0.0, 0.0); 
	
	glTexCoord2f(tw, 0.0);
	glVertex2d(w, 0.0);
	
	glTexCoord2f(tw,th);
	glVertex2d(w, h);
	
	glTexCoord2f(0.0, th);
	glVertex2d(0.0, h);
	
	glEnd();
	glDisable(GL_TEXTURE_2D); 
}

gui_style*
gui_style_initdefault
(
	void
)
{
	gui_style* gs = gui_style_init();
	
	int len;
	color** carr = _gui_style_gencolors(&len);
	gui_style_setpalette(gs, carr, len);
	
	fontinfo* guifont = font_init("data/extremeradcool", color_new3(0.0, 0.0, 0.0));
	gui_style_setfont(gs, guifont);
	
	gs->gui = _gui_style_guicb;
	gs->window = _gui_style_windowcb;
	gs->cell = _gui_style_cellcb;
	gs->button = _gui_style_buttoncb;
	gs->text = _gui_style_textcb;
	gs->viewport = _gui_style_viewportcb;
	gs->slider = _gui_style_slidercb;
	gs->textin = _gui_style_textincb;
	
	return gs;
}