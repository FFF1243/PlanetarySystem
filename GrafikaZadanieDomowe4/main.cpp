#define _USE_MATH_DEFINES
#include <windows.h>
#include <cmath>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include "Planet.h"
#include "PlanetController.h"
#include "Elipse.h"
#define DEG2RAD 3.14159/180.0
const double elipseXradius = 8.0;
const double elipseYradius = 5.0;
const double secondElipseXradius = 9;
//////////////////////////////////////////////////////////
typedef float point3[3];
bool Move = false;
bool Spin = false;
int Model = 1;
float degree = 0;
int NumberOfPoints = 35;
float moveFactor = 0.05;
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
//FIRST ELIPSE:	//A^2 - B^2 = 39
double focalPointX = sqrt((pow(elipseXradius, 2) - pow(elipseYradius, 2)));
std::shared_ptr<Elipse> firstElipse = std::make_shared<Elipse>(8, 5, focalPointX, 0);
//SECOND ELIPSE:
std::shared_ptr<Elipse> secondElipse = std::make_shared<Elipse>(secondElipseXradius, sqrt(pow(secondElipseXradius, 2) - (pow(elipseXradius, 2) - pow(elipseYradius, 2))), focalPointX, 0);
//PLANETS:
std::shared_ptr<Planet> planet = std::make_shared<Planet>(elipseXradius, 0.0, 0.0, 0.2,firstElipse ,NumberOfPoints, true, "first.tga");
std::shared_ptr<Planet> secondPlanet = std::make_shared<Planet>(-secondElipseXradius, 0.0, 0.0, 0.3,secondElipse, NumberOfPoints, true, "second.tga");
std::shared_ptr<Planet> sun = std::make_shared<Planet>(-focalPointX, 0.0, 0.0, 0.5, NumberOfPoints, false);
//CONTROLLERS:
double semiMajorAxisRatio = pow(secondElipseXradius, 3) / pow(elipseXradius, 3);
double myRatio = sqrt(360 * 360 * semiMajorAxisRatio / pow(0.05, 2));
std::unique_ptr<PlanetController> planetctr = std::make_unique<PlanetController>(planet, firstElipse, 0.05);
std::unique_ptr<PlanetController> secondPlanetctr = std::make_unique<PlanetController>(secondPlanet, secondElipse, 360 / myRatio);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static GLfloat pix2angleV;
static GLfloat pix2angleH;
int status = 0;
static int x_pos_old = 0;
static int y_pos_old = 0;
static int zoom;
static GLfloat angle[] = { 0.0, 0.0 };
static GLfloat coordinates[] = { 0.0, 1.0, 10.0 };
static GLfloat lookAtPoint[] = { 0.0, 0.0,0.0 };

static int delta_zoom;
static int delta_x;
static int delta_y;
static int thetaZoom;
static int thetaXAxis;
static int thetaYAxis;

static GLfloat xangle = 0;
static GLfloat yangle = 0;

static GLfloat azimuth;
static GLfloat altitude;

static GLfloat RADIUS = 10.0;
static GLfloat verticalSize;


void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji X kursora
							  // jako pozycji poprzedniej
		y_pos_old = y;        // przypisanie aktualnie odczytanej pozycji y kursora
							  // jako pozycji poprzedniej
		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// przypisanie aktualnie odczytanej pozycji y kursora
		// jako pozycji poprzedniej
		zoom = y;
		status = 2;					//Wcisnieto prawy klawisz
	}
	else
		status = 0;          // nie zosta� wci�ni�ty �aden klawisz
}
void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	delta_y = y - y_pos_old;
	delta_zoom = y - zoom;
	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
	y_pos_old = y;
	zoom = y;



	glutPostRedisplay();     // przerysowanie obrazu sceny
}

void movePlanets()
{
	if (Move)
	{
		planetctr->Move();
		secondPlanetctr->Move();
		glutPostRedisplay();
	}
		
}


/*************************************************************************************/
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
// przerysowa� scen�)

void lookat()
{
	std::cout << "lookAtPoint X: " << lookAtPoint[0] << " ";
	std::cout << "lookAtPoint Y: " << lookAtPoint[1] << " ";
	std::cout << "lookAtPoint Z: " << lookAtPoint[2] << std::endl;
	std::cout << "coordinates X: " << coordinates[0] << " ";
	std::cout << "coordinates Y: " << coordinates[1] << " ";
	std::cout << "coordinates Z: " << coordinates[2] << std::endl;
	std::cout << "xAngle: " << xangle * 180 / M_PI << " yAngle: " << angle[1] << std::endl;
	if (angle[1] >= 90 && angle[1] <= 270)
	{
		gluLookAt(coordinates[0], coordinates[1], coordinates[2], lookAtPoint[0] + coordinates[0], lookAtPoint[1] + coordinates[1], lookAtPoint[2] + coordinates[2], 0.0, 1.0, 0.0);
	}
	else
	{
		gluLookAt(coordinates[0], coordinates[1], coordinates[2], lookAtPoint[0] + coordinates[0], lookAtPoint[1] + coordinates[1], lookAtPoint[2] + coordinates[2], 0.0, 1.0, 0.0);
	}
	if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
	{
		//angle[0] += delta_x*pix2angleH;
		xangle += -delta_x*pix2angleH*0.01;
		lookAtPoint[0] = sin(xangle);
		lookAtPoint[2] = -cos(xangle);
		angle[0] = xangle * 180 / M_PI;

		if (angle[0] > 360)								// modyfikacja k�ta obrotu o kat proporcjonalny
		{
			angle[0] -= 360.0;
			xangle -= 2 * M_PI;
		}
		if (angle[0] < 0)
		{
			angle[0] += 360.0;
			xangle += 2 * M_PI;
		}


		yangle += -delta_y*pix2angleV*0.01;
		lookAtPoint[1] = -sin(yangle);
		angle[1] = (yangle * 180 / M_PI);
		if (angle[1] > 360)
		{
			yangle -= 2 * M_PI;
			angle[1] -= 360.0;
		}
		if (angle[1] < 0)
		{
			yangle += 2 * M_PI;
			angle[1] += 360.0;
		}



		delta_x = 0;
		delta_y = 0;
	}
	if (status == 2)
	{
		lookAtPoint[2] += (delta_zoom * pix2angleV) / 10;

		delta_zoom = 0;
	}
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym


	glLoadIdentity();
	lookat();

	glPushMatrix();
	planet->DrawMe(Model);
	glPopMatrix();

	glPushMatrix();
	secondPlanet->DrawMe(Model);
	glPopMatrix();

	glPushMatrix();
	sun->DrawMe(Model);
	glPopMatrix();



	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	if (key == '1') Model = 1;
	else if (key == '2') Model = 2;
	else if (key == '3') Model = 3;
	else if (key == '4') Model = 4;
	else if (key == '5') Model = 5;
	else if (key == '6') Model = 6;
	else if (key == 'w')
	{

		coordinates[0] += moveFactor * sin(xangle) * 10.1;
		coordinates[1] -= moveFactor * sin(yangle) * 10.1;
		coordinates[2] -= moveFactor * cos(xangle) * 10.1;

	}
	else if(key == 's')
	{

		coordinates[0] -= moveFactor * sin(xangle) * 10.1;
		coordinates[1] +=  moveFactor * sin(yangle) * 10.1;
		coordinates[2] += moveFactor * cos(xangle) * 10.1;
	}
	else if(key == 'a')
	{
		coordinates[0] -= moveFactor * 10;
		lookAtPoint[0] -= moveFactor * 10;
	}
	else if(key =='d')
	{
		coordinates[0] += moveFactor * 10;
		lookAtPoint[0] += moveFactor * 10;
	}
	else if (key == 'm')
	{
		Move = !Move;
	}

	else if (key == 'e')
	{
		Spin = !Spin;
	}

	RenderScene(); // przerysowanie obrazu sceny
}
/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
	// Zmienne dla obrazu tekstury
	GLbyte *pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny

	// Definicja materia�u z jakiego zrobiony jest przedmiot
	//-------------------------------------------------------
	GLfloat mat_ambient[] = { 0.35,0.35, 0.35f, 1.0f };
	// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego                

	GLfloat mat_shininess = { 128.0 };
	// wsp�czynnik n opisuj�cy po�ysk powierzchni

	// Definicja �r�d�a �wiat�a
	//-------------------------------------------------------
	GLfloat light_position[2][4] = {
		{ focalPointX, 0.0, 0.0, 0.0 },
		{ focalPointX, 0.0, 0.0, 0.0 }
	};
	// po�o�enie �r�d�a

	GLfloat light_ambient[] = { 0.22f, 0.22f, 0.22f, 1.0f };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[2][4] = {
		{ 1.0f, 1.0, 1.0f, 1.0 },
		{ 1.0f, 1.0, 1.0f, 1.0 }
	};
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[2][4] = {
		{ 1.0f, 1.0, 1.0f, 1.0 }
	,{ 1.0f, 1.0, 1.0f, 1.0 } };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0f };
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = { 0.01f };
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = { 0.01f };
	// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	// Ustawienie patrametr�w materia�u 
	//-------------------------------------------------------
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	// Ustawienie parametr�w �r�d�a �wiat�a
	//-------------------------------------------------------
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse[0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular[0]);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position[0]);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse[1]);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular[1]);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position[1]);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie opcji systemu o�wietlania sceny 
	//-------------------------------------------------------
	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING); // w�aczenie systemu o�wietlenia sceny 
	glEnable(GL_LIGHT0); // w��czenie �r�d�a o numerze 0
	glEnable(GL_LIGHT1); // w��czenie �r�d�a o numerze 1
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora 
	glEnable(GL_POLYGON_SMOOTH);



	

}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angleH = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie horizontal
	pix2angleV = 360.0 / (float)vertical;  // przeliczenie pikseli na stopnie vertrical
	verticalSize = (float)vertical;

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej

	gluPerspective(70, 1.0, 1.0, 100.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(void)
{

	//std::cout << focalPointX << " area : " << getRadDistance(0, 1, -1, 1);

	PlanetColor::GetColors(NumberOfPoints);
	StarColor::GetColors(NumberOfPoints);



	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk�ad planetarny");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzba przeryswania okna 
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy
	glutKeyboardFunc(keys);

	glutReshapeFunc(ChangeSize);
	glutIdleFunc(movePlanets);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna      

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT



}

/*************************************************************************************/
