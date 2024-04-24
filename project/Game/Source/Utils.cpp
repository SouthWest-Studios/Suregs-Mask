#include <corecrt_math.h>
#include "Point.h"

inline float dist(int ax, int ay, int bx, int by) {
	return sqrt(pow(ax - bx, 2) + (pow(ay - by, 2)));
}
inline float dist(iPoint a, iPoint b) {
	return sqrt(pow(a.x - b.x, 2) + (pow(a.y - b.y, 2)));
}
