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

#define DATAPATH data/

#include "lookup3.c"
#include "math/perlin.c"
#include "math/vector.h"
#include "math.c"

#include "stack.c"
#include "list.c"
#include "hashtable.c"
#include "set.c"

#include "event/bus.h"

#include "loader.c"
#include "errorlog.c"

#include "mouse.c"

#include "audio/audio.h"

#include "color.c"
#include "font/font.h"
#include "shapes.c"

#include "shader/shader.h"

#include "gui/gui.h"