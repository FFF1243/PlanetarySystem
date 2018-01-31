#pragma once
#include <gl/gl.h>

#ifndef ELIPSE_HEADER
#define ELIPSE_HEADER


class Elipse
{
private:
	float elipseXradius;
	float elipseYradius;
	float focalPoint[2];

public:
	Elipse(float xRadius, float yRadius, float xFocal, float yFocal)
	{
		//focalPoint = new float[2];
		focalPoint[0] = xFocal;
		focalPoint[1] = yFocal;
		elipseXradius = xRadius;
		elipseYradius = yRadius;
	}
	float getXRadius();
	float getYRadius();
	float getXFocal();
	float getYFocal();
	void DrawMe();


};

#endif
