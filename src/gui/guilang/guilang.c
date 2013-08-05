// GUILANG
// ========================================================================= //
/*	This file specifies the first version of GUILANG. The file specifying the
	grammar had better (!!!!) match the expressions specified at the head of
	each of the functions in this file.
	Expressions should be read literally, besides the following types:
	$WORD	References another GUI object generated by a different function.
	WORD*	Kleene star, 0 or more copies of WORD.
	[WORD*]	A list of possible optional terms
	#		Any number.
*/

//Error log
// ========================================================================= //]
// This should never have to be called! Parser should do work correctly!!!!
void
guilang_error
(
	guilang_processor* processor,
	const char* message,
	const char* word
)
{
	char errorstring[LANG_LINELEN]; 
	sprintf(errorstring, message, word);
	errorlog_logdef(processor->log, "GUILANG TRANSLATOR", errorstring);
}

// CELL
// ========================================================================= //
// CELL{ [ ( [horizontal, vertical] [,#]* ) ] $CELL* }
gui_cell*
guilang_buildcell
(
	guilang_processor* processor,
	gui_window* gw
)
{
	if (strcmp(processor->current, "CELL") != 0) {
		return NULL;
	}
	
	guilang_processor_match(processor, "CELL");
	guilang_processor_match(processor, "{");
	
	char* curr = guilang_processor_consume(processor);
	gui_cell* gc = gui_cell_init();;
	if(curr[0] == '(') {
		curr = guilang_processor_consume(processor);
		gui_cell_content orientation;
		if (strcmp(curr, "horizontal") == 0) {
			orientation = GUI_CELL_HORIZONTALCELLS;
		} else
		if (strcmp(curr, "vertical") == 0) {
			orientation = GUI_CELL_VERTICALCELLS;
		} else {
			guilang_error(processor, "Bad Parameter! CELL expects an orientation! Got \'%s\'", curr);
		}
		guilang_processor_match(processor, ",");
		list* positions = list_new();
		do {
			curr = guilang_processor_consume(processor);
			double* pos = malloc(sizeof(double));
			*pos = strtod(curr, NULL);
			list_add(positions, pos);
			curr = guilang_processor_consume(processor);
		} while (curr[0] != ')');
		
		
		gui_cell* cell = guilang_buildcell(processor, gw);
		gui_cell_addcell(gc, cell, orientation);
		vec2* pos = vec2_new(0.0, 0.0);
		for(list* l = positions; l->data!=NULL; l = l->next) {
			gui_cell* cell = guilang_buildcell(processor, gw);
			double* step = (double*)(l->data);
			if (orientation == GUI_CELL_HORIZONTALCELLS) { 
				pos->x = (*step)*gw->dim->w;
			} else {
				pos->y = (*step)*gw->dim->h;
			}
			gui_cell_move(cell, pos);
			gui_cell_addcell(gc, cell, orientation);
		}
		guilang_processor_match(processor, "}");
		list_deepdelete(positions, free);
	} else
	if(curr[0] == '}') {
	}
	
	return gc;
}

// WINDOW
// ========================================================================= //
// WINDOW[([[x, y, w, h]:#]* )]{ [$CELL] }
gui_window*
guilang_buildwindow
(
	guilang_processor* processor,
	gui* g
)
{
	if (strcmp(processor->current, "WINDOW") != 0) return NULL;
	float x = 0.0;
	float y = 0.0;
	float w = 1.0;
	float h = 1.0;
	
	guilang_processor_match(processor, "WINDOW");
	char* curr = guilang_processor_consume(processor);
	
	if (curr[0] == '(') {
		float* param;
		do {
			curr = guilang_processor_consume(processor);
			switch(curr[0]) {
				case 'x': param = &x; break;
				case 'y': param = &y; break;
				case 'w': param = &w; break;
				case 'h': param = &h; break;
				default: guilang_error(processor, "Bad parameter! WINDOW object does not accept \'%s\'!", curr);
			}
			guilang_processor_match(processor, ":");
			curr = guilang_processor_consume(processor);
			*param = strtod(curr, NULL);
			curr = guilang_processor_consume(processor);
		} while(curr[0] != ')');
	}
	
	x *= g->dim->w;
	y *= g->dim->h;
	w *= g->dim->w;
	h *= g->dim->h;
	gui_window* gw = gui_window_init(x, y, w, h);
	
	guilang_processor_match(processor, "{");
	if (strcmp(processor->current, "CELL") == 0) {
		gui_cell* cell = guilang_buildcell(processor, gw);
		gui_window_setcell(gw, cell);
	} else
	if (processor->current[0] == '}') {
		guilang_processor_match(processor, "}");
	} else {
		guilang_error(processor, "Window syntax error! Badly formed window sOemHOW!! the bird is \'%s\'", curr);
	}
	
	
	return gw;
}

// GUI
// ========================================================================= //
// GUI[( [[x, y, w, h]:#]* )]{ $WINDOW* }
gui*
guilang_buildgui
(
	char** stream,
	errorlog* log,
	event_bus* bus,
	screeninfo* info
)
{
	float x = 0.0;
	float y = 0.0;
	float w = 1.0;
	float h = 1.0;

	guilang_processor* processor = guilang_processor_init(stream, log, bus);
	
	guilang_processor_match(processor, "GUI");
	
	char* curr = guilang_processor_consume(processor);
	
	if (curr[0] == '(') {
		float* param;
		do {
			curr = guilang_processor_consume(processor);
			switch(curr[0]) {
				case 'x': param = &x; break;
				case 'y': param = &y; break;
				case 'w': param = &w; break;
				case 'h': param = &h; break;
				default: guilang_error(processor, "Bad parameter! GUI object does not accept \'%s\'!", curr);
			}
			guilang_processor_match(processor, ":");
			curr = guilang_processor_consume(processor);
			*param = strtod(curr, NULL);
			curr = guilang_processor_consume(processor);
		} while(curr[0] != ')');
	}
	
	x *= info->w;
	y *= info->h;
	w *= info->w;
	h *= info->h;
	gui* g = gui_init(x, y, w, h);
	gui_window* gw = guilang_buildwindow(processor, g);
	while(gw != NULL) {
		gui_openwindow(g, gw);
		gw = guilang_buildwindow(processor, g);
	}
		
	gui_print(g);
	return g;
}