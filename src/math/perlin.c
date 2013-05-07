/* ==================================================================
 * +	perlin.c
 * +	
 * +	This file generates the 2d perlin noise used for the terrain heightmap
 * ==================================================================
 */

// A pseudo-random number generator based on two discrete values and time
float perlin_rand(float t, int x, int y) {
	x = (((x<<13)+(int)t) ^ x);
	y = (((y<<17)+(int)t) ^ y);
	return ( 1.0 - ( (x * (y * x * 15731 + 789221) + 1376312589 + x*y*543) & 0x7fffffff) / 1073741824.0); 
}

// A pseudo-random number generator taking a single argument
float perlin_rand1i(int x) {
	x = (x<<13) ^ x;
	return ( 2.0 - ( (x * (x * x * 15731 + 789221) + 1376312589 + x*x*543) & 0x7fffffff) / 1073741824.0)/2.0; 
}

// Generates 2d perlin noise for a given x and y position
float perlin_noise(
		int x, int y, 	// Position on surface
		int width, int height, 	// surface width and height
		float t, 		// Time
		unsigned int startdepth, unsigned int maxdepth	// Start and end depths for noise
	) {
	
	// time factor
	float speed = t * 4.0;

	// initialize the values for generating the noise
	float ret = 0.0;
	int frequency = 1 << startdepth;
	// Summation iterator for the noise
	int i; for(i=0;i<maxdepth-startdepth;i++) {

		// Scaled x and y values as generator seeds
		int sx = x*frequency/width;
		int sy = y*frequency/height;

		// float version of the above, used for interpolating values
		float ix  = (float)((x*frequency)%width)/(float)width;
		float iy = (float)((y*frequency)%height)/(float)height;

		// Generate a height value based on the seeds, scaled by the 
		// interpolation values
		float v	= (perlin_rand(speed, sx, sy)*ix
				+ perlin_rand(speed, sx-1, sy)*(1.0-ix)) * iy
				+ (perlin_rand(speed, sx, sy-1)*ix
				+ perlin_rand(speed, sx-1,  sy-1)*(1.0-ix)) * (1.0-iy); 			
		v	/= (float)frequency;

		// double the frequency
		frequency <<= 1;

		// Sum
		ret += v;
	}

	// return the absolute value of the summed noise value
	ret = ret>0.0?ret:-ret;
	return ret;
}
