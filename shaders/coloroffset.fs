uniform float time;
uniform sampler2D tex;
varying vec2 texc;

const int numsamples = 40;
const float samplestr = 2.2;
const float sampledist = 2.0;

void main (void)  
{     

	vec4 offset = vec4(0, 0, 0, 0);
	
	offset.r = texture2D(tex, texc + vec2(sin(time)/fmod(time, 2.0), 0.0));
	offset.g = texture2D(tex, texc + vec2(0.0, sin(time)/fmod(time, 3.14159)));
	offset.b = texture2D(tex, texc + vec2(0.0, sin(time)/fmod(time, 5.0)));
	
	gl_FragColor = texture2D(tex, texc) + offset/4.0;
}