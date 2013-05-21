#ifndef _MATH_EXTENSION_H_
#define _MATH_EXTENSION_H_

#include <math.h>

class Math {
public:
	// Checks of two floats are close enough to be considered equal
	static bool floatEquality(float a, float b) {
		return fabsf((a - b) / ((b == 0.0f) ? 1.0f : b)) < 1e-6f;
	}

	// Returns the -1 if x is negative, 0 if x is 0, and 1 if x is positive
	static int floatSign(float x) {
		if (x > 0) return true;
		return floatEquality(x, 0.0f) ? 0 : -1;
	}
};
#endif