typedef enum
_E_gui_cell_content
{
	GUI_CELL_EMPTY,
	GUI_CELL_HORIZONTALCELLS,
	GUI_CELL_VERTICALCELLS
} gui_cell_content;

typedef struct
_S_gui_cell
{
	vec2* pos;
	_gui_dimension* dim;
	gui_cell_content content;
	list* cells;
	#ifdef GUI_DEBUGDRAWGUI
	color* debugcolor;
	#endif
} gui_cell;

gui_cell*
gui_cell_init
(
)
{
	gui_cell* gc = malloc(sizeof(gui_cell));
	gc->pos = vec2_new(0.0, 0.0);
	gc->dim = _gui_dimension_init(1.0, 1.0);
	gc->content = GUI_CELL_EMPTY;
	gc->cells = NULL;
	#ifdef GUI_DEBUGCOLORS
	gc->debugcolor = color_new4(0.0, 1.0, 0.0, 1.0);
	#endif
	return gc;
}

void
gui_cell_update
(
	gui_cell* gc,
	double t,
	double dt
)
{

}

void
gui_cell_draw
(
	gui_cell* gc,
	double t,
	double dt
)
{
	if (gc == NULL) return;
	glPushMatrix();
	vec2_translate(gc->pos);
	
	switch (gc->content) {
		case GUI_CELL_EMPTY: break;
		case GUI_CELL_HORIZONTALCELLS: {
			list* l = gc->cells;
			gui_cell_draw(l->data, t, dt);
			for(l = l->next; l!= NULL; l = l->next) {
				gui_cell* cell = l->data;	
				gui_cell_draw(cell, t, dt);
				
				#ifdef GUI_DEBUGDRAWGUI
				glBegin(GL_LINES);
				color_apply(gc->debugcolor);
				glVertex2d(cell->pos->x, 0.0);
				glVertex2d(cell->pos->x, gc->dim->h);
				glEnd();
				#endif
			}
		} break;
		case GUI_CELL_VERTICALCELLS: {
			list* l = gc->cells;
			gui_cell_draw(l->data, t, dt);
			for(l = l->next; l->data!= NULL; l = l->next) {
				gui_cell* cell = l->data;	
				gui_cell_draw(cell, t, dt);
				
				#ifdef GUI_DEBUGDRAWGUI
				glBegin(GL_LINES);
				color_apply(gc->debugcolor);
				glVertex2d(0.0,cell->pos->y);
				glVertex2d(gc->dim->w, cell->pos->y);
				glEnd();
				#endif
			}
		} break;
	}
	glPopMatrix();
}

void
gui_cell_addcell
(
	gui_cell* cell,
	gui_cell* toadd,
	gui_cell_content orientation
)
{
	if (cell->content == GUI_CELL_EMPTY) {
		cell->content = orientation;
		cell->cells = list_new();
	}
	list_add(cell->cells, toadd);
}

void
gui_cell_move
(
	gui_cell* cell,
	vec2* step
)
{	
	vec4_addto(cell->pos, step);
	vec2_print(cell->pos);
}

void
gui_cell_resize
(
	gui_cell* gc,
	double w,
	double h
)
{
	double sw = w/gc->dim->w;
	double sh = h/gc->dim->h;
	switch(gc->content) {
		case GUI_CELL_EMPTY: break;
		case GUI_CELL_VERTICALCELLS:
		case GUI_CELL_HORIZONTALCELLS:{
			for (list* l = gc->cells; l->data != NULL; l = l->next) {
				gui_cell* cell = (gui_cell*)(l->data);
				gui_cell_resize(cell, cell->dim->w*sw, cell->dim->h*sh);
			}
		} break;
	}
	gc->dim->w = w;
	gc->dim->h = h;
}
