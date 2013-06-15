typedef enum
e_gui_cell_content
{
	GUI_CELL_BRANCH,
	GUI_CELL_OBJECTS
} gui_cell_content;

typedef enum
e_gui_cell_ornt
{
	GUI_CELL_NOORNT,
	GUI_CELL_HORZ,
	GUI_CELL_VERT
} gui_cell_ornt;

typedef struct
t_gui_cell
{
	gui_cell_content content;
	gui_drawcallback drawcb;
	int		cellnum;
	struct t_gui_cell** cells;
	float*	partitions;
	gui_cell_ornt orientation;
	gui_object** objects;
	int		objectnum;
} gui_cell;

gui_cell*
gui_cell_init
(

)
{
	gui_cell* cell = malloc(sizeof(gui_cell));
	
	cell->content 	= GUI_CELL_OBJECTS;
	cell->drawcb	= gui_drawdefault;
	cell->cellnum 	= 0;
	cell->cells		= NULL;
	cell->partitions = NULL;
	cell->orientation=GUI_CELL_NOORNT;
	cell->objects	= NULL;
	cell->objectnum = 0;
	
	return cell;
}

void
gui_cell_draw
(
	gui_cell*	cell,
	float t, 
	float dt
)
{
	switch(cell->content)
	{
		case GUI_CELL_BRANCH:
		{
			float cpos = 0, dpos = 0;
			float *x, *y;

			if (cell->orientation == GUI_CELL_VERT) {
				x = &dpos;
				y = &cpos;
			} else 
			if (cell->orientation == GUI_CELL_HORZ) {
				x = &cpos;
				y = &dpos;
			}
			
			for (int i = 0; i < cell->cellnum; i++) {
				glTranslatef(*x, *y, 0);
				
				glPushMatrix();
				gui_cell_draw(cell->cells[i], t, dt);
				glPopMatrix();
				
				dpos += cell->partitions[i];
			}
		} break;
		case GUI_CELL_OBJECTS:
		{
		
		} break;
	}
}