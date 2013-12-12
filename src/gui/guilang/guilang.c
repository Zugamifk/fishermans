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

//Error logging
// ========================================================================= //
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
	errorlog_logdef(processor->log, "GUILANG TRANSLATOR SYNTAX", errorstring);
}

// Semantic / Logic errors
// This is ok to call! It's still bad, but the translator is supposed to pick
// these up. These errors indicate your guilang code is crap. 
void
guilang_logicerror
(
	guilang_processor* processor,
	const char* message,
	const char* word
)
{
	char errorstring[LANG_LINELEN]; 
	sprintf(errorstring, message, word);
	errorlog_logdef(processor->log, "GUILANG TRANSLATOR LOGIC", errorstring);
}

// ========================================================================= //
// GUI HEADER INFO
// ========================================================================= //
void
guilang_readheader
(
	guilang_processor* processor
)
{
	char* curr;
	
	if (strcmp(processor->current, "event") == 0) {
		guilang_processor_match(processor, "event");
		guilang_processor_match(processor, ":");
		
		curr = guilang_processor_consume(processor);
		while (strcmp(processor->current, ",") == 0) {
			if (!bus_eventnameexists(processor->bus, curr)) {
				guilang_logicerror(processor, "Externally defined event \"%s\" does not exist!\n", curr);
			}
			guilang_processor_match(processor, ",");
			curr = guilang_processor_consume(processor);
		}
		if (!bus_eventnameexists(processor->bus, curr)) {
			guilang_logicerror(processor, "Externally defined event \"%s\" does not exist!\n", curr);
		}
	} else
	if (strcmp(processor->current, "var") == 0) {
		guilang_processor_match(processor, "var");
		guilang_processor_match(processor, ":");
		
		curr = guilang_processor_consume(processor);
		while (strcmp(processor->current, ",") == 0) {
			if (hashtable_get(processor->vars, curr) == NULL) {
				guilang_logicerror(processor, "Externally defined variable \"%s\" does not exist!\n", curr);
			}
			guilang_processor_match(processor, ",");
			curr = guilang_processor_consume(processor);
		}
		if (hashtable_get(processor->vars, curr) == NULL) {
			guilang_logicerror(processor, "Externally defined variable \"%s\" does not exist!\n", curr);
		}
	} else return;
	guilang_readheader(processor);
}

// ========================================================================= //
// CORE GUI OBJECTS
// ========================================================================= //

// VIEWPORT
// ========================================================================= //
// VIEWPORT ( [ name ]:@)
gui_viewport*
guilang_buildviewport
(
	guilang_processor* processor,
	gui_cell* gc
)
{
	double x = 0.0;
	double y = 0.0;
	double w = 1.0;
	double h = 1.0;
	char* name = "VIEWPORT";
	
	guilang_processor_match(processor, "VIEWPORT");
	guilang_processor_match(processor, "(");
	char* curr;
	do {
		curr = guilang_processor_consume(processor);
		if (strcmp(curr, "name") == 0) {
			guilang_processor_match(processor, ":");
			name = guilang_processor_consume(processor);
		}
	} while(strcmp(guilang_processor_consume(processor), ",") == 0);
	
	gui_viewport* gv = gui_viewport_init(name, x, y, w, h);
	return gv;
}

// BUTTON
// ========================================================================= //
// BUTTON ( [ [ [x, y, w, h]:#, [text, name]:@ ] ]* )
gui_button*
guilang_buildbutton
(
	guilang_processor* processor,
	gui_cell* gc
)
{
	double x = 0.0;
	double y = 0.0;
	double w = 1.0;
	double h = 1.0;
	char* click = NULL;
	char* name = "BUTTON";
	char* text = "BUTTON";
	
	guilang_processor_match(processor, "BUTTON");
	guilang_processor_match(processor, "(");
	char* curr;
	do {
		curr = guilang_processor_consume(processor);
		if (strcmp(curr, "name") == 0) {
			guilang_processor_match(processor, ":");
			name = guilang_processor_consume(processor);
		} else 
		if (strcmp(curr, "text") == 0) {
			guilang_processor_match(processor, ":");
			text = guilang_processor_consume(processor);
		} else 
		if (strcmp(curr, "click") == 0) {
			guilang_processor_match(processor, ":");
			click = guilang_processor_consume(processor);
		} else {
			double* param;
			switch(curr[0]) {
				case 'x': param = &x; break;
				case 'y': param = &y; break;
				case 'w': param = &w; break;
				case 'h': param = &h; break;
				default: guilang_error(processor, "Bad parameter! BUTTON object does not accept \'%s\'!", curr);
			}
			guilang_processor_match(processor, ":");
			curr = guilang_processor_consume(processor);
			*param = strtod(curr, NULL);
		}
	} while(strcmp(guilang_processor_consume(processor), ",") == 0);
	
	gui_button* gb = gui_button_init(name, x, y, w, h);
	gui_text_settext(gb->text, text, 0);
	gui_button_setclickcb(gb, processor->bus, click);
	return gb;
}
	
// SLIDER
// ========================================================================= //
// SLIDER ( [ [x, y, w, h, value]:#, name:@]* )
gui_slider*
guilang_buildslider
(
	guilang_processor* processor,
	gui_cell* gc
)
{
	double x = 0.0;
	double y = 0.0;
	double w = 1.0;
	double h = 1.0;
	double value = 0.0;
	char* name = "SLIDER";
	
	guilang_processor_match(processor, "SLIDER");
	guilang_processor_match(processor, "(");
	char* curr;
	do {
		curr = guilang_processor_consume(processor);
		if (strcmp(curr, "name") == 0) {
			guilang_processor_match(processor, ":");
			name = guilang_processor_consume(processor);
		} else
		if (strcmp(curr, "value") == 0) {
			guilang_processor_match(processor, ":");
			value = strtod(guilang_processor_consume(processor), NULL);
		}		
		else {
			double* param;
			switch(curr[0]) {
				case 'x': param = &x; break;
				case 'y': param = &y; break;
				case 'w': param = &w; break;
				case 'h': param = &h; break;
				default: guilang_error(processor, "Bad parameter! SLIDER object does not accept \'%s\'!", curr);
			}
			guilang_processor_match(processor, ":");
			curr = guilang_processor_consume(processor);
			*param = strtod(curr, NULL);
		}
	} while (strcmp(guilang_processor_consume(processor), ",") == 0);
	
	gui_slider* gs = gui_slider_init(name, x, y, w, h);
	gs->value = value;
	return gs;
}

// TEXTIN
// ========================================================================= //
// TEXTIN ( [ [x, y, w, h]:#, [name, text]:@]* )
gui_textin*
guilang_buildtextin
(
	guilang_processor* processor,
	gui_cell* gc
)
{
	double x = 0.0;
	double y = 0.0;
	double w = 1.0;
	double h = 1.0;
	char* text = NULL;
	char* name = "TEXTIN";
	
	guilang_processor_match(processor, "TEXTIN");
	guilang_processor_match(processor, "(");
	char* curr;
	do {
		curr = guilang_processor_consume(processor);
		if (strcmp(curr, "name") == 0) {
			guilang_processor_match(processor, ":");
			name = guilang_processor_consume(processor);
		} else
		if (strcmp(curr, "text") == 0) {
			guilang_processor_match(processor, ":");
			text = guilang_processor_consume(processor);
		}		
		else {
			double* param;
			switch(curr[0]) {
				case 'x': param = &x; break;
				case 'y': param = &y; break;
				case 'w': param = &w; break;
				case 'h': param = &h; break;
				default: guilang_error(processor, "Bad parameter! TEXTIN object does not accept \'%s\'!", curr);
			}
			guilang_processor_match(processor, ":");
			curr = guilang_processor_consume(processor);
			*param = strtod(curr, NULL);
		}
	} while (strcmp(guilang_processor_consume(processor), ",") == 0);

			printf("?\n");
	gui_textin* gt = gui_textin_init(name, x, y, w, h);
	if (text!= NULL) gui_textin_cat(gt, text);
	return gt;
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
	// Don't build a cell if there is no cell code to read!
	if (strcmp(processor->current, "CELL") != 0) {
		return NULL;
	}
	
	// Match first tokens
	guilang_processor_match(processor, "CELL");
	guilang_processor_match(processor, "{");
	
	// Current word pointer
	char* curr;
	
	// Initialize cell to return
	gui_cell* gc = gui_cell_init();
	
	// Build internal cells
	if(strcmp(processor->current, "(") == 0) {
		// match open paren
		guilang_processor_match(processor, "(");
		
		// Get a parameter name
		curr = guilang_processor_consume(processor);
		
		// The first value should be an orientation
		gui_cell_content orientation;
		if (strcmp(curr, "horizontal") == 0) {
			orientation = GUI_CELL_HORIZONTALCELLS;
		} else
		if (strcmp(curr, "vertical") == 0) {
			orientation = GUI_CELL_VERTICALCELLS;
		} else {
			guilang_error(processor, "Bad Parameter! CELL expects an orientation! Got \'%s\'", curr);
		}
		
		// Match comma and set up a list of partition values for spacing cells
		guilang_processor_match(processor, ",");
		list* positions = list_new();
		do {
			// match partition value
			curr = guilang_processor_consume(processor);
			double* pos = malloc(sizeof(double));			
			*pos = strtod(curr, NULL);
			
			// add to partition list
			list_add(positions, pos);
			
			//consume next token and compare for loop
			curr = guilang_processor_consume(processor);
		} while (curr[0] != ')');
		
		// Initialize cell partition
		gui_cell* cell = guilang_buildcell(processor, gw);
		
		// initialize cell position
		double x = 0.0;
		double y = 0.0;
		
		// Step values for partitions
		double* step;
		double laststep = 0.0;
		
		// Iterate over list of partitions
		for(list* l = positions; l->data!=NULL; l = l->next) {
		
			// Get positions
			step = (double*)(l->data);
			
			// Add the last cell to the current cell
			gui_cell_addcell(gc, cell, orientation, step, *step - laststep);
			
			// Get position for next cell
			if (orientation == GUI_CELL_HORIZONTALCELLS) { 
				x = (*step)*gc->dim->w;
			} else {
				y = (*step)*gc->dim->h;
			}
			
			// Initialize next cell and move
			cell = guilang_buildcell(processor, gw);
			gui_cell_move(cell, x, y);
			
			// Set last step value
			laststep = *step;
		}
		
		// Set end partition value
		double* end = malloc(sizeof(double));
		*end = 1.0;
		
		/// add last cell
		gui_cell_addcell(gc, cell, orientation, end, 1.0 - laststep);
		
		// Clear temp objects
		list_delete(positions);
		
	} else
	if(strcmp(processor->current, "BUTTON") == 0) {
		gui_button* gb = guilang_buildbutton(processor, gc);
		gui_cell_addobject(gc, gb, GUI_CELL_BUTTON);
	} else
	if(strcmp(processor->current, "VIEWPORT") == 0) {
		gui_viewport* gv = guilang_buildviewport(processor, gc);
		gui_cell_addobject(gc, gv, GUI_CELL_VIEWPORT);
	} else
	if(strcmp(processor->current, "SLIDER") == 0) {
		gui_slider* gs = guilang_buildslider(processor, gc);
		gui_cell_addobject(gc, gs, GUI_CELL_SLIDER);
	} else
	if(strcmp(processor->current, "TEXTIN") == 0) {
		gui_textin* gt = guilang_buildtextin(processor, gc);
		gui_cell_addobject(gc, gt, GUI_CELL_TEXTIN);
	} else
	if(strcmp(processor->current, "text") == 0) {
		gui_text* gt = gui_text_init(0, 0, 10.0);

		guilang_processor_match(processor, "text");
		guilang_processor_match(processor, ":");
		char* fmtstr = guilang_processor_consume(processor);
		if (processor->current[0] == '+') {
			guilang_processor_match(processor, "+");
			curr = guilang_processor_consume(processor);
			gui_text_settext(gt, fmtstr, 1, hashtable_get(processor->vars, curr));
		} else {
			gui_text_settext(gt, fmtstr, 0);
		}
		
		gui_cell_addobject(gc, gt, GUI_CELL_TEXT);
	} else
	if(strcmp(processor->current, "}") == 0) {
	}

	guilang_processor_match(processor, "}");
	
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
	if (processor->current == NULL || strcmp(processor->current, "WINDOW") != 0) {
		return NULL;
	}
		
	float x = 0.0;
	float y = 0.0;
	float w = 1.0;
	float h = 1.0;
	char* name = "WINDOW";
	
	guilang_processor_match(processor, "WINDOW");
	char* curr = guilang_processor_consume(processor);
	
	if (curr[0] == '(') {
		
		do {
			curr = guilang_processor_consume(processor);
			if (strcmp(curr, "name") == 0) {
				guilang_processor_match(processor, ":");
				name = guilang_processor_consume(processor);
			} else {
				float* param;
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
			}
			curr = guilang_processor_consume(processor);
		} while(curr[0] != ')');
	}
	x *= g->dim->w;
	y *= g->dim->h;
	gui_window* gw = gui_window_init(name, x, y, w, h);
	gui_window_resize(gw, g->dim->w, g->dim->h);
	
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
	guilang_processor_match(processor, "}");

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
	hashtable* vars
)
{
	float x = 0.0;
	float y = 0.0;
	float w = 1.0;
	float h = 1.0;

	guilang_processor* processor = guilang_processor_init(stream, log, bus);
	processor->vars = vars;
	
	guilang_readheader(processor);
	
	guilang_processor_match(processor, "GUI");
	
	char* curr;
	
	if (processor->current[0]== '(') {
		guilang_processor_match(processor, "(");
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

	gui* g = gui_init(x, y, w, h);
	g->vars = vars;
	int* screenw = hashtable_get(vars, GUIVAR_GUIWIDTH);
	int* screenh = hashtable_get(vars, GUIVAR_GUIHEIGHT);

	gui_resize(g, *screenw, *screenh);
	gui_window* gw = guilang_buildwindow(processor, g);
	while(gw != NULL) {
		gui_openwindow(g, gw);
		gw = guilang_buildwindow(processor, g);
	}
	return g;
}