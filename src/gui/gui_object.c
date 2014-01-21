typedef union
_U_gui_object_descriptor
{
	gui_viewport* viewport;
} gui_object_descriptor;

typedef struct
_S_gui_object
{
	gui_object_descriptor
	_gui_box* bounds;
	event_id hover_eid;
	event_id click_eid;
	event_id keydown_eid;
	event_id keyup_eid;
} gui_object;