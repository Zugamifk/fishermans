typedef struct
_S_gui_dimension
{
	float w;
	float h;
} _gui_dimension;

_gui_dimension*
_gui_dimension_init
(
	float w,
	float h
)
{
	_gui_dimension* dim = malloc(sizeof(_gui_dimension));
	dim->w = w;
	dim->h = h;
	return dim;
}

void
_gui_dimension_free
(
	_gui_dimension* dim
)
{
	free(dim);
}