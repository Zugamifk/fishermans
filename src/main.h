#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>
#include <ctype.h>

#define GLEW_STATIC
#include "glew/glew.c"
#include <GL/glut.h>
#include <GL/glu.h>
#include <fmod.h>
#include <fmod_errors.h>

#ifndef pi
    #define pi 3.14159
#endif

double aspectratio;
double TIME, DTIME;
typedef struct
_S_screeninfo
{
	int w;
	int h;
	int ar;
} screeninfo;

#define DATAPATH data/

#include "debug.c"
#include "hax/bittwiddle.h"

#include "timers.h"

#include "math/funtable.c"
#include "ext/lookup3.c"
#include "math/perlin.c"
#include "math/vector.h"
#include "math/noise/noise.h"
#include "math/curves/curves.h"
#include "math.c"

#include "structures/datastructures.h"

#include "event/bus.h"

#include "loader.c"
#include "errorlog.c"

#include "mouse.c"
#include "keyboard.c"

#include "color.c"
#include "font/line/font.h"
#include "font/outline/font-outline.h"
#include "shapes.c"

#include "texture/texture.h"
#include "shader/shader.h"

#include "lang/lang_tools.h"
#include "audio/audio.h"
#include "gui/gui.h"

#include "application.c"

#define SPRITE_PATH app/Sprite
#include "../Sprite/src/sprite.h"

#include "../Font Editor/src/editor.h"