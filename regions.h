#ifndef _REGIONS_H_
#define _REGIONS_H_

class Rect
{
public:

	Rect():upper(-1),left(-1),height(0),width(0) {}
	Rect(int u, int l, int h, int w) :upper(u), left(l), height(h), width(w) {}

	Rect &operator=(const Rect &r);
	bool operator==(const Rect &r);
	
	int upper;
	int left;
	int height;
	int width;
};

class MinMax {
public:
	MinMax() :min(0), max(0) {}
	MinMax(int mn, int mx) :min(mn), max(mx) {}
	int min;
	int max;
};

#endif