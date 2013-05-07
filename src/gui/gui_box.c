// ==============================================
// GUI BUOX

gui_box *
gui_box_init(double x, double y, double w, double h)
{
	gui_box *b = malloc(sizeof(gui_box));
	b->scale	= 1.0;
	b->x 		= x;
	b->y 		= y;
	b->hpadin	= 0.02;
	b->vpadin	= 0.02;
	b->hpadout	= 0.02;
	b->vpadout	= 0.02;
	b->width 	= w+b->hpadin*2.0;
	b->height	= h+b->vpadin*2.0;
	b->visible 	= 1;
	return b;
}

void
gui_box_position(gui_box *b, gui_container *c, double sx, double sy)
{ 
	double rx 	= sx * c->width;
	double ry 	= sy * c->height;
	double lft	= rx - b->scale*b->hpadout;
	double rgt	= rx + b->scale * (b->width + b->hpadout);
	double bot	= ry - b->scale*b->vpadout;
	double top	= ry + b->scale * (b->height + b->vpadout);	
	
	double wr = c->width/(rgt-lft);
	double hr = c->height/(top-bot);
	double rs = wr - hr;
	
	if (rs < 0.0) {
		b->scale = b->scale*c->width/(rgt-lft);
	} else {
		b->scale = b->scale*c->height/(top-bot);
	}
	
	if (lft+GUI_EPSILON < 0.0) {
		double error = -lft;
		double offsetx = error/c->width;
		return gui_box_position(b, c, sx+offsetx, sy);
	} else 
	if (rgt-GUI_EPSILON > c->width)  {	
	 	double error = rgt - c->width;
		double offsetx = error/c->width;
		return gui_box_position(b, c, sx-offsetx, sy);
	}
	if (bot+GUI_EPSILON < 0.0) {
		double error = -bot;
		double offsety = error/c->height;
		return gui_box_position(b, c, sx, sy+offsety);
	} else 
	if (top-GUI_EPSILON > c->height)  {
		double error = top - c->height;
		double offsety = error/c->height;
		return gui_box_position(b, c, sx, sy-offsety);
	}
	
	if (b->scale > 1.0) b->scale = 1.0;
	if (b->scale < 0.0) b->scale = 0.0;
	
	b->x = rx;
	b->y = ry;
}

void
gui_box_translateinto
(gui_box *box)
{
	glTranslated(box->x+box->scale*box->hpadin, box->y+box->scale*box->vpadin, 0.0);
}

int
gui_box_inbox(gui_box *b, double x, double y)
{
	return 	x > b->x
		&&	x < b->x + b->scale*b->width
		&&	y > b->y
		&&	y < b->y + b->scale*b->height;
}

void
gui_box_getscalars(gui_box *b, double *sx, double *sy, double x, double y)
{
	if (x < b->x) 
		*sx = 0.0;
	else if (x > b->x + b->scale*b->width)
		*sx = 1.0;
	else
		*sx = (x - b->x) / (b->scale*b->width);
		
	if (y < b->y) 
		*sy = 0.0;
	else if (y > b->y + b->scale*b->height)
		*sy = 1.0;
	else
		*sy = (y - b->y) / (b->scale*b->height);
} 

void
gui_box_draw(gui_box *box, int style, color *c)
{
	if (!box->visible) return;
	
	double lft	= box->x;
	double rgt	= box->x + box->scale*box->width;
	double bot	= box->y;
	double top	= box->y + box->scale*box->height;
	
	color_apply(c);
	glBegin(style);
	glVertex2d(lft, top);
	glVertex2d(rgt, top);
	glVertex2d(rgt, bot);
	glVertex2d(lft, bot);
	glEnd();
}