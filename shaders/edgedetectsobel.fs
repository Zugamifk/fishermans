uniform float time;
uniform sampler2D tex;
varying vec2 texc;

const int numsamples = 40;
const float samplestr = 2.2;
const float sampledist = 2.0;

float 
get(float dx, float dy)
{
	float glow = 0.5*sin(5.0*time)+1.5;
	vec2 pt = texc + vec2(dx, dy)*glow*0.01;
	vec4 c = texture2D(tex, pt);
	return 0.2126*c.r + 0.7152*c.g + 0.0722*c.b;
}

void main (void)  
{     

	vec2 g = 
		vec2(get(1.0, 1.0), get(1.0, 1.0)) +
		vec2(get(1.0, 0.0), get(0.0, 1.0))*2.0 +
		vec2(get(1.0, -1.0), get(-1.0, 1.0)) -
		vec2(get(-1.0, 1.0), get(1.0, -1.0)) -
		vec2(get(-1.0, 0.0), get(0.0, -1.0))*2.0 -
		vec2(get(-1.0, -1.0), get(-1.0, -1.0));
	
	float g1 = g.x*g.x+g.y*g.y;
	
	gl_FragColor = texture2D(tex, texc) + vec4(g1, g1, g1, 1.0);
}