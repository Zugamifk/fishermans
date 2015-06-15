#define GUI_TEXTIN_MAX (1<<12) // 4096 characters = 4kb

typedef enum
_E_gui_textin_state
{
	GUI_TEXTIN_INACTIVE,
	GUI_TEXTIN_ACTIVE,
	GUI_TEXTIN_HOVER,
	GUI_TEXTIN_TYPING
} gui_textin_state;

typedef struct
_S_gui_textin
{
	char* name;
	char* str;
	char** inptr;
	gui_text* text;
	unsigned int len;
	unsigned int cursor;
	_gui_box* bounds;
	gui_textin_state state;
} gui_textin;

gui_textin*
gui_textin_init
(
	char* name,
	double x,
	double y,
	double w,
	double h
)
{
	gui_textin* gt = malloc(sizeof(gui_textin));
	gt->name = malloc(strlen(name));
	strcpy(gt->name, name);

	gt->str = calloc(1, GUI_TEXTIN_MAX);
	gt->inptr = NULL;
	gt->text = gui_text_init(0.0, 5.0, 8.0);
	gui_text_settext(gt->text, "", 0);
	gt->len = 0;
	gt->cursor = 0;

	gt->bounds = _gui_box_init(x, y, w, h);
	gt->state = GUI_TEXTIN_ACTIVE;

	return gt;
}

void
gui_textin_reset
(
	gui_textin* gt
)
{
	gt->state = GUI_TEXTIN_ACTIVE;
}

bool
gui_textin_contains
(
	gui_textin* gt,
	double x,
	double y
)
{
	return _gui_box_contains(gt->bounds, x, y);
}

void
gui_textin_click
(
	gui_textin* gt,
	hashtable* vars
)
{
	if (gt->state == GUI_TEXTIN_HOVER) {
		gt->state = GUI_TEXTIN_TYPING;
		hashtable_insert(vars, GUI_FOCUSSTRING, gt);
	}
}
 
void
gui_textin_mouseupdate
(
	gui_textin* gt,
	int x,
	int y
)
{
	if (gui_textin_contains(gt, x, y) && gt->state == GUI_TEXTIN_ACTIVE) {
		gt->state = GUI_TEXTIN_HOVER;
	}
}

void
gui_textin_cat
(
	gui_textin* gt,
	char* src
)
{
	char buf[1024];
	int bi = 0;
	for (char *c = src; *c != '\0'; c++) {
		switch (*c) {
			case 8: break;
			default: buf[bi++] = *c; break;
		}
	}
	unsigned int cut = GUI_TEXTIN_MAX - gt->len;
	if (cut < strlen(buf)) buf[cut] = '\0';
	strcat(gt->str, buf);
	gui_text_settext(gt->text, gt->str, 0);
}

void
gui_textin_resize
(
	gui_textin* gt,
	double w,
	double h
)
{
	_gui_box_resize(gt->bounds, w, h);
}

void
gui_textin_print
(
	gui_textin* gt
)
{
	printf("TEXT INPUT \'%s\':\n", gt->name);
	_gui_box_print(gt->bounds);
	printf("\tTEXT: \"%s\"\tLEN: %d\tCURSOR: %d\n", gt->str, gt->len, gt->cursor);
}