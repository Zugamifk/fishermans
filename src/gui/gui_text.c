#define GUI_TEXT_LEN (1<<8)
typedef struct
_S_gui_text
{
	char* text;
	double size;
	vec2* pos;
	char* fmtstring;
	list* fmtargs;
	bool frozen;
} gui_text;

gui_text*
gui_text_init
(
	double x,
	double y,
	double s
)
{
	gui_text* gt = malloc(sizeof(gui_text));
	
	gt->text = malloc(GUI_TEXT_LEN);
	gt->pos = vec2_new(x, y);
	gt->size = s;
	gt->fmtstring = malloc(GUI_TEXT_LEN);
	gt->fmtargs = list_new();
	gt->frozen = false;
	return gt;
}

void
gui_text_update
(
	gui_text* gt
)
{
	if (gt->frozen) return;
	
	char* curr = gt->fmtstring;
	char* word = curr;
	char* textptr = gt->text;
	list* args = gt->fmtargs;
	while (*curr != '\0') {
		if (*curr == '%') {
			int len = curr-word;
			strncpy(textptr, word, len);
			textptr += len;
			curr++;
			char* fmtstr;
			if (args == NULL || args->data == NULL) {
				textptr += sprintf(textptr, "NULL");
			} else
			if (*curr == 'd') {
				fmtstr = "%d";
				int* arg = (int*)(args->data);
				textptr += sprintf(textptr, fmtstr, *arg);
				args = args->next;
			} else
			if (*curr == 'f') {
				fmtstr = "%f";
				textptr += sprintf(textptr, fmtstr, *((double*)(args->data)));
				args = args->next;
			}
			curr++;
			word = curr;
		} else {
			curr++;
		}
	}
	int len = curr-word;
	strncpy(textptr, word, len);
	textptr += len;
	*textptr = '\0';
}

void
gui_text_settext
(
	gui_text* gt,
	char* fmtstring,
	int numargs,
	...
)
{
	va_list fmtargs;
	va_start(fmtargs, numargs);
	for (int i = 0; i < numargs; i++) {
		list_add(gt->fmtargs, va_arg(fmtargs, void*));
	}
	va_end(fmtargs);
	strcpy(gt->fmtstring, fmtstring);
	gui_text_update(gt);
}

void
gui_text_print
(
	gui_text* gt
)
{
	printf("TEXT:\"%s\"\n", gt->text);
	printf("\tSIZE:\t%f\n", gt->size);
	printf("\tPOS:\t");
	vec2_print(gt->pos);
	printf("FORMAT STRING:\"%s\"\n", gt->fmtstring);
	printf("FROZEN?: %d\n", gt->frozen);
}