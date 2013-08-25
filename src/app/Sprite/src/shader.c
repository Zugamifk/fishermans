const char* SpriteVshaders[] = {
	SPRITE_VERTSHADERPATH
};
const int SpriteVshaderct = 1;

const char* SpriteFshaders[] = {
	SPRITE_FRAGSHADERPATH
};
const int SpriteFshaderct = 1;

#define SPRITESHADER_GUICOLOR 1.0, 0.0, 0.0, 1.0
#define SPRITESHADER_WINDOWCOLOR 0.8, 0.0, 0.0, 1.0
#define SPRITESHADER_CELLCOLOR 0.6, 0.0, 0.0, 1.0

shaderprogram*
Spriteshader_init
(
	errorlog* log
)
{
	shaderprogram* sp = shaderprogram_init(SpriteVshaders, SpriteVshaderct, SpriteFshaders, SpriteFshaderct, log);
	
	return sp;
}