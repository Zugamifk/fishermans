void
gui_debugstyle_guicb(gui_style* style, gui* g, double t, double dt) {
	color* c = gui_style_getcolor(style, 0);
	if (g->state == GUI_STATE_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	shapes_box(g->dim->w, g->dim->h);
}

void
gui_debugstyle_windowcb(gui_style* style, gui_window* gw, double t, double dt) {
	color* c = gui_style_getcolor(style, 1);
	if (gw->state == GUI_WINDOW_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	shapes_box(gw->dim->w, gw->dim->h);
}

void
gui_debugstyle_cellcb(gui_style* style, gui_cell* gc, double t, double dt) {
	color* c = gui_style_getcolor(style, 2);
	if (gc->state == GUI_CELL_CONTAINSMOUSE) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	shapes_box(gc->dim->w, gc->dim->h);
}

void
gui_debugstyle_buttoncb(gui_style* style, gui_button* gb, double t, double dt) {
	color* c = gui_style_getcolor(style, 3);
	if (gb->state == GUI_BUTTON_HOVER) {
		color_applyinverse(c);
	} else {
		color_apply(c);
	}
	shapes_box(gb->dim->w, gb->dim->h);
}

gui_style*
gui_debugstyle_init
(
	void
)
{
	gui_style* gs = gui_style_init();
	
	int len = 4;
	color** carr = malloc(sizeof(color*)*len);
	carr[0] = color_new4(1.0, 0.0, 0.0, 1.0); //gui
	carr[1] = color_new4(0.0, 0.0, 1.0, 1.0); //window
	carr[2] = color_new4(0.0, 1.0, 0.0, 1.0); //cell
	carr[3] = color_new4(0.0, 0.6, 0.3, 1.0); //button
	gui_style_setpalette(gs, carr, len);
	
	fontinfo* guifont = font_initdef();
	gui_style_setfont(gs, guifont);
	
	gs->gui = gui_debugstyle_guicb;
	gs->window = gui_debugstyle_windowcb;
	gs->cell = gui_debugstyle_cellcb;
	gs->button = gui_debugstyle_buttoncb;
	
	return gs;
}