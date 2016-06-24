#include "regions.h"

Rect &Rect::operator = (const Rect &r)
{
	if (this != &r)
	{
		height = r.height;
		width = r.width;
		upper = r.upper;
		left = r.left;
	}
	return *this;
}

bool Rect::operator== (const Rect &r)
{	
	return ((r.width == width) && (r.height == height) && (r.upper == upper) && (r.left == left));
}







