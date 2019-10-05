#include "R3DNoise.h"

#define F2 0.366025403784438
#define F3 0.3333333333333333333

#define G2 0.211324865405187
#define G3 0.1666666666666666667


#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

double gradient(int hash, double x);
double gradient(int hash, double x, double y);
double gradient(int hash, double x, double y, double z);

double gradient(int hash, double x)
{
	int h = hash & 15;
	double grad = 1.0 + (h & 7);

	if (h & 8)
	{
		grad = -grad;
	}

	return grad * x;
}

double gradient(int hash, double x, double y)
{
	int h = hash & 7;
	double u = h < 4 ? x : y;
	double v = h < 4 ? y : x;
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

double gradient(int hash, double x, double y, double z)
{
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

double R3DNoise::Noise1D(double x, double inFactor)
{
	x *= inFactor;

	int i = FASTFLOOR(x);
	int i1 = i + 1;

	double x0 = x - i;
	double x1 = x0 - 1.0;

	double t = 1.0 - x0 * x0;
	t *= t;
	double n0 = t * t * gradient(R3DNoise::perm[i & 0xff], x0);

	double t1 = 1.0 - x1 * x1;
	t1 *= t1;
	double n1 = t1 * t1 * gradient(R3DNoise::perm[i1 & 0xff], x1);

	return 0.395 * (n0 + n1);
}

double R3DNoise::Noise2D(double x, double y, double inFactor)
{
	x *= inFactor;
	y *= inFactor;

	double n0, n1, n2;

	double xs = x + ((x + y) * F2);
	double ys = y + ((x + y) * F2);

	int i = FASTFLOOR(xs);
	int j = FASTFLOOR(ys);

	double t = (static_cast<double>(i) + static_cast<double>(j)) * G2;
	double X0 = i - t;
	double Y0 = j - t;
	double x0 = x - X0;
	double y0 = y - Y0;


	int i1, j1;
	if (x0 > y0)
	{ 
		i1 = 1; 
		j1 = 0; 
	}
	else 
	{ 
		i1 = 0; 
		j1 = 1; 
	}						

	double x1 = x0 - i1 + G2;
	double y1 = y0 - j1 + G2;
	double x2 = x0 - 1.0 + 2.0 * G2;
	double y2 = y0 - 1.0 + 2.0 * G2;

	int ii = i & 0xff;
	int jj = j & 0xff;


	double t0 = 0.5 - x0 * x0 - y0 * y0;
	if (t0 < 0.0f)
	{
		n0 = 0.0f;
	}
	else 
	{
		t0 *= t0;
		n0 = t0 * t0 * gradient(R3DNoise::perm[ii + R3DNoise::perm[jj]], x0, y0);
	}

	double t1 = 0.5 - x1 * x1 - y1 * y1;
	if (t1 < 0.0)
	{
		n1 = 0.0;
	}
	else 
	{
		t1 *= t1;
		n1 = t1 * t1 * gradient(R3DNoise::perm[ii + i1 + R3DNoise::perm[jj + j1]], x1, y1);
	}

	double t2 = 0.5 - x2 * x2 - y2 * y2;
	if (t2 < 0.0)
	{
		n2 = 0.0;
	}
	else {
		t2 *= t2;
		n2 = t2 * t2 * gradient(R3DNoise::perm[ii + 1 + R3DNoise::perm[jj + 1]], x2, y2);
	}

	return 40.0 / 0.884343445 * (n0 + n1 + n2);
}

double R3DNoise::Noise3D(double x, double y, double z, double inFactor)
{
	x *= inFactor;
	y *= inFactor;
	z *= inFactor;

	double n0, n1, n2, n3;

	double xs = x + ((x + y + z) * F3);
	double ys = y + ((x + y + z) * F3);
	double zs = z + ((x + y + z) * F3);

	int i = FASTFLOOR(xs);
	int j = FASTFLOOR(ys);
	int k = FASTFLOOR(zs);

	double t = (static_cast<double>(i) + static_cast<double>(j) + static_cast<double>(k)) * G3;
	double X0 = i - t;
	double Y0 = j - t;
	double Z0 = k - t;
	double x0 = x - X0;
	double y0 = y - Y0;
	double z0 = z - Z0;

	
	int i1, j1, k1;
	int i2, j2, k2;

	if (x0 >= y0)
	{
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
		else if (x0 >= z0) 
		{ 
			i1 = 1;
			j1 = 0;
			k1 = 0;
			i2 = 1;
			j2 = 0;
			k2 = 1;
		}
		else 
		{ 
			i1 = 0;
			j1 = 0;
			k1 = 1;
			i2 = 1;
			j2 = 0;
			k2 = 1;
		}
	}
	else 
	{
		if (y0 < z0)
		{ 
			i1 = 0;
			j1 = 0;
			k1 = 1;
			i2 = 0;
			j2 = 1;
			k2 = 1;
		}
		else if (x0 < z0) 
		{ 
			i1 = 0;
			j1 = 1;
			k1 = 0;
			i2 = 0;
			j2 = 1;
			k2 = 1;
		}
		else 
		{ 
			i1 = 0;
			j1 = 1;
			k1 = 0;
			i2 = 1;
			j2 = 1;
			k2 = 0;
		}
	}

	
	double x1 = x0 - i1 + G3;
	double y1 = y0 - j1 + G3;
	double z1 = z0 - k1 + G3;
	double x2 = x0 - i2 + 2.0 * G3;
	double y2 = y0 - j2 + 2.0 * G3;
	double z2 = z0 - k2 + 2.0 * G3;
	double x3 = x0 - 1.0 + 3.0 * G3;
	double y3 = y0 - 1.0 + 3.0 * G3;
	double z3 = z0 - 1.0 + 3.0 * G3;

	
	int ii = i & 0xff;
	int jj = j & 0xff;
	int kk = k & 0xff;

	double t0 = 0.6 - x0 * x0 - y0 * y0 - z0 * z0;
	if (t0 < 0.0) n0 = 0.0;
	else {
		t0 *= t0;
		n0 = t0 * t0 * gradient(perm[ii + perm[jj + perm[kk]]], x0, y0, z0);
	}

	double t1 = 0.6 - x1 * x1 - y1 * y1 - z1 * z1;
	if (t1 < 0.0) n1 = 0.0;
	else {
		t1 *= t1;
		n1 = t1 * t1 * gradient(perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]], x1, y1, z1);
	}

	double t2 = 0.6 - x2 * x2 - y2 * y2 - z2 * z2;
	if (t2 < 0.0) n2 = 0.0;
	else {
		t2 *= t2;
		n2 = t2 * t2 * gradient(perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]], x2, y2, z2);
	}

	double t3 = 0.6 - x3 * x3 - y3 * y3 - z3 * z3;
	if (t3 < 0.0) n3 = 0.0;
	else {
		t3 *= t3;
		n3 = t3 * t3 * gradient(perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]], x3, y3, z3);
	}

	return 32.0 * (n0 + n1 + n2 + n3);
}
