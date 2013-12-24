uniform float time;
uniform sampler2D tex;
uniform int pxside;
varying vec2 texc;

const float pi = 3.14159;

const int GUIID = 0;
const int WINDOWID = 1;
const int CELLID = 2;
const int BOXID = 3;
const int WTFID = 4;
const int NUMIDS = 5;


int
decodeid
(
	vec4 color
)
{
	float val = float(NUMIDS)*color.r;
	return val;
}
 
void main (void)  
{   
		int id = decodeid(gl_Color); 
		
		switch(id) {
			case WTFID: {
				gl_FragColor = texture2D(tex, texc);
			} break;
			default:
					gl_FragColor = vec4(gl_Color.g,gl_Color.b,gl_Color.r, 1.0);
			break;
		}
		
	//	int GMASK = (int)(gl_Color.g*255.0d);
	//	//int BMASK = (int)(gl_Color.b*255.0d);
} 