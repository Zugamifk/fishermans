#define SPRITETEX_WIDTH (1<<9)
#define SPRITETEX_HEIGHT (1<<9)

void
Spritetex_perlin
(GLuint *tex)
{

	glDeleteTextures(1, tex);
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	int w = SPRITETEX_WIDTH;
	int h = SPRITETEX_HEIGHT;
	BYTE *data = malloc(w*h*3);
	float r = rand()%1000;
	float g = rand()%1000;
	float b = rand()%1000;
	for(int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int i = (x*h + y)*3;
			int s = 0, d = 10;
			data[i] = 255*perlin_noise(x, y, w, h, r, s, d);
			data[i+1] = 255*perlin_noise(x, y, w, h, g, s, d);
			data[i+2] = 255*perlin_noise(x, y, w, h, b, s, d);
		}
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
}

void
Spritetex_2dgradient
(GLuint *tex)
{

	glDeleteTextures(1, tex);
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	int w = SPRITETEX_WIDTH;
	int h = SPRITETEX_HEIGHT;
	BYTE *data = malloc(w*h*3);
	for(int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int i = (x*h + y)*3;
			double px = (double)x/(double)w;
			double py = (double)y/(double)h;
			data[i] = 0;
			data[i+1] = 255*px;
			data[i+2] = 255*py;
		}
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
}