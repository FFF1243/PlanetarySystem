#include "PlanetController.h"
#include <math.h>
double PlanetController::stepRatio = 0.01;

double PlanetController::getAreaFromPoints(double x1, double y1, double x2, double y2)
{
	double focalPointX = -getEccentricity(elipse->getXRadius(), elipse->getYRadius());
	double b = sqrt(pow(y1, 2) + pow(x1 - focalPointX, 2));
	double a = sqrt(pow(y2, 2) + pow(focalPointX - x2, 2));
	double c = sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));

	return triangleArea(a, b, c);
}

GLdouble PlanetController::getEccentricity(float radiusX, float radiusY)
{
	GLdouble e2 = (pow(radiusX, 2) - pow(radiusY, 2));
	GLdouble e = sqrt(e2);
	return  e;
}

double PlanetController::triangleArea(double x, double y, double z)
{
	float s = (x + y + z) / 2;
	return sqrt(s*(s - x)*(s - y)*(s - z));
}

void PlanetController::Move()
{
	float rad = degree * DEG2RAD;
	double newX;
	double newY;
	double area;
	do
	{
		degree += stepRatio;
		rad = degree * DEG2RAD;
		newX = elipse->getXRadius() * cos(rad);
		newY = elipse->getYRadius()* sin(rad);
		area = getAreaFromPoints(object->x, object->y, newX, newY);
	} while (area < constantSurface);
	object->x = newX;
	object->y = newY;
	if (degree > 360)
	{
		degree = 0;
	}
}
