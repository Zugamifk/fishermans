#define SPRITE_GUISTYLE_FONTSIZE 12

void
Sprite_guistyle_textcb(gui_style* style, gui_text* gt, double t, double dt) {
	double x = gt->pos->x;
	double y = gt->pos->y;
	font_draw(gt->text, style->font, x, y, SPRITE_GUISTYLE_FONTSIZE, gt->width);
}

void
Sprite_guistyle_guicb(gui_style* style, gui* g, double t, double dt) {
	color* c = gui_style_getcolor(style, SPRITESHADER_GUIID);
	if (g->state == GUI_STATE_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	
	shapes_boxfilled(g->dim->w, g->dim->h);
}

void
Sprite_guistyle_windowcb(gui_style* style, gui_window* gw, double t, double dt) {

	font_draw(gw->name, style->font, 10.0, 10.0, SPRITE_GUISTYLE_FONTSIZE, FONT_LEN_INF);

	color* c = gui_style_getcolor(style, SPRITESHADER_WINDOWID);
	if (gw->state == GUI_WINDOW_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	
	shapes_boxfilled(gw->dim->w, gw->dim->h);
}

void
Sprite_guistyle_cellcb(gui_style* style, gui_cell* gc, double t, double dt) {
	if (gc->content == GUI_CELL_TEXT) {
		style->text(style, gc->object.text, t, dt);
	}
	
	color* c = gui_style_getcolor(style, SPRITESHADER_CELLID);
	if (gc->state == GUI_CELL_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}

	shapes_boxfilled(gc->dim->w, gc->dim->h);

}

void
Sprite_guistyle_buttoncb(gui_style* style, gui_button* gb, double t, double dt) {
	style->text(style, gb->text, t, dt); 
	 
	color* c = gui_style_getcolor(style, SPRITESHADER_BUTTONID);
	if (gb->state == GUI_BUTTON_HOVER) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	shapes_boxfilled(gb->dim->w, gb->dim->h);
}

void
Sprite_guistyle_viewportcb(gui_style* style, gui_viewport* gb, double t, double dt) {
	color* c = gui_style_getcolor(style, SPRITESHADER_PARTICLEID);
	if (gb->state == GUI_VIEWPORT_HOVER) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	shapes_boxfilled(gb->dim->w, gb->dim->h);
}

gui_style*
Sprite_guistyle_init
(
	void
)
{
	gui_style* gs = gui_style_init();
	
	int len;
	color** carr = Spriteshader_gencolors(&len);
	gui_style_setpalette(gs, carr, len);
	
	fontinfo* guifont = font_init("data/extremeradcool", color_new3(0.0, 0.0, 0.0));
	gui_style_setfont(gs, guifont);
	
	gs->gui = Sprite_guistyle_guicb;
	gs->window = Sprite_guistyle_windowcb;
	gs->cell = Sprite_guistyle_cellcb;
	gs->button = Sprite_guistyle_buttoncb;
	gs->text = Sprite_guistyle_textcb;
	gs->viewport = Sprite_guistyle_viewportcb;
	
	return gs;
}