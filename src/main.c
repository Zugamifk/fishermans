#include "main.h"

#define DEBUGMAIN
#ifdef DEBUGMAIN
//	#define DEBUG_DONTOPENWINDOW
	#define DEBUG_KEEPLOG
#endif

void initglut();
void update(int v);
void draw();
void resize(int, int);
void upkeys(unsigned char, int, int);
void skeys(int, int, int);
void mousemove(mouse_state *);
void mouseup(mouse_state *);
void quit();

int screenwidth;
int screenheight;
event_bus *Meb;
errorlog *Mlog;
mouse_state *Mms;
audio_system *Mfms;
shaderprogram *Msp;
gui* Mgui;
gui_style* Mgs;

int main(int argc, char** argv) {
	
	//bittwiddle_test();
	
	srand(time(NULL));
	glutInit(&argc, argv);
	
	DTIME = 0.016;
	
	vec4_initglobals();
	
	screenwidth = 800;
	screenheight = 600;
	
	Meb 	= bus_init();
	event_id quitid = bus_neweventwithname(Meb, "QUIT");
	bus_subscribe(Meb, quitid, quit);
	
	Mlog = errorlog_init("Main", "errorlog.txt", 0);
	//errorlog_logdef(Mlog, "ARE YOU READY TO GET DOWN", "DOWN");
		
	Mms = mouse_init(Meb);
	bus_subscribe(Meb, mouse_event_up, (void *)mouseup);
	bus_subscribe(Meb, mouse_event_move, (void *)mousemove);
	
	Mfms = audio_system_init();
	
	hashtable* guivars = hashtable_init(0);
	hashtable_insert(guivars, GUIVAR_GUIWIDTH, &screenwidth);
	hashtable_insert(guivars, GUIVAR_GUIHEIGHT, &screenheight);

	//Mgui = guilang_compile("data/guilangspec1", "data/testgui.txt", Mlog, Meb, guivars);
	//gui_print(Mgui);
	//Mgs = gui_debugstyle_init();
	
	Sprite_init(Mlog, Meb, guivars);
	
	//gui_init(Meb, Mms, SCREENW, SCREENH);

	//audio_test_init(Mfms, gui_dials, SDR_NUMDIALS);
	
	#ifndef DEBUG_DONTOPENWINDOW
	initglut();
	#endif
	
	quit();
	
	return 0;
}

void
initglut()
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenwidth, screenheight);
	glutInitWindowPosition(0,0);
	glutCreateWindow("ULTRA SHADER EXTREME");
	
	glShadeModel( GL_SMOOTH );
	glClearColor( 1.0, 1.0, 1.0, 0.0 );
	glClearDepth( 1.0 );
	glEnable( GL_DEPTH_TEST );

	glutDisplayFunc(draw);
	glutTimerFunc(40, update, 0);
 	glutReshapeFunc( resize );
	//glutKeyboardFunc(keys);
	glutKeyboardUpFunc(upkeys);
	glutSpecialFunc(skeys);
	//glutSpecialUpFunc(upkeys);
	glutMouseFunc(mouse_clickevent); 
	glutPassiveMotionFunc(mouse_moveevent);
	glutMotionFunc(mouse_moveevent);

	glutIgnoreKeyRepeat(1);
	glutSetCursor(GLUT_CURSOR_NONE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0, 1.0, 1.0, 30.0 );

	glMatrixMode(GL_MODELVIEW);
	
	// Set up shaders
	glewInit();
	
	// const GLchar *shaders[] = {
		// "shaders/vshader.sdr",
		// "shaders/fshader.sdr"
	// };
//	Msp = shader_init(shaders);
	Sprite_initshaders();
	
	printf("init done!\n");
	glutMainLoop();

}

void
update(int v)
{
	glutPostRedisplay();
	TIME += DTIME;

	//shader_update1f(Msp, "time", TIME);
	//shader_update1fv(Msp, "dials", SDR_NUMDIALS, gui_dials);
	
	Sprite_update(TIME, DTIME);
	//gui_update(Mgui, TIME, DTIME);
	
	glutTimerFunc(16, update, v);
}

void
draw()
{	
	
	//glActiveTexture(GL_TEXTURE0);
//	shader_update1i(Msp, "tex", 0);
//	glBindTexture(GL_TEXTURE_2D, Msp->texture);
		glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluOrtho2D(0.0, (double)screenwidth, 0.0, (double)screenheight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glTranslated(Mms->x, screenheight - Mms->y, 0.0);
	glColor3f(0.4, 0.1, 0.4);
	shapes_cursor();
	glPopMatrix();
	
	Sprite_draw(TIME, DTIME);
	//gui_style_draw(Mgs, Mgui, TIME, DTIME);
 //     gluPerspective( 60.0, aspectratio, 1.0, 30.0 );

  //  gluLookAt(0.0, 5.0, -7.0,
  //          0.0, 2.0, 0.0,
  //          0.0, 1.0, 7.0);
	glutSwapBuffers();
	glutPostRedisplay();
}

void
resize(int w, int h)
{
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport( 0, 0, w, h );	// View port uses whole window
	
	screenwidth = w;
	screenheight = h;
	aspectratio = (float)w/(float)h;
	
	//gui_resize(Mgui, w, h);
	Sprite_resize(w, h);
	
	// Set up the projection view matrix (not very well!)
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, aspectratio, 0.1, 100.0 );

	// Select the Modelview matrix
    glMatrixMode( GL_MODELVIEW );
}

void
mousemove(mouse_state *ms)
{
	Sprite_mousemove(ms);
	//gui_mouseupdate(Mgui, ms->x, screenheight - ms->y);
}

void
mouseup(mouse_state *ms)
{
	Sprite_mouseup(ms);
	//gui_click(Mgui, Meb);
}

void
upkeys(unsigned char key, int x, int y)
{
	switch(key) {
		case 27:
			// kill window somehow
			quit();
		break;
	}
}

void
skeys(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_UP:
		//	shader_gentex(&(Msp->texture));
		break;
		case GLUT_KEY_DOWN:
		break;
	}
}

void
quit
(
	void* anything
)
{
	errorlog_free(Mlog);
	exit(0);
}