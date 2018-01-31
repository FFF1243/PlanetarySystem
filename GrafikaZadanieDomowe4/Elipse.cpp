#include <windows.h>
#include <math.h>
#include "Elipse.h"
#define DEG2RAD 3.14159/180.0
float Elipse::getXRadius()
{
	return elipseXradius;
}

float Elipse::getYRadius()
{
	return elipseYradius;
}

float Elipse::getXFocal()
{
	return focalPoint[0];
}

float Elipse::getYFocal()
{
	return focalPoint[1];
}

void Elipse::DrawMe()
{
	int i;

	glBegin(GL_LINE_LOOP);

	for (i = 0;i<360;i++)
	{
		float rad = i*DEG2RAD;
		glVertex2f(cos(rad)*elipseXradius,
			sin(rad)*elipseYradius);
	}

	glEnd();
}

