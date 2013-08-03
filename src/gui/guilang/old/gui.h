typedef	vec2	gui_position;
typedef vec2	gui_dimensions;

typedef enum
e_gui_state
{
	GUI_STATE_INACTIVE,
	GUI_STATE_ACTIVE
} gui_state;

typedef void (*gui_drawcallback)(void*, float, float);

typedef int		gui_WindowID;

#define GUI_MAXWINDOWS	256

void gui_drawdefault(void*, float, float);

#include "gui_object.c"
#include "gui_cell.c"
#include "gui_window.c"
#include "gui.c"

#include "guilang/guilang.h"