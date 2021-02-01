#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vect.h"

#define PRIVATE static
#define PUBLIC

PUBLIC Vect vInit(double x, double y, double z)
{
	Vect v;
	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

PUBLIC Vect vCirc(Vect v, int n)
{
	if (n < 0) {
		n = 3 - abs(n)%3;
	} else {
		n = n%3;
	}

	switch (n) {
	case 0 : return (vInit(v.x, v.y, v.z)); break;
	case 1 : return (vInit(v.y, v.z, v.x)); break;
	case 2 : return (vInit(v.z, v.x, v.y)); break;

	default: return (vInit(v.x, v.y, v.z)); break;
	}
}

PUBLIC Vect vAdd(Vect a, Vect b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return (a);
}

PUBLIC Vect vSub(Vect a, Vect b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return (a);
}

PUBLIC Vect vScale(Vect v, double s)
{
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return (v);
}

PUBLIC Vect vDiv(Vect v, double d)
{
	v.x /= d;
	v.y /= d;
	v.z /= d;
	return (v);
}

PUBLIC double vDot(Vect a, Vect b) { return (a.x*b.x + a.y*b.y + a.z*b.z); }

PUBLIC Vect vCross(Vect a, Vect b)
{
/*
	a = vCirc(a, 1);
	b = vCirc(b, 2);
	return (vSub(vMul(a, b), vMul(b, a)));
*/
	return (vInit(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x));
}

PUBLIC Vect vMul(Vect a, Vect b) { return (vInit(a.x*b.x, a.y*b.y, a.z*b.z)); }

PUBLIC double vLen2(Vect v) { return (vDot(v, v)); }

PUBLIC double vLen(Vect v) { return (sqrt(vLen2(v))); }

PUBLIC Vect vUnit(Vect v) { return (vDiv(v, vLen(v))); }

PUBLIC double vDif(Vect a, Vect b) { return (vLen(vSub(a, b))); }

PUBLIC Vect vLerp(Vect a, Vect b, double t)
{
	return (vAdd(vScale(a, 1.0 - t), vScale(b, t)));
}

PUBLIC Vect vSlerp(Vect a, Vect b, double t)
{
	double c, s, w;

	a = vUnit(a);
	b = vUnit(b);
	c = vDot(a, b);
	s = sqrt(1.0 - c*c);
	w = atan2(s, c);
	return (vAdd(vScale(a, sin(w*(1.0 - t))/s), vScale(b, sin(w*t)/s)));
}

PUBLIC Vect vMax(Vect a, Vect b)
{
	if (b.x > a.x) a.x = b.x;
	if (b.y > a.y) a.y = b.y;
	if (b.z > a.z) a.z = b.z;
	return (a);
}

PUBLIC Vect vMin(Vect a, Vect b)
{
	if (b.x < a.x) a.x = b.x;
	if (b.y < a.y) a.y = b.y;
	if (b.z < a.z) a.z = b.z;
	return (a);
}

PUBLIC void vPrint(FILE *fp, Vect v)
{
	fprintf(fp, "%e\t%e\t%e\n", v.x, v.y, v.z);
}

#ifdef __cplusplus
}
#endif

