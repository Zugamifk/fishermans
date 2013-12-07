// bit twiddling macros
#define BT

#ifdef BT

// Length of a int
#define BT_CHAR_BIT 8

// Calculate the sign of x
#define BT_SIGN(x) (((x)>0)-((x)<0))

// Check if (x) and (y) are different signs
#define BT_OPPSIGN(x, y) (((x)^(y))<0)

// Get the absolute value of (x) without branching
#define _BT_ABSMASK(x) ((x) >> sizeof(int) * (BT_CHAR_BIT - 1))
#define BT_ABS(x) (((x) ^ _BT_ABSMASK((x))) - _BT_ABSMASK((x)))

// Get minimum of (x) and (y)
#define BT_MIN(x, y) ((y) ^ (((x) ^ (y)) & -((x) < (y))))

// Get maximum of (x) and (y)
#define BT_MAX(x, y) ((x) ^ (((x) ^ (y)) & (-((x) < (y)))))

// Check if (x) is power of 2
#define BT_POW2(x) ((x) && !((x)&((x)-1)))

// negate (x) if f
#define BT_IFNEG(f, x) (((x)^-f)+f)
// negate (x) if not f
#define BT_ELSENEG(f, x) ((f ^ (f - 1)) * (x))

//get the ith bit of x
#define BT_BIT(x, i) ((x & (1<<i))>>i)

#endif

#include "bittwiddle.c"