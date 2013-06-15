void
gui_mousemoveevent(mouse_state *ms)
{
	gui_mousex = gui_aspectratio*(double)gui_mousestate->x/(double)gui_w;
	gui_mousey = 1.0-(double)gui_mousestate->y/(double)gui_h;
}

void
gui_mouseupevent(mouse_state *ms)
{
	gui_container_mouseup(gui_maingui, gui_mousex, gui_mousey);
}

void
_quitthisshit(void *_)
{
	exit(0);
}

void
_updatedial0(void *v)
{
	gui_dials[0] = (float)(*((double *)v));
}

void
_updatedial1(void *v)
{
	gui_dials[1] = (float)(*((double *)v));
}

void
_updatedial2(void *v)
{
	gui_dials[2] = (float)(*((double *)v));
}

void
_updatedial3(void *v)
{
	gui_dials[3] = (float)(*((double *)v));
}

void
_updatedial4(void *v)
{
	gui_dials[4] = (float)(*((double *)v));
}

void
_updatedial5(void *v)
{
	gui_dials[5] = (float)(*((double *)v));
}

void
_updatedial6(void *v)
{
	gui_dials[6] = (float)(*((double *)v));
}

void
_resettexture(void *_)
{
	extern shaderprogram *Msp;
	shader_gentex(&(Msp->texture));
}


void
gui_init(event_bus *eb, mouse_state *ms, int w, int h)
{
	// set up globals
	gui_eventbus 	= eb;
	gui_w 			= w;
	gui_h			= h;
	gui_mousestate 	= ms;
	gui_aspectratio = (double)w/(double)h;
	gui_mousex = gui_aspectratio*(double)gui_mousestate->x/(double)gui_w;
	gui_mousey = 1.0-(double)gui_mousestate->y/(double)gui_h;
	
	gui_dials = calloc(SDR_NUMDIALS, sizeof(float));
	
	// Set GUI colours
	color **cols = malloc(sizeof(color*)*6);
	cols[0] = color_new3(0.02, 0.18, 0.3);
	cols[1] = color_new3(0.8, 0.3, 0.1);
	cols[2] = color_new3(0.95, 0.95, 0.9);
	cols[3] = color_new3(0.08, 0.1, 0.1);
	cols[4] = color_new3(0.016, 0.2, 0.2);
	cols[5] = color_new3(0, 0, 0);
	gui_colors = color_newpack(cols, 6);
	
	// Set up GUI
	gui_fontinfo = font_init("extremeradcool", gui_colors->colors[2]);
	
	
	gui_div *dddials[SDR_NUMDIALS];
	gui_contenttype ddtypes[SDR_NUMDIALS];
	vec2 ddpos[SDR_NUMDIALS];
	void (*ddevents[SDR_NUMDIALS])(void *) = {
		_updatedial0,
		_updatedial1,
		_updatedial2,
		_updatedial3,
		_updatedial4,
		_updatedial5,
		_updatedial6
	};
	static char ddnames[SDR_NUMDIALS][32] = {
		"Oftenality",
		"Affection",
		"Spiralitude",
		"Boxulation",
		"Nonstraightness",
		"Wanderlust",
		"Zoom"
	};
	
	for(int i=0; i<SDR_NUMDIALS; i++) {
		gui_textbox *text = 
			gui_textbox_init(
				ddnames[i],
				gui_fontinfo,
				0.035, 
				0.0, 0.0
			);
		text->bounds->visible = 0;
		gui_slider *slider = 
			gui_slider_initwithe(
				0.5, 0.01, 
				GUI_SLIDER_HORZ,
				bus_newevent(gui_eventbus),
				ddevents[i]
			);
		dddials[i] = gui_div_initwith(
			GUI_DIV_HORZ,
			gui_container_initwith(1, 1,
				text,
				GUI_CONTENTTYPE_TEXTBOX,
				0, 0.5),
			gui_container_initwith(1, 1,
				slider,
				GUI_CONTENTTYPE_SLIDER,
				0.1, 0.1
			)
		);	
		dddials[i]->visible = 0;
		ddtypes[i] = GUI_CONTENTTYPE_DIV;
		ddpos[i].x = 0.5;
		ddpos[i].y = 0.5;
	}
	
	gui_container *dialdiv = 
		gui_container_initmany(
			1, 1,
			GUI_DIV_HORZ,
			(void **)dddials,
			ddtypes,
			ddpos,
			SDR_NUMDIALS
		);
	
	gui_container *buttondiv = 
		gui_container_initwith(1, 1,
			gui_div_initwith(
				GUI_DIV_VERT,
				gui_container_initwith(1, 1,
					gui_button_initwithfe(
						"Reset", gui_fontinfo,
						bus_newevent(gui_eventbus), NULL, _resettexture,
						0.03
					),
					GUI_CONTENTTYPE_BUTTON,
					0.5, 0.5
				),
				gui_container_initwith(1, 1,
					gui_button_initwithfe(
						"Quit", gui_fontinfo,
						bus_newevent(gui_eventbus), NULL, _quitthisshit,
						0.03
					),
					GUI_CONTENTTYPE_BUTTON,
					0.5, 0.5
				)
			),
			GUI_CONTENTTYPE_DIV,
			0.5, 0.5
		);
		((gui_div *)(buttondiv->content))->visible = 0;
		
	gui_container *rightpane = 
		gui_container_initwith(1, 1,
			gui_div_initwith(GUI_DIV_HORZ,
				dialdiv,
				buttondiv
			),
			GUI_CONTENTTYPE_DIV,
			0, 0.1
		);
		
	gui_message = malloc(512);
	sprintf(gui_message, "aass");
	
	gui_container *leftpane =
		gui_container_initwith(1, 1,
					// gui_textbox_init(
						// gui_message,
						// gui_fontinfo,
						// 0.02,
						// 1.0, 0.1
					// ),
					NULL,
					GUI_CONTENTTYPE_NULL,
					0.0, 0.0
				);
	leftpane->visible = 1;
				
	gui_maingui =
		gui_container_initwith(gui_aspectratio, 1.0,
			gui_div_initwith(GUI_DIV_VERT,
				leftpane,
				rightpane
			),
			GUI_CONTENTTYPE_DIV,
			0.8, 0.9
		);

//	pc = gui_container_init(gui_aspectratio, 0.5);
	
	bus_subscribe(eb, mouse_event_up, (void *)gui_mouseupevent);
	bus_subscribe(eb, mouse_event_move, (void *)gui_mousemoveevent);
}

void
gui_resize(int w, int h)
{
	gui_w = w;
	gui_h = h;
	gui_aspectratio = (double)w/(double)h;
	
	gui_mousex = gui_aspectratio*(double)gui_mousestate->x/(double)gui_w;
	gui_mousey = 1.0-(double)gui_mousestate->y/(double)gui_h;

	gui_container_resize(gui_maingui, gui_aspectratio, 1.0);
}

void
gui_update(double t, double dt)
{
	sprintf(gui_message, 
		"FEAST YOUR EYEBALLS\nM = distance from centre -- T = time -- A = angle\nf(x) = "
			"%.1f * "
			"sin(M*%.1f + A*%1.0f + 10*T) * "
			"cos(%.1f*sin(M*%.1f + 3*T)+%1.0f*A)",
		gui_dials[1],
		gui_dials[0]*32.0,
		gui_dials[2]*32.0,
		gui_dials[5]*32.0,
		gui_dials[4]*32.0,
		gui_dials[3]*32.0
	);
	gui_container_reset(gui_maingui);
	gui_container_findmouse(gui_maingui, gui_mousex, gui_mousey);
}

void
gui_draw(double t, double dt)
{

	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluOrtho2D(0, gui_aspectratio, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
		glTranslated(gui_mousex, gui_mousey, 0.0);
		glColor3f(0.2, 0.65, 0.05);
		shapes_cursor(0.3);
	glPopMatrix();
	
	glPushMatrix();
	gui_container_draw(gui_maingui, t, dt);
	glPopMatrix();
	
}