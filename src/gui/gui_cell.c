typedef enum
_E_gui_cell_state
{
	GUI_CELL_NONE,
	GUI_CELL_CONTAINSMOUSE
} gui_cell_state;

typedef enum
_E_gui_cell_content
{
	GUI_CELL_EMPTY,
	GUI_CELL_HORIZONTALCELLS,
	GUI_CELL_VERTICALCELLS,
	GUI_CELL_BUTTON,
	GUI_CELL_TEXT
} gui_cell_content;

const char* gui_cell_contentstrs[] =
{
	"EMPTY",
	"HORIZONTAL CELLS",
	"VERTICAL CELLS",
	"BUTTON",
	"TEXT"
};

typedef union
_U_gui_cell_object
{
	gui_button* button;
	gui_text* text;
} _gui_cell_object;

typedef struct
_S_gui_cell
{
	gui_cell_state state;
	vec2* pos;
	_gui_dimension* dim;
	gui_cell_content content;
	list* cells;
	list* partitions;
	_gui_cell_object object;
	void (*clickcb)(struct _S_gui_cell*);
} gui_cell;

gui_cell*
gui_cell_init
()
{
	gui_cell* gc = malloc(sizeof(gui_cell));
	gc->state = GUI_CELL_NONE;
	gc->pos = vec2_new(0.0, 0.0);
	gc->dim = _gui_dimension_init(1.0, 1.0);
	gc->content = GUI_CELL_EMPTY;
	gc->partitions = NULL;
	gc->cells = NULL;
	gc->clickcb = NULL;
	return gc;
}

void
gui_cell_reset
(
	gui_cell* gc
)
{
	gc->state = GUI_CELL_NONE;
	switch(gc->content) {
		case GUI_CELL_EMPTY: break;
		case GUI_CELL_HORIZONTALCELLS:
		case GUI_CELL_VERTICALCELLS: {
			for (list *l = gc->cells; l->data != NULL; l = l->next) {
				gui_cell_reset(l->data);
			}
		}break;
		case GUI_CELL_BUTTON: {
			gui_button_reset(gc->object.button);
		} break;
		case GUI_CELL_TEXT: break;
	}
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

bool
gui_cell_contains
(
	gui_cell* gc,
	double x,
	double y
)
{
	return (gc->pos->x < x &&
			gc->pos->y < y &&
			x < gc->pos->x + gc->dim->w &&
			y < gc->pos->y + gc->dim->h);
}

void
gui_cell_mouseupdate
(
	gui_cell* gc,
	int x,
	int y
)
{
	if (gui_cell_contains(gc, x, y)) {
		gc->state = GUI_CELL_CONTAINSMOUSE;
		double rx = x - gc->pos->x;
		double ry = y - gc->pos->y;
		switch(gc->content) {
			case GUI_CELL_EMPTY: break;
			case GUI_CELL_HORIZONTALCELLS:
			case GUI_CELL_VERTICALCELLS: {
				for (list *l = gc->cells; l->data != NULL; l = l->next) {
					gui_cell_mouseupdate(l->data, rx, ry);
				}
			}break;
			case GUI_CELL_BUTTON: {
				gui_button_mouseupdate(gc->object.button, rx, ry);
			} break;
			case GUI_CELL_TEXT: break;
		}
	}
}

void
gui_cell_click
(
	gui_cell* gc,
	event_bus* bus
)
{
	if (gc->state == GUI_CELL_CONTAINSMOUSE) {
		if (gc->clickcb != NULL) gc->clickcb(gc);
		
		switch(gc->content) {
			case GUI_CELL_EMPTY: break;
			case GUI_CELL_HORIZONTALCELLS:
			case GUI_CELL_VERTICALCELLS: {
				for (list *l = gc->cells; l->data != NULL; l = l->next) {
					gui_cell_click(l->data, bus);
				}
			}break;
			case GUI_CELL_BUTTON: {
				gui_button_click(gc->object.button, bus);
			} break;
			case GUI_CELL_TEXT: break;
		}
	}
}

void
gui_cell_resize
(
	gui_cell* gc,
	double w,
	double h
)
{
	_gui_dimension_resize(gc->dim, w, h);

	switch(gc->content) {
		case GUI_CELL_EMPTY: break;
		case GUI_CELL_VERTICALCELLS:{
			list *p = gc->partitions;
			double pos = 0.0;
			for (list* l = gc->cells; l->data != NULL; l = l->next) {
				gui_cell* cell = (gui_cell*)(l->data);
				double* len = (double*)(p->data);
				gui_cell_resize(cell, gc->dim->w, gc->dim->h);
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
				gui_cell_resize(cell, gc->dim->w, gc->dim->h);
				cell->pos->x = pos * w;
				p = p->next;
				pos = *len;
			}
		} break;
		case GUI_CELL_BUTTON: {
			gui_button_resize(gc->object.button, gc->dim->w, gc->dim->h);
		} break;
		case GUI_CELL_TEXT: {
			gui_text* gt = gc->object.text;
			gui_text_resize(gt, gc->dim->w, gc->dim->h);
			gt->pos->y = gc->dim->h - gt->size - 10.0;
			gt->pos->x = 10.0;
		} break;
	}
}

void
gui_cell_addcell
(
	gui_cell* cell,
	gui_cell* toadd,
	gui_cell_content orientation,
	double* partition,
	double size
)
{
	if (cell->content == GUI_CELL_EMPTY) {
		cell->content = orientation;
		cell->cells = list_new();
		cell->partitions = list_new();
	}
	
	if (orientation == GUI_CELL_HORIZONTALCELLS) {
		toadd->dim->sw = size;
		toadd->dim->w = cell->dim->w * size;
		toadd->dim->h = cell->dim->h;
	} else 
	if (orientation == GUI_CELL_VERTICALCELLS) {
		toadd->dim->sh = size;
		toadd->dim->w = cell->dim->w;
		toadd->dim->h = cell->dim->h * size;
	}
	list_add(cell->cells, toadd);
	list_add(cell->partitions, partition);
}

void
gui_cell_addobject
(
	gui_cell* gc,
	void* o,
	gui_cell_content type
)
{
	switch(type) {
		case GUI_CELL_EMPTY:
		case GUI_CELL_HORIZONTALCELLS:
		case GUI_CELL_VERTICALCELLS:
		break;
		case GUI_CELL_BUTTON: {
			gc->content = type;
			gc->object.button = o;
		} break;
		case GUI_CELL_TEXT: {
			gc->content = type;
			gc->object.text = o;
		} break;
	}
}

void
gui_cell_move
(
	gui_cell* cell,
	double x, 
	double y
)
{	
	cell->pos->x += x;
	cell->pos->y += y;
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
	switch(gc->content) {
		case GUI_CELL_EMPTY: break;
		case GUI_CELL_HORIZONTALCELLS:
		case GUI_CELL_VERTICALCELLS:{
			printf("\tCELLS:\n");
			list* p = gc->partitions;
			for(list* l = gc->cells; l->data != NULL; l = l->next) {
				printf("[%f] ", p->data);
				gui_cell_print(l->data);
				p = p->next;
			}
		}
		break;
		case GUI_CELL_BUTTON: {
			gui_button_print(gc->object.button);
		} break;
		case GUI_CELL_TEXT: {
			gui_text_print(gc->object.text);
		}break;
	}
	printf("============================\n");
}