#pragma once
#pragma once
#ifndef Planet_cont_HEADER
#define Planet_cont_HEADER
#include <windows.h>
#include "Planet.h"
#include <memory>
#include "Elipse.h"
#define DEG2RAD 3.14159/180.0
class PlanetController
{

private:
	std::shared_ptr<Planet> object;
	double degree;
	std::shared_ptr<Elipse> elipse;
	double getAreaFromPoints(double x1, double y1, double x2, double y2);
	GLdouble getEccentricity(float radiusX, float radiusY);
	double triangleArea(double x, double y, double z);
	double constantSurface;
public:
	static double stepRatio;
	PlanetController(std::shared_ptr<Planet> planet, std::shared_ptr<Elipse> eli, double surface) :
		object(planet), elipse(eli), constantSurface(surface)
	{
		degree = 0;
	};
	void Move();
};
#endif