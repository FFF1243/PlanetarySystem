#define _USE_MATH_DEFINES
#include <cmath>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include "Planet.h"
#include <string>
using namespace std;

int PlanetColor::numberOfPoints = 0;
point3** PlanetColor::colors = nullptr;
int StarColor::numberOfPoints = 0;
point3** StarColor::colors = nullptr;

void PlanetColor::InitColor(int number)
{
	numberOfPoints = number;
	colors = new point3*[numberOfPoints];
	for (int i = 0; i < numberOfPoints; i++)
	{
		colors[i] = new point3[numberOfPoints];
	}

	for (int i = 0; i < numberOfPoints; i++)
	{
		for (int j = 0; j < numberOfPoints; j++)
		{
			colors[i][j][0] = float(rand() / (float)RAND_MAX);
			colors[i][j][1] = float(rand() / (float)RAND_MAX);
			colors[i][j][2] = float(rand() / (float)RAND_MAX);
		}
	}
}

point3** PlanetColor::GetColors(int number)
{
	if (colors == nullptr)
		InitColor(number);

	return colors;
}

void StarColor::InitColor(int number)
{
	numberOfPoints = number;
	colors = new point3*[numberOfPoints];
	for (int i = 0; i < numberOfPoints; i++)
	{
		colors[i] = new point3[numberOfPoints];
	}

	for (int i = 0; i < numberOfPoints; i++)
	{
		for (int j = 0; j < numberOfPoints; j++)
		{
			colors[i][j][0] = float(1.0);
			colors[i][j][1] = float(1.0);
			colors[i][j][2] = float(0.0);
		}
	}
}

point3** StarColor::GetColors(int number)
{
	if (colors == nullptr)
		InitColor(number);

	return colors;
}

void Planet::initArrays()
{
	GLfloat u = (1.0 / (GLfloat)numberOfPoints);
	GLfloat v = (1.0 / (GLfloat)numberOfPoints);

	points = new GLfloat**[numberOfPoints + 1];
	normals = new GLfloat**[numberOfPoints + 1];
	textures = new GLfloat**[numberOfPoints + 1];
	for (int i = 0; i < numberOfPoints + 1; i++)
	{
		points[i] = new GLfloat*[numberOfPoints + 1];
		normals[i] = new GLfloat*[numberOfPoints + 1];
		textures[i] = new GLfloat*[numberOfPoints + 1];
	}

	for (int i = 0; i < numberOfPoints + 1; i++)
	{

		for (int j = 0; j < numberOfPoints + 1; j++)
		{
			points[i][j] = pointCoordinates(u, v);
			normals[i][j] = pointNormal(u, v,i,j);
			textures[i][j] = vecTexture(u, v);
			v = v + (1.0 / (GLfloat)numberOfPoints);
		}
		v = (1.0 / (GLfloat)numberOfPoints);
		u = u + (1.0 / (GLfloat)numberOfPoints);
	}
	//LIFE HACK
	u = 0.0; v = 0.0;
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < numberOfPoints; j++)
		{
			points[i][j] = pointCoordinates(u, v);
			normals[i][j] = pointNormal(u, v, i, j);
			textures[i][j] = vecTexture(u, v);
			v = v + (1.0 / (GLfloat)numberOfPoints);
		}
		v = 0.0;
		u = u + (1.0 / (GLfloat)numberOfPoints);
	}


}




GLfloat* Planet::pointCoordinates(GLfloat u, GLfloat v)
{
	GLdouble r = radius;
	GLfloat x = radius * sin(M_PI * u)*cos(2 * M_PI*v);
	GLfloat y = radius*sin(M_PI * u)*sin(2 * M_PI*v);
	GLfloat z = radius*cos(M_PI * u);

	GLfloat* result = new GLfloat[3];
	result[0] = x;
	result[1] = y;
	result[2] = z;
	return result;
}

GLfloat* Planet::pointNormal(GLfloat u, GLfloat v, int i, int j)
{
	double x_u = M_PI * radius * cos(M_PI*u)*cos(2 * M_PI*v);
	double x_v = -2 * M_PI*radius*sin(M_PI*u)*sin(2 * M_PI*v);
	double y_u = M_PI * radius * cos(M_PI*u)*sin(2 * M_PI*v);
	double y_v = 2* M_PI * radius * sin(M_PI*u)*cos(2 * M_PI*v);
	double z_u = -1 * M_PI * radius * sin(M_PI*u);
	double z_v = 0;

	float* normal = new float[3];
	normal[0] = y_u * z_v - z_u * y_v;
	normal[1] = z_u * x_v - x_u * z_v;
	normal[2] = x_u * y_v - y_u * x_v;


	double len = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));


	if (len == 0) {
		if (i == 0) normal[2] = -1;
		// zabezpieczenie przed dzieleniem przez zero
		normal[2] = -1;
		len = 1;
	}

	/*if (i >= (numberOfPoints / 2))
	{
		normal[0] *= -1;
		normal[1] *= -1;
		normal[2] *= -1;
	}*/

	normal[0] /= len;
	normal[1] /= len;
	normal[2] /= len;

	return normal;
}

GLfloat* Planet::vecTexture(GLfloat u, GLfloat v)
{
		float* vec = new float[2];
			vec[0] = u,
			vec[1] = v;

		return vec;
}

void Planet::disposeArrays()
{
	if (points != nullptr)
	{
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				delete[] points[i][j];
			}
			delete[]points[i];
		}
		delete[]points;

		points = nullptr;
	}

	if (normals != nullptr)
	{
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				delete[] normals[i][j];
			}
			delete[]normals[i];
		}
		delete[]normals;

		normals = nullptr;
	}

	if (textures != nullptr)
	{
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				delete[] textures[i][j];
			}
			delete[]textures[i];
		}
		delete[]textures;

		textures = nullptr;
	}
}

GLbyte* Planet::LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{

	/*************************************************************************************/

	// Struktura dla nag³ówka pliku  TGA


#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;


	/*************************************************************************************/

	// Wartoœci domyœlne zwracane w przypadku b³êdu

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	auto err = fopen_s(&pFile, FileName, "rb");
	if (pFile == NULL)
		return NULL;

	/*************************************************************************************/
	// Przeczytanie nag³ówka pliku 


	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);


	/*************************************************************************************/

	// Odczytanie szerokoœci, wysokoœci i g³êbi obrazu

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;


	/*************************************************************************************/
	// Sprawdzenie, czy g³êbia spe³nia za³o¿one warunki (8, 24, lub 32 bity)

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/

	// Obliczenie rozmiaru bufora w pamiêci


	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;


	/*************************************************************************************/

	// Alokacja pamiêci dla danych obrazu


	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}


	/*************************************************************************************/

	// Ustawienie formatu OpenGL


	switch (sDepth)

	{

	case 3:

		*ImFormat = GL_BGR_EXT;

		*ImComponents = GL_RGB8;

		break;

	case 4:

		*ImFormat = GL_BGRA_EXT;

		*ImComponents = GL_RGBA8;

		break;

	case 1:

		*ImFormat = GL_LUMINANCE;

		*ImComponents = GL_LUMINANCE8;

		break;

	};



	fclose(pFile);



	return pbitsperpixel;
}

void Planet::initTexture()
{
	// Zmienne dla obrazu tekstury

	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//  Definicja materia³u z jakiego zrobiony jest czajnik
	//  i definicja Ÿród³a œwiat³a

	/*************************************************************************************/

	// Teksturowanie bêdzie prowadzone tyko po jednej stronie œciany

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	/*************************************************************************************/

	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga
		tgaImage = LoadTGAImage(textureName.c_str(), &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/

	// Zdefiniowanie tekstury 2-D

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, tgaImage);

	/*************************************************************************************/

	free(tgaImage);
	/*************************************************************************************/

	// W³¹czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/

	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/

	// Okreœlenie sposobu nak³adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void glVertex3f(float* tab)
{
	glVertex3f(tab[0], tab[1], tab[2]);
}

void Planet::DrawMe(int Model)
{
	initArrays();
	point3** colors;

	if (isPlanet)
		colors = PlanetColor::GetColors(numberOfPoints);
	else
		colors = StarColor::GetColors(numberOfPoints);

	if(elipse)
		elipse->DrawMe();

	glColor3f(0.0, 0.0, 0.0);
	glTranslated(x, y, z);
	if (Model == 1)	//punkty
	{
		glBegin(GL_POINTS);
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				glVertex3f(points[i][j][0], points[i][j][1], points[i][j][2]);
			}
		}
		glEnd();
	}
	else if (Model == 2)
	{
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				glBegin(GL_LINES);		//PIONOWE
				glVertex3f(points[i][j]);
				glVertex3f(points[(i + 1) % numberOfPoints][j]);
				glEnd();
				glBegin(GL_LINES);		//POZIOME
				{
					glVertex3f(points[i][j]);
					glVertex3f(points[i][(j + 1) % numberOfPoints]);
				}
				glEnd();
				glBegin(GL_LINES);	//UKOSNE
				{
					glVertex3f(points[(i + 1) % numberOfPoints][j]);
					glVertex3f(points[i][(j + 1) % numberOfPoints]);
				}

				glEnd();
			}
		}
	}
	else if (Model == 3)
	{
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				/*if (j + 1 == N)
				{
				if (i > 0)
				{
				glBegin(GL_TRIANGLES);
				glColor3fv(Colors[i][j]);
				glVertex3fv(points[i][j]);
				glColor3fv(Colors[(i + 1) % N][j]);
				glVertex3fv(points[(i + 1) % N][j]);
				glColor3fv(Colors[N - i][0]);
				glVertex3fv(points[N - i][0]);
				glEnd();
				}
				glBegin(GL_TRIANGLES);
				glColor3fv(Colors[(i + 1) % N][j]);
				glVertex3fv(points[(i + 1) % N][j]);
				glColor3fv(Colors[(N - i) % N][0]);
				glVertex3fv(points[(N - i) % N][0]);
				glColor3fv(Colors[N - i - 1][0]);
				glVertex3fv(points[N - i - 1][0]);
				glEnd();
				}
				else*/
				{
					glBegin(GL_TRIANGLES);
					glColor3fv(colors[i][j]);
					glVertex3fv(points[i][j]);
					glColor3fv(colors[(i + 1) % numberOfPoints][j]);
					glVertex3fv(points[(i + 1) % numberOfPoints][j]);
					glColor3fv(colors[i][(j + 1) % numberOfPoints]);
					glVertex3fv(points[i][(j + 1) % numberOfPoints]);
					glEnd();

					glBegin(GL_TRIANGLES);
					glColor3fv(colors[(i + 1) % numberOfPoints][j]);
					glVertex3fv(points[(i + 1) % numberOfPoints][j]);
					glColor3fv(colors[(i + 1) % numberOfPoints][(j + 1) % numberOfPoints]);
					glVertex3fv(points[(i + 1) % numberOfPoints][(j + 1) % numberOfPoints]);
					glColor3fv(colors[i][(j + 1) % numberOfPoints]);
					glVertex3fv(points[i][(j + 1) % numberOfPoints]);
					glEnd();
				}
			}
		}
	}
	else if (Model == 4)
	{
		point3 white = { 1.0,1.0,1.0 };
		point3 gray = { 192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0 };
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				{
					//PIERWSZY TROJKAT
					glBegin(GL_TRIANGLES);
					glColor3fv(white);
					glVertex3fv(points[i][j]);
					glColor3fv(white);
					glVertex3fv(points[(i + 1) % numberOfPoints][j]);
					glColor3fv(white);
					glVertex3fv(points[i][(j + 1) % numberOfPoints]);
					glEnd();

					//DRUGI TROJKAT
					glBegin(GL_TRIANGLES);
					glColor3fv(gray);
					glVertex3fv(points[(i + 1) % numberOfPoints][j]);
					glColor3fv(gray);
					glVertex3fv(points[(i + 1) % numberOfPoints][(j + 1) % numberOfPoints]);
					glColor3fv(gray);
					glVertex3fv(points[i][(j + 1) % numberOfPoints]);
					glEnd();
				}
			}
		}
	}
	else if(Model == 5)
	{
		glColor3f(0.0, 0.0, 0.0);
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				
				glBegin(GL_TRIANGLES);
				glNormal3fv(normals[i][(j + 1) % numberOfPoints]);
				glVertex3fv(points[i][(j + 1) % numberOfPoints]);
				glNormal3fv(normals[(i + 1) % numberOfPoints][j]);
				glVertex3fv(points[(i + 1) % numberOfPoints][j]);
				glNormal3fv(normals[(i + 1) % numberOfPoints][(j + 1) % numberOfPoints]);
				glVertex3fv(points[(i + 1) % numberOfPoints][(j + 1) % numberOfPoints]);
				glEnd();

				glBegin(GL_TRIANGLES);
				glNormal3fv(normals[i][j]);
				glVertex3fv(points[i][j]);
				glNormal3fv(normals[(i + 1) % numberOfPoints][j]);
				glVertex3fv(points[(i + 1) % numberOfPoints][j]);
				glNormal3fv(normals[i][(j + 1) % numberOfPoints]);
				glVertex3fv(points[i][(j + 1) % numberOfPoints]);
				glEnd();
			}
		}
	}
	else if(Model == 6)
	{
		glColor3f(0.0, 0.0, 0.0);
		if (textureName != "")
		{
			initTexture();
		}
		for (int i = 0; i < numberOfPoints; i++)
		{
			for (int j = 0; j < numberOfPoints; j++)
			{
				glBegin(GL_TRIANGLES);
					glNormal3fv(normals[i][(j + 1)]);
					glTexCoord2fv(textures[i][(j + 1) ]);
					glVertex3fv(points[i][(j + 1)]);

					glNormal3fv(normals[(i + 1)][j]);
					glTexCoord2fv(textures[(i + 1)][j]);
					glVertex3fv(points[(i + 1)][j]);

					glNormal3fv(normals[(i + 1)][(j + 1)]);
					glTexCoord2fv(textures[(i + 1) ][(j + 1) ]);
					glVertex3fv(points[(i + 1) ][(j + 1) ]);
				glEnd();

				glBegin(GL_TRIANGLES);
					glNormal3fv(normals[i][j]);
					glTexCoord2fv(textures[i][j]);
					glVertex3fv(points[i][j]);

					glNormal3fv(normals[(i + 1)][j]);
					glTexCoord2fv(textures[(i + 1) ][j]);
					glVertex3fv(points[(i + 1) ][j]);

					glNormal3fv(normals[i][(j + 1)]);
					glTexCoord2fv(textures[i][(j + 1) ]);
					glVertex3fv(points[i][(j + 1) ]);
				glEnd();
			}
		}
	}

	disposeArrays();
}

Planet::~Planet()
{
	disposeArrays();
}
