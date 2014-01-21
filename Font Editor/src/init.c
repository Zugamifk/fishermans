char* modestring = "GRID";
char* charstr;
int charpos = 32;
#define EDITOR_GRIDMAX 16
int gridw = 0, gridh = 0;
int *cells;

gridfont *currentfont;

void
editor_initvars
()
{
	charstr = malloc(256);
	sprintf(charstr, "?");
	gridw = 16;
	gridh = 16;
	cells = malloc(sizeof(int)*EDITOR_GRIDMAX*EDITOR_GRIDMAX);
	for (int i = 0; i < EDITOR_GRIDMAX*EDITOR_GRIDMAX; i++) {
		cells[i] = 0;
	}
	currentfont = gridfont_init();
}

void
editor_bindguivars
(
	hashtable *vars
)
{
	hashtable_insert(vars, "TEXTMODE", &modestring);
	hashtable_insert(vars, "CURRENTCHAR", &charstr);
}

void
editor_bindshadervars
(
	shaderprogram *shaders
)
{
	shaderprogram_addvar(shaders, "cells", cells, SHADER_INT1V, EDITOR_GRIDMAX*EDITOR_GRIDMAX);
}

void
editor_canvasclick
(void* data) 
{
	gui_viewport* viewport = (gui_viewport*)data;
	int x = gridw*viewport->mousex;
	int y = gridh*viewport->mousey;
	
	cells[x+y*gridw] = cells[x+y*gridw] ^ 1;
	modestring = "POO";
}

void
editor_resetbutton
(void* data)
{
	for (int i = 0; i < EDITOR_GRIDMAX*EDITOR_GRIDMAX; i++) {
		cells[i] = 0;
	}
}

void
editor_charsucc
(void *data)
{
	if (charpos < 255) {
		charpos++;
		sprintf(charstr, "%c", charpos);
	}
}

void
editor_charprev
(void *data)
{
	if (charpos > 33) {
		charpos--;
		sprintf(charstr, "%c", charpos);
	}
}

void
editor_charsave
(void* data)
{
	vec2 **pts = malloc(sizeof(vec2*)*EDITOR_GRIDMAX*EDITOR_GRIDMAX);
	int num = 0;
	int w=0, h=0;
	int x, y;
	for (int i = 0; i < EDITOR_GRIDMAX*EDITOR_GRIDMAX; i++) {
		if (cells[i]==1) {
			x = i%gridw;
			y = i/gridw;
			if (x > w) w = x;
			if (y > h) h = y;
			pts[num] = vec2_new((float)x, (float)y);
			num++;
		}
	}
	currentfont->chars[charpos] = gridfont_char_init(pts, num, w, h);
}

// viewport draw callback
// =================================
void
editor_viewportdrawcb(gui_style* style, gui_viewport* gv, double t, double dt) {

	color* c = gui_style_getcolor(style, GUISTYLE_WTF);
	float w = gv->dim->w;
	float h = gv->dim->h;
	
	float tw = 1.0;
	float th = 1.0;
	float txi = tw/(float)EDITOR_GRIDMAX;
	float tyi = th/(float)EDITOR_GRIDMAX;
	
	glColor3d(0.7,0.8,0.4);
	glPushMatrix();
	float s = w/128.0;
	glScaled(s,s,1.0);
	for (int i = 33; i < 255; i++) {
		glTranslated(16.0,0.0,0.0);
		gridfont_char *c = currentfont->chars[i];
		if (c!= NULL) {
			gridfont_char_draw(c);
		}
	}
	glPopMatrix();
	
	glBegin(GL_LINES);
	glColor3d(0.0,0.2,0.3);
	for (double x = txi; x < 1.0; x = x + txi) {
		glVertex2d(x*w, 0.0);
		glVertex2d(x*w, h);
	}
	bool base = false, mid = false, cap = false;
	for (double y = tyi; y < 1.0; y = y + tyi) {
		if (!base && y > 0.2) {
			glColor3d(0.3,0.5,0.8);
			base = true;
		} else
		if (!mid && y > 0.6) {
			glColor3d(0.3,0.3,0.8);
			mid = true;
		} else
		if (!cap && y > 0.9) {
			glColor3d(0.5,0.3,0.6);
			cap = true;
		} else glColor3d(0.0,0.2,0.3);
		glVertex2d(0.0, y*h);
		glVertex2d(w, y*h);
	}
	glEnd();
	
	color_apply(c);

	glEnable(GL_TEXTURE_2D); 
		
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2d(0.0, 0.0); 
	
	glTexCoord2f(tw, 0.0);
	glVertex2d(w, 0.0);
	
	glTexCoord2f(tw,th);
	glVertex2d(w, h);
	
	glTexCoord2f(0.0, th);
	glVertex2d(0.0, h);
	
	glEnd();
	glDisable(GL_TEXTURE_2D); 
}
