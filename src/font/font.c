typedef enum
font_format_e
{
	FONT_NONE,
	FONT_GRID
} font_format;

typedef union
font_data_u
{
	fontinfo *line;
	gridfont *grid;
} font_data;

typedef struct
font_s
{
	font_format format;
	font_data *data;
} font;

font*
font_init
(
	font_format *format
)
{
	font *f = malloc(sizeof(font));
	f->format = format;
	switch(format) {
		case FONT_GRID: {
			d->data.grid = gridfont_init();
		} break;
		default: f->data = NULL; break;
	}
	return f;
}

void
font_write
(
	char* str
)
{

}