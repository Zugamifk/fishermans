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

int screenwidth = 800;
int screenheight = 600;
event_bus *Meb;
errorlog *Mlog;
mouse_state *Mms;
keyboard_state *Mks;
application_data *Mapp;

int main(int argc, char** argv) {
	
	srand(time(NULL));
	glutInit(&argc, argv);
	initglut();
	
	DTIME = 0.016;
	
	vec4_initglobals();
	
	Meb = bus_init();
	event_id quitid = bus_neweventwithname(Meb, "QUIT");
	bus_subscribe(Meb, quitid, quit);
	
	Mlog = errorlog_init("Main", "errorlog.txt", 0);
	
	Mms = mouse_init(Meb);
	bus_subscribe(Meb, mouse_event_up, (void *)mouseup);
	bus_subscribe(Meb, mouse_event_move, (void *)mousemove);
	
	Mks = keyboard_state_init(0.25, 4, 0.1);
	
	hashtable* Mvars = hashtable_init(0);
	hashtable_insert(Mvars, "WIDTH", &screenwidth);
	hashtable_insert(Mvars, "HEIGHT", &screenheight);
	hashtable_insert(Mvars, "TIME", &TIME);
	hashtable_insert(Mvars, "DTIME", &DTIME);

	Mapp = editor_getappdata(Mlog, Meb, Mvars);
	
	Mapp->init_cb(Mlog, Meb, Mvars);
		
	#ifndef DEBUG_DONTOPENWINDOW
	glutMainLoop();
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
		
	printf("init done!\n");

}

void
update(int v)
{
	glutPostRedisplay();
	TIME += DTIME;

	Mapp->update_cb(TIME, DTIME);
	
	glutTimerFunc(16, update, v);
}

void
draw()
{	
	
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluOrtho2D(0.0, (double)screenwidth, 0.0, (double)screenheight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glTranslated(Mms->x, screenheight - Mms->y, 0.0);
	glColor3f(0.4, 0.1, 0.4);
	shapes_cursor();
	glPopMatrix();
	
	Mapp->draw_cb(TIME, DTIME);

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
	
	if (Mapp->resize_cb != NULL) Mapp->resize_cb(w, h);
	
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
	if (Mapp->mousemove_cb != NULL) Mapp->mousemove_cb(ms);
}

void
mouseup(mouse_state *ms)
{
	if (Mapp->mouseup_cb != NULL) Mapp->mouseup_cb(ms);
}

void
upkeys(unsigned char key, int x, int y)
{
	switch(key) {
		case 27:
			// kill window somehow
			quit();
		break;
		default: {
			if (keyboard_state_update(Mks, key, TIME) &&
				Mapp->keyboardup_cb != NULL) 
			{
				Mapp->keyboardup_cb(Mks, key);
			}
		}
	}
}

void
skeys(int key, int x, int y)
{

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