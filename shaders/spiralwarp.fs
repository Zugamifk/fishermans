uniform float time;
uniform sampler2D tex;
varying vec2 texc;

const int numsamples = 40;
const float samplestr = 2.2;
const float sampledist = 2.0;

const float pi = 3.14159;

void main (void)  
{     

	vec2 dir = 0.5 - texc;
	float s = length(dir);
	float ang = acos(dot(dir/s, vec2(1.0, 0.0)));
	float wave = sin(s*pi*10 - time*10.0 + ang*sign(dir.y));
	vec2 offset = dir/s * (0.5+0.5*wave)*0.08;
		
	gl_FragColor = texture2D(tex, texc+offset);
} 