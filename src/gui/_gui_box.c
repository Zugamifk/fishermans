typedef struct
_S_gui_box
{
	_gui_dimension* dim;
	vec2* pos;
} _gui_box;

_gui_box*
_gui_box_init
(
	double x,
	double y,
	double w,
	double h
)
{
	_gui_box* gb = malloc(sizeof(_gui_box));
	gb->dim = _gui_dimension_init(w, h);
	gb->pos = vec2_new(x, y);
	return gb;
}

void
_gui_box_resize
(
	_gui_box* gb,
	double w,
	double h
)
{
	_gui_dimension_resize(gb->dim, w, h);
}

bool
_gui_box_contains
(
	_gui_box* gb,
	double x,
	double y
)
{
	return (gb->pos->x < x && 
		gb->pos->y < y && 
		x < gb->pos->x + gb->dim->w && 
		y < gb->pos->y + gb->dim->h);
}

int
_gui_box_getx
(
	_gui_box* gb,
	int x
)
{
	return x - gb->pos->x;
}

double
_gui_box_getxn
(
	_gui_box* gb,
	int x
)
{
	return ((double)x-gb->pos->x)/gb->dim->w;
}

int
_gui_box_gety
(
	_gui_box* gb,
	int y
)
{
	return y - gb->pos->y;
}

double
_gui_box_getyn
(
	_gui_box* gb,
	int y
)
{
	return ((double)y-gb->pos->y)/gb->dim->h;
}

void
_gui_box_defdraw
(
	_gui_box* gb
)
{
	shapes_boxfilled(gb->dim->w, gb->dim->h);
}

void
_gui_box_print
(
	_gui_box* gb
)
{
	printf("\tBOX BOUNDS:");
	printf("\tPOS:\t");
	vec2_print(gb->pos);
	printf("\tDIM:\t");
	_gui_dimension_print(gb->dim);
}