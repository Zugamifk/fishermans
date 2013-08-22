typedef struct
_S_gui_manager
{
	gui* data;
	event_bus* bus;
	errorlog* log;
} gui_manager;

gui_manager*
gui_manager_init
(
	char** stream,
	errorlog* log,
	event_bus* bus,
	screeninfo* info
)
{
	gui_manager* gm = malloc(sizeof(gui_manager));
	
	guilang_processor* processor = guilang_processor_init(stream, log, bus);

	gui* g = guilang_buildgui(processor);
	gui_resize(g, info->w, info->h);
	gm->data = g;
	gm->bus = bus;
	gm->log = log;
	
	return gm;
}