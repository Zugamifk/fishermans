const char* SpriteVshaders[] = {
	SPRITE_VERTSHADERPATH
};
const int SpriteVshaderct = 1;

const char* SpriteFshaders[] = {
	SPRITE_FRAGSHADERPATH
};
const int SpriteFshaderct = 1;

shaderprogram*
Spriteshader_init
(
	errorlog* log
)
{
	shaderprogram* sp = shaderprogram_init(SpriteVshaders, SpriteVshaderct, SpriteFshaders, SpriteFshaderct, log);
	
	return sp;
}