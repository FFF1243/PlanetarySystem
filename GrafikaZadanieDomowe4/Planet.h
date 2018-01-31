#pragma once


#pragma once
#ifndef STAR_HEADER
#define STAR_HEADER
#define _USE_MATH_DEFINES
#include <gl/gl.h>
#include <memory>
#include "Elipse.h"
#include <string>
typedef float point3[3];

class PlanetColor
{
private:
	static int numberOfPoints;
	static point3 **colors;
	static void InitColor(int number);
public:
	static point3 ** GetColors(int number);
};

class StarColor
{
private:
	static int numberOfPoints;
	static point3 **colors;
	static void InitColor(int number);
public:
	static point3 ** GetColors(int number);
};



class Planet
{
private:
	int numberOfPoints;
	GLfloat*** points;
	GLfloat*** normals;
	GLfloat*** textures;
	GLbyte* tgaImage;
	std::string textureName;
	GLdouble radius;
	std::shared_ptr<Elipse> elipse;
	bool isPlanet;
	void initArrays();
	GLfloat* pointCoordinates(GLfloat u, GLfloat v);
	GLfloat* pointNormal(GLfloat u, GLfloat v, int i, int j);
	GLfloat* vecTexture(GLfloat u, GLfloat v);
	void disposeArrays();

	void initTexture();
public:
	static GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat);
	GLdouble x;
	GLdouble y;
	GLdouble z;
	Planet() :x(0), y(0), z(0), radius(1.0), numberOfPoints(10), isPlanet(true)
	{
		initArrays();
		tgaImage = nullptr;
	}
	Planet(GLdouble xparam, GLdouble yparam, GLdouble zparam, GLdouble rad,std::shared_ptr<Elipse> eli, int numPoints, bool planet, std::string texName = "") : x(xparam), y(yparam), z(zparam), radius(rad), numberOfPoints(numPoints), isPlanet(planet), elipse(eli), textureName(texName)
	{
		initArrays();
		tgaImage = nullptr;
	}
	Planet(GLdouble xparam, GLdouble yparam, GLdouble zparam, GLdouble rad, int numPoints, bool planet) : x(xparam), y(yparam), z(zparam), radius(rad), numberOfPoints(numPoints), isPlanet(planet)
	{
		elipse = nullptr;
		tgaImage = nullptr;
		initArrays();
	}
	void DrawMe(int Model);
	~Planet();
};
#endif