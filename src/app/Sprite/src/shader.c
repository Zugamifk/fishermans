const char* SpriteVshaders[] = {
	SPRITE_VERTSHADERPATH
};
const int SpriteVshaderct = 1;

const char* SpriteFshaders[] = {
	SPRITE_FRAGSHADERPATH
};
const int SpriteFshaderct = 1;

const int SPRITESHADER_GUIID = 0;
const int SPRITESHADER_WINDOWID = 1;
const int SPRITESHADER_CELLID = 2;
const int SPRITESHADER_BUTTONID = 3;
const int SPRITESHADER_PARTICLEID = 4;
const int SPRITESHADER_NUMIDS = 5;

shaderprogram*
Spriteshader_init
(
	errorlog* log
)
{
	shaderprogram* sp = shaderprogram_init(SpriteVshaders, SpriteVshaderct, SpriteFshaders, SpriteFshaderct, log);
	
	return sp;
}

color*
Spriteshader_encodecolor
(
	unsigned int id
)
{
	double fac = (double)id;
	double den = (double)SPRITESHADER_NUMIDS;
	printf("%f %f", fac, den);
	return color_new4(fac/den, 0.0, 0.0, 1.0);
}

color**
Spriteshader_gencolors
(
	int* len
)
{
	*len = SPRITESHADER_NUMIDS;
	color** pack = malloc(sizeof(color*)*SPRITESHADER_NUMIDS);
	pack[SPRITESHADER_GUIID] =  Spriteshader_encodecolor(SPRITESHADER_GUIID);//gui
	pack[SPRITESHADER_WINDOWID] = Spriteshader_encodecolor(SPRITESHADER_WINDOWID); //window
	pack[SPRITESHADER_CELLID] = Spriteshader_encodecolor(SPRITESHADER_CELLID); //cell
	pack[SPRITESHADER_BUTTONID] = Spriteshader_encodecolor(SPRITESHADER_BUTTONID); //button
	pack[SPRITESHADER_PARTICLEID] = Spriteshader_encodecolor(SPRITESHADER_PARTICLEID); //button
	return pack;
}