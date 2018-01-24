#include "stdafx.h"
#include "ColorInt.h"
// A simple color holder class to hold colors of int range from 0 ~ 255.
ColorInt::ColorInt()
{
	Red = 0;
	Green = 0;
	Blue = 0;
}

ColorInt::ColorInt(int red, int green, int blue)
{
	Red = red;
	Green = green;
	Blue = blue;
}


ColorInt::~ColorInt()
{}
