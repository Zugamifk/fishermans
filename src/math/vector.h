#define vec2_print(v)		(vec4_print(v))
#define vec2_new(x, y) 		(vec4_new(x, y, 0.0, 0.0))
#define vec2_copy(v) 		(vec4_copy(v))
#define vec2_copyto(v, u) 		(vec4_copyto(v, u))
#define vec2_set(v, x, y) 	(vec4_set(v, x, y, 0.0, 0.0))
#define vec2_apply(v)		(vec4_apply(v))
#define vec2_translate(v)		(vec4_translate(v))
#define vec2_add(r, x, y)	(vec4_add(r, x, y))
#define vec2_sub(r, x, y)	(vec4_sub(r, x, y))
#define vec2_scale(r, v, s)	(vec4_scale(r, v, s))
#define vec2_length(v)		(vec4_length(v))
#define vec2_normalize(r, v)	(vec4_normalize(r, v)) 
#define vec2 vec4

#define vec3_print(v)		(vec4_print(v))
#define vec3_new(x, y, z) 	(vec4_new(x, y, z, 0.0))
#define vec3_set(v, x, y, z) 	(vec4_set(v, x, y, z, 0.0))
#define vec3_add(r, x, y)	(vec4_add(r, x, y))
#define vec3_sub(r, x, y)	(vec4_sub(r, x, y))
#define vec3_scale(r, v, s)	(vec4_scale(r, v, s))
#define vec3_length(v)		(vec4_length(v))
#define vec3_normalize(r, v)	(vec4_normalize(r, v)) 
#define vec3 vec4

#include "vector.c"
#include "matrix.c"
