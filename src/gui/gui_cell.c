typedef enum
_E_gui_cell_content
{
	GUI_CELL_EMPTY,
	GUI_CELL_HORIZONTALCELLS,
	GUI_CELL_VERTICALCELLS
} gui_cell_content;

const char* gui_cell_contentstrs[] =
{
	"EMPTY",
	"HORIZONTAL CELLS",
	"VERTICAL CELLS"
};

typedef struct
_S_gui_cell
{
	vec2* pos;
	_gui_dimension* dim;
	gui_cell_content content;
	list* cells;
	list* partitions;
	#ifdef GUI_DEBUGDRAWGUI
	color* debugcolor;
	#endif
} gui_cell;

gui_cell*
gui_cell_init
()
{
	gui_cell* gc = malloc(sizeof(gui_cell));
	gc->pos = vec2_new(0.0, 0.0);
	gc->dim = _gui_dimension_init(1.0, 1.0);
	gc->content = GUI_CELL_EMPTY;
	gc->partitions = NULL;
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
			for(l = l->next; l->data!= NULL; l = l->next) {
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
	gui_cell_content orientation,
	double* partition
)
{
	if (cell->content == GUI_CELL_EMPTY) {
		cell->content = orientation;
		cell->cells = list_new();
		cell->partitions = list_new();
	}
	if (orientation == GUI_CELL_HORIZONTALCELLS) {
		toadd->dim->w = cell->dim->w * *partition;
		toadd->dim->h = cell->dim->h;
	} else 
	if (orientation == GUI_CELL_VERTICALCELLS) {
		toadd->dim->w = cell->dim->w;
		toadd->dim->h = cell->dim->h * *partition;
	}
	list_add(cell->cells, toadd);
	list_add(cell->partitions, partition);
}

void
gui_cell_move
(
	gui_cell* cell,
	vec2* step
)
{	
	vec4_addto(cell->pos, step);
}

void
gui_cell_adjustpartitions
(
	gui_cell* gc
)
{
	// if(gc->content) return;
	
	// double total = 0.0;
	// for (list* l = gc->cells; l->data != NULL; l = l->next) {
		// gui_cell* cell = (gui_cell*)(l->data);
		// total += cell->partition;
	// }
	
	// if (total < 1.0) return;
	
	// for (list* l = gc->cells; l->data != NULL; l = l->next) {
		// gui_cell* cell = (gui_cell*)(l->data);
		// cell->partition /= total;
	// }
}

void
gui_cell_resize
(
	gui_cell* gc,
	double w,
	double h
)
{
	switch(gc->content) {
		case GUI_CELL_EMPTY: break;
		case GUI_CELL_VERTICALCELLS:{
			list *p = gc->partitions;
			double pos = 0.0;
			for (list* l = gc->cells; l->data != NULL; l = l->next) {
				gui_cell* cell = (gui_cell*)(l->data);
				double* len = (double*)(p->data);
				gui_cell_resize(cell, w, h*(*len));
				cell->pos->y = pos * h;
				p = p->next;
				pos = *len;
			}
		} break;
		case GUI_CELL_HORIZONTALCELLS:{
			list *p = gc->partitions;
			double pos = 0.0;
			for (list* l = gc->cells; l->data != NULL; l = l->next) {
				gui_cell* cell = (gui_cell*)(l->data);
				double* len = (double*)(p->data);
				gui_cell_resize(cell, w*(*len), h);
				cell->pos->x = pos * w;
				p = p->next;
				pos = *len;
			}
		} break;
	}
	gc->dim->w = w;
	gc->dim->h = h;
}

void
gui_cell_print
(
	gui_cell* gc
)
{
	printf("CELL:\n============================\n");
	printf("\tPOS:\t");
	vec2_print(gc->pos);
	printf("\tDIM:\t");
	_gui_dimension_print(gc->dim);
	printf("\tCONTENT:\t%s\n", gui_cell_contentstrs[gc->content]);
	if(gc->content != GUI_CELL_EMPTY) {
		printf("\tCELLS:\n");
		list* p = gc->partitions;
		for(list* l = gc->cells; l->data != NULL; l = l->next) {
			printf("[%f] ", p->data);
			gui_cell_print(l->data);
			p = p->next;
		}
	}
	printf("============================\n");
}