typedef struct
_S_gui_dimension
{
	double w;
	double h;
	double sw;
	double sh;
} _gui_dimension;

_gui_dimension*
_gui_dimension_init
(
	double sw,
	double sh
)
{
	_gui_dimension* dim = malloc(sizeof(_gui_dimension));
	dim->sw = sw;
	dim->sh = sh;
	return dim;
}

void
_gui_dimension_resize
(
	_gui_dimension* dim,
	double w,
	double h
)
{
	dim->w = w * dim->sw;
	dim->h = h * dim->sh;
}

void
_gui_dimension_free
(
	_gui_dimension* dim
)
{
	free(dim);
}

void
_gui_dimension_print
(
	_gui_dimension* dim
)
{
	printf("[%.2f, %.2f, %.2f, %.2f]\n", dim->w, dim->h, dim->sw, dim->sh);
}