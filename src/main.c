#include "main.h"

#define DEBUGMAIN
#ifdef DEBUGMAIN
	#define DEBUG_DONTOPENWINDOW
	#define DEBUG_KEEPLOG
#endif

void initglut();
void update(int v);
void draw();
void resize(int, int);
void upkeys(unsigned char, int, int);
void skeys(int, int, int);

void quit();

#define SCREENW 800
#define SCREENH 600
event_bus *Meb;
errorlog *Mlog;
mouse_state *Mms;
audio_system *Mfms;
shaderprogram *Msp;

int main(int argc, char** argv) {
	
	srand(time(NULL));
	glutInit(&argc, argv);
	
	DTIME = 0.016;
	
	vec4_initglobals();
	
	Meb 	= bus_init();	
	
	Mlog = errorlog_init("Main", "errorlog.txt", 0);
	errorlog_logdef(Mlog, "ARE YOU READY TO GET DOWN", "DOWN");
		
	Mms = mouse_init(Meb);
	
	Mfms = audio_system_init();
	
	guilang_compile();
	
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
	glutInitWindowSize(800, 600);
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
	
	const GLchar *shaders[] = {
		"shaders/vshader.sdr",
		"shaders/fshader.sdr"
	};
	Msp = shader_init(shaders);
	
	printf("init done!\n");
	glutMainLoop();

}

void
update(int v)
{
	glutPostRedisplay();
	TIME += DTIME;

	shader_update1f(Msp, "time", TIME);
	//shader_update1fv(Msp, "dials", SDR_NUMDIALS, gui_dials);
	
	//gui_update(TIME, DTIME);
	
	glutTimerFunc(16, update, v);
}

void
draw()
{	
	
	glActiveTexture(GL_TEXTURE0);
	shader_update1i(Msp, "tex", 0);
	glBindTexture(GL_TEXTURE_2D, Msp->texture);
	
	//gui_draw(TIME, DTIME);
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
	aspectratio = (float)w/(float)h;
	
	//gui_resize(w, h);

	// Set up the projection view matrix (not very well!)
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, aspectratio, 0.1, 100.0 );

	// Select the Modelview matrix
    glMatrixMode( GL_MODELVIEW );
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
			shader_gentex(&(Msp->texture));
		break;
		case GLUT_KEY_DOWN:
		break;
	}
}

void
quit
(
	void
)
{
	errorlog_free(Mlog);
	exit(0);
}