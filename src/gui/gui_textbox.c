gui_textbox	*	
gui_textbox_init(char *text, fontinfo *fi, double scale, double w, double h)
{
	gui_textbox *tb 	= malloc(sizeof(gui_textbox));
	tb->bounds	= gui_box_init(0, 0, w, h);
	tb->font 	= fi;
	tb->text 	= text;
	tb->scale 	= scale;
	tb->scalarx = 0.0;
	tb->scalary = 0.0;
	return tb;
}

void
gui_textbox_draw(gui_textbox *tb, double t, double dt)
{
	gui_box *box = tb->bounds;
	double x = box->x + box->scale*box->hpadin;
	double y = box->y + box->scale*(box->height-box->vpadin-tb->scale);
	glPushMatrix();	
		glTranslated(x, y, 0.0);
		glScaled(tb->scale*box->scale, tb->scale*box->scale, 1.0);
		color_apply(gui_colors->colors[2]);
		font_draw(tb->text, tb->font, 0.0, 0.0, 1.0, (box->width-2.0*box->hpadin)/tb->scale);
	glPopMatrix();
	
	gui_box_draw(box, GL_LINE_LOOP, gui_colors->colors[1]);
	
	color *col = gui_colors->colors[3];
	gui_box_draw(box, GL_QUADS, col);
}