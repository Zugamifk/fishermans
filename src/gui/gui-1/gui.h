#define GUI_EPSILON 0.0001
typedef int gui_contenttype;

// Structs
// ==============
// GUI OBJECT CONTAINER
#define GUI_CONTENTTYPE_NULL	(-1)
#define GUI_CONTENTTYPE_DIV		0
#define GUI_CONTENTTYPE_BUTTON	1
#define	GUI_CONTENTTYPE_TEXTBOX	2
#define	GUI_CONTENTTYPE_SLIDER	3

#define	GUI_CONTAINER_INVISIBLE	0
#define GUI_CONTAINER_VISIBLE	1

typedef struct
t_guicontainer
{
	void 		*content;
	gui_contenttype type;
	int 		visible;
	double 		width;
	double 		height;
} gui_container;

gui_container *gui_container_init(double w, double h);
gui_container *gui_container_initwith(double w, double h, void *content, gui_contenttype type, double x, double y);
int			gui_container_insert(gui_container *c, void *content, gui_contenttype type, double x, double y);
void		gui_container_draw(gui_container *c, double t, double dt);

//====================
// Gui object box
typedef struct
t_gui_box
{
	double scale;
	double x;
	double y;
	double width;
	double height;
	double hpadin;
	double vpadin;
	double hpadout;
	double vpadout;
	int		visible;
} gui_box;
gui_box *	gui_box_init(double x, double y, double w, double h);
void		gui_box_position(gui_box *b, gui_container *c, double sx, double sy);
void		gui_box_translateinto(gui_box *box);
int			gui_box_inbox(gui_box *b, double x, double y);
void		gui_box_getscalars(gui_box *b, double *sx, double *sy, double x, double y);

// GUI CONTAINER DIVIDER
#define		GUI_DIV_HORZ	0
#define		GUI_DIV_VERT	1
#define		GUI_DIV_LEFT	0
#define		GUI_DIV_RIGHT 	1
#define		GUI_DIV_BOTTOM	1
#define		GUI_DIV_TOP		0

typedef struct
t_guidiv
{
	int 		axis;
	double 		scalarpos;
	double		realpos;
	double		width;
	double		height;
	int			visible;
	gui_container **divs;
} gui_div;

gui_div *	gui_div_init(int axis);
gui_div *	gui_div_initwith(int axis, gui_container *c0, gui_container *c1);
void		gui_div_position(gui_div *d, double spos);
int			gui_div_insert(gui_div *d, int pos, gui_container *c);
void		gui_div_resize(gui_div *div, double w, double h);
gui_container *gui_div_find(gui_container *c, double *x, double *y);
void		gui_div_draw(gui_div *d, double t, double dt);


// ============================================
// BUTTON
#define GUI_BUTTON_INACTIVE 0
#define GUI_BUTTON_ACTIVE	1
#define GUI_BUTTON_CLICKED	2

typedef struct
t_guibutton
{
	int 		state;
	char *		text;
	double 		scale;
	gui_box *	bounds;
	double 		scalarx;
	double		scalary;
	fontinfo	*font;
	event_id	clickevent;
	void 		*eventdata;
} gui_button;

gui_button *	gui_button_init(double scale);
gui_button *	gui_button_initwithf(char *text, fontinfo *fi, double scale);
gui_button *	gui_button_initwithe(event_id e, void *data, void (*f)(void *), double scale);
gui_button *	gui_button_initwithfe(char *text, fontinfo *fi, event_id e, void *data, void (*f)(void *), double scale);
int				gui_button_setevent(gui_button *b, event_id e, void *data);
void			gui_button_triggerevent(gui_button *b);
int				gui_button_onbutton(gui_button *b, gui_container *c, double x, double y);
void			gui_button_draw(gui_button *b, double t, double dt);

// ===============================================
// GUI TEXTBOX

typedef struct
t_guitextbox
{
	fontinfo *	font;
	char *		text;
	double		scale;
	gui_box *	bounds;
	double 		scalarx;
	double		scalary;
} gui_textbox;

gui_textbox	*	gui_textbox_init(char *text, fontinfo *fi, double scale, double w, double h);
void			gui_textbox_draw(gui_textbox *tb, double t, double dt);

// ===============================================
// GUI SLIDER

#define GUI_SLIDER_ACTIVE 0
#define GUI_SLIDER_INACTIVE 1
#define GUI_SLIDER_SLIDING 2

#define GUI_SLIDER_HORZ 0
#define GUI_SLIDER_VERT 1

typedef struct
t_guislider
{
	int 		state;
	int			axis;
	double		scale;
	double		length;
	double 		val;
	gui_box *	bounds;
	vec2 *		pos;
	event_id	slideevent;
} gui_slider;

gui_slider *	gui_slider_init(double length, double scale, int axis);
gui_slider *	gui_slider_initwithe(double length, double scale, int axis, event_id e, void (*f)(void *));
int				gui_slider_setevent(gui_slider *s, event_id e);
void			gui_slider_triggerevent(gui_slider *s);
void			gui_slider_onclick(gui_slider *s, double x, double y);
void			gui_slider_draw(gui_slider *s, double t, double dt);

// ========================================
// GUI GLOBALS

event_bus 	*gui_eventbus;

int 		gui_w, gui_h;
double 		gui_aspectratio;

mouse_state *gui_mousestate;
double		gui_mousex, gui_mousey;

float		*gui_dials;
char		*gui_message;

fontinfo	*gui_fontinfo;
gui_container *gui_maingui;
colorpack	*gui_colors;

#include "gui_box.c"
#include "gui_container.c"

#include "gui_div.c"
#include "gui_button.c"
#include "gui_textbox.c"
#include "gui_slider.c"

#include "gui_elements.c"
#include "gui.c"
