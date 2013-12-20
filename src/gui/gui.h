#ifndef GUI_RELEASE
#define GUI_DEBUGDRAWGUI
#define GUI_DEBUGCOLORS
#endif

#define GUI_FOCUSSTRING "KEYBOARD FOCUS"

// element components
#include "_gui_dimension.c"
#include "_gui_box.c"

// text
#include "gui_text.c"
#include "gui_textbox.c"

// atomic elements
#include "gui_viewport.c"
#include "gui_button.c"
#include "gui_slider.c"
#include "gui_textin.c"

// compound elements
#include "gui_console.c"

// containers
#include "gui_cell.c"
#include "gui_window.c"

// main gui
#include "gui.c"

// .gui file interpreter
#include "guilang/guilang.h"

// gui graphics
#include "gui_style.c"
#include "gui_style_defaultstyle.c"
#include "gui_debugstyle.c"