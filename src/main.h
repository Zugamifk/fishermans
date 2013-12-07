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

#include "math/funtable.c"
#include "ext/lookup3.c"
#include "math/perlin.c"
#include "math/vector.h"
#include "math.c"

#include "timers.h"
#include "structures/datastructures.h"

#include "event/bus.h"

#include "loader.c"
#include "errorlog.c"

#include "mouse.c"

#include "color.c"
#include "font/font.h"
#include "shapes.c"

#include "shader/shader.h"

#include "lang/lang_tools.h"
#include "audio/audio.h"
#include "gui/gui.h"

#define SPRITE_PATH app/Sprite
#include "../Sprite/src/sprite.h"