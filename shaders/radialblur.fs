uniform float time;
uniform sampler2D tex;
varying vec2 texc;

const int numsamples = 40;
const float samplestr = 2.2;
const float sampledist = 2.0;

void main (void)  
{     

	float speed = 1.0 * time;
	vec2 offset = vec2(sin(speed), cos(speed));
	vec2 dir = 0.5 - texc;
	float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
	dir = dir/dist;
	vec2 samplestep = dir/numsamples;
	vec2 pt = dir;

	vec4 c = texture2D(tex,texc);
	
	vec4 sum = c;
	for (int i = 0; i< numsamples; i++) {
		sum += texture2D(tex, offset+texc + dist * sampledist * pt);
		pt += samplestep;
	}
	
	sum /= numsamples;
	
	gl_FragColor = sum*sum+gl_Color;  
}