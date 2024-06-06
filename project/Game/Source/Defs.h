#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdio.h>
#include <vector>

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != NULL )		\
		{					  \
	  delete x;			\
	  x = NULL;			  \
		}					  \
	}

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{							  \
	if( x != NULL )			  \
		{							\
	  delete[] x;				\
	  x = NULL;					\
		}							\
							  \
	}

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Standard string size
#define SHORT_STR	 32
#define MID_STR	    255
#define HUGE_STR   8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

inline double lerp(double A, double B, double t) {
	return A + (B - A) * t;
}

// Maths calculations
#define PI 3.14159265359f
#define DEG_2_RAD(a) a * PI / 180.0f
#define RAD_2_DEG(a) a * 180.0f / PI





#define TILES_TO_LOAD 22
#define TILESTEXTURE_TO_LOAD 4

#endif	// __DEFS_H__