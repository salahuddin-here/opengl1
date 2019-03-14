/*********
CTIS164 - Template Source Program
----------
STUDENT : Mohammad Salahuddin Kurd 
SECTION : Section 164 - 003 
HOMEWORK:	Home work 2 ( Shooting GAME)
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
*********/

#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  30// Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {						//cannon structure 
	int x, y, rL, rs;
}firingObject;
firingObject fObj{-400, -280, 17, 7};

typedef struct {						//plane structure
	int x, y;
}movingObject;

movingObject mObj[5];


typedef struct {			//bullet structure
	int x, y;
}bullet_t;
bullet_t bul{fObj.x + 30, fObj.y + 20 };



int objY[5] = { -370,-530, -690, -850, -1010 }, Sec1 = 0, Sec2 = 0, countObj = 0, hit = 0, point = 0, TotalPoints = 0;
bool move = false,END = false;

bool activator = false, hitPoint = false;

						  //
						  // to draw circle, center at (x,y)
						  // radius r
						  //
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}


void displayBackground() {

	glBegin(GL_QUADS);
	//dark blue colour
	glColor3ub(102, 178, 255);
	glVertex2f(-400, 300);
	glVertex2f(400, 300);
	//light blue color
	glColor3ub(204, 229, 255);
	glVertex2f(400, -300);
	glVertex2f(-400, -300);
	glEnd();


	glColor3f(0.0, 0.0, 0.0);

	glRectf(-400, -300, 400, -280);
	glColor3f(1, 1, 1);
	vprint(-170, -295, GLUT_BITMAP_8_BY_13, "<SPACEBAR> Fire, <F1> Pause/Restart");
}
void displayScoreboard() {
	glColor3f(0, 0, 0);	// score board black color
	glRectf(400, 300, 300, -300);

	

	glColor3f(1, 1, 1); // color of the headings written in the score board 
	vprint(315, 255, GLUT_BITMAP_HELVETICA_12, "REMAINING");
	vprint(332, 240, GLUT_BITMAP_HELVETICA_12, "TIME");

	vprint(324, 220, GLUT_BITMAP_HELVETICA_18, "%02d:%02d", Sec1, Sec2);


	vprint(330, 160, GLUT_BITMAP_HELVETICA_12, "TOTAL");
	vprint(320, 145, GLUT_BITMAP_HELVETICA_12, "PLANES");
	vprint(340, 125, GLUT_BITMAP_HELVETICA_18, "%d", countObj);

	vprint(330, 70, GLUT_BITMAP_HELVETICA_12, "PLANES");
	vprint(320, 55, GLUT_BITMAP_HELVETICA_12, "EXPLODED");
	vprint(348, 35, GLUT_BITMAP_HELVETICA_18, "%d", hit);

	vprint(335, -20, GLUT_BITMAP_HELVETICA_12, "LAST");
	vprint(329, -35, GLUT_BITMAP_HELVETICA_12, "POINTS");
	vprint(342, -55, GLUT_BITMAP_HELVETICA_18, "%d", point);

	vprint(335, -125, GLUT_BITMAP_HELVETICA_12, "TOTAL");
	vprint(331, -140, GLUT_BITMAP_HELVETICA_12, "POINTS");
	vprint(347, -165, GLUT_BITMAP_HELVETICA_18, "%d", TotalPoints);

}
void displayFiringObject() {


	glColor3ub(255, 153, 53); //brown color gun
	glBegin(GL_TRIANGLES);
	glVertex2f(fObj.x, fObj.y);
	glVertex2f(fObj.x, fObj.y + 50);
	glVertex2f(fObj.x + 30, fObj.y + 25);
	glEnd();

	circle(fObj.x + 20, fObj.y + 25, fObj.rL);


	glBegin(GL_QUADS);
	glVertex2f(fObj.x + 20, fObj.y + 42);
	glVertex2f(fObj.x + 20, fObj.y + 8);
	glVertex2f(fObj.x + 70, fObj.y + 16);
	glVertex2f(fObj.x + 70, fObj.y + 33);
	glEnd();
	
	//dark brown color circle 
	glColor3ub(204, 102, 0);
	circle(fObj.x + 20, fObj.y + 25, fObj.rs);

}

void displayMovingObjects(int x, int y) {
	
	glColor3ub(51, 51, 255);
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 20, y - 25);
	glVertex2f(x + 50, y - 25);
	glVertex2f(x + 15, y - 5);
	glEnd();

	glColor3ub(0.3, 0.3, 0.3);
	glRectf(x, y - 25 , x + 30, y + 35);
	
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y + 35);
	glVertex2f(x + 30, y + 35);
	glVertex2f(x +15 , y + 55);
	glEnd();
	
	glBegin(GL_QUADS);
	glVertex2f(x , y + 3);
	glVertex2f(x, y + 15);
	glColor3ub(255, 0, 0);
	glVertex2f(x - 30, y + 10);
	glVertex2f(x - 30, y );
	glEnd();

	glColor3ub(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(x + 30, y + 3);
	glVertex2f(x + 30, y + 15);
	glColor3ub(255, 0, 0);
	glVertex2f(x + 60, y + 10);
	glVertex2f(x + 60, y);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(x + 16, y + 30);
	glVertex2f(x + 16, y - 25);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(x , y + 35);
	glVertex2f(x + 16, y + 30);
	glVertex2f(x +30, y + 35);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(x + 16, y + 30);
	glVertex2f(x + 30, y + 35);
	glEnd();

}

void bullet() {
	

	glColor3f(0, 0, 0);
	glRectf(bul.x, bul.y, bul.x + 20, bul.y + 10);

	glColor3f(1, 0, 0);

	glBegin(GL_TRIANGLES);
	glVertex2f(bul.x + 20, bul.y + 10);
	glVertex2f(bul.x + 20, bul.y);
	glVertex2f(bul.x + 30, bul.y + 5);
	glEnd();

}

void bullMove() {
		bul.x += 10;
}
void bulletBackAgain() {
	if (bul.x >= 300)
	{
		move = false;
		bul = { fObj.x + 30, fObj.y + 20 };
	}
}

void hitCount() {
	for (int i = 0; i < 5; i++)
	{
		if ((bul.x + 30 >= mObj[i].x && bul.x + 30 <= mObj[i].x + 30) && (bul.y > mObj[i].y - 25 && bul.y < mObj[i].y + 55))
		{

			if (bul.y <= mObj[i].y + 20 && bul.y >= mObj[i].y + 10)
				point = 5;
			else if ((bul.y <= mObj[i].y + 30 && bul.y > mObj[i].y + 20 ) || (bul.y >= mObj[i].y && bul.y < mObj[i].y + 10))
				point = 4;
			else if ((bul.y <= mObj[i].y + 40 && bul.y > mObj[i].y + 30) || (bul.y >= mObj[i].y - 10 && bul.y < mObj[i].y))
				point = 3;
			else if ((bul.y <= mObj[i].y + 50 && bul.y > mObj[i].y + 40) || (bul.y >= mObj[i].y - 20 && bul.y < mObj[i].y - 10))
				point = 2;
			else if ((bul.y <= mObj[i].y + 55 && bul.y > mObj[i].y + 50) || (bul.y >= mObj[i].y - 25 && bul.y < mObj[i].y - 20))
				point = 1;
			
			TotalPoints += point;

			hit++;
			mObj[i].y = objY[i];
			mObj[i].x = rand() % 250 + (-150);

		}
	}
}
void initialize() {
	
	for (int i = 0; i < 5; i++)
	{
		mObj[i].y = objY[i];
		mObj[i].x = rand() % 250 + (-150);
	}
	hit = 0;
	countObj = 0;
	point = 0;
	TotalPoints = 0;
	fObj = { -400, -280, 17, 7 };
	bul = { fObj.x + 30, fObj.y + 20 };

}
	

//
// To display onto window using OpenGL commands
//
void display() {

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	
	displayBackground();
	bullet();
	displayFiringObject();
	bulletBackAgain();
	displayScoreboard();



	displayMovingObjects(mObj[0].x, mObj[0].y);
	displayMovingObjects(mObj[1].x, mObj[1].y);
	displayMovingObjects(mObj[2].x, mObj[2].y);
	displayMovingObjects(mObj[3].x, mObj[3].y);
	displayMovingObjects(mObj[4].x, mObj[4].y);
	

	
	
	glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == 32 && activator == true)
		move = true;
	
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	if (key == GLUT_KEY_UP && fObj.y + 53 <= WINDOW_HEIGHT / 2 && activator == true)
	{
		fObj.y += 5;
		if (move == false)
			bul.y += 5;
	}
	if (key == GLUT_KEY_DOWN && fObj.y >= -WINDOW_HEIGHT / 2 + 23 && activator == true)
	{
		fObj.y -= 5;
		if (move == false)
			bul.y -= 5;
	}
	if (key == GLUT_KEY_F1)
	{
		
		activator = !activator;
		
		if (Sec2 == 0)
		{
			Sec2 = 99;
			Sec1 = 19;
		}
		if (END == true)
		{
			initialize();
			Sec2 = 99;
			Sec1 = 19;
			END = false;
		}
		
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (activator ==  true)
	{
		Sec2--;
		if (Sec2 % 100 == 0)
		{
			Sec1--;
			if (Sec1 != 0)
				Sec2 = 99;

		}

		if (move == true)
			bullMove();
		

		for (int i = 0; i < 5; i++)
		{
			mObj[i].y += 10;

			if (mObj[i].y == -280)
			{
				countObj++;
			}
			hitCount();

		}
		if (mObj[0].y >= 300)
		{
			mObj[0].y = objY[0];
			mObj[0].x = rand() % 50 + (-200);
		}
		if (mObj[1].y >= 300)
		{
			mObj[1].y = objY[1];
			mObj[1].x = rand() % 50 + (-50);
		}
		if (mObj[2].y >= 300)
		{
			mObj[2].y = objY[2];
			mObj[2].x = rand() % 50 + 50;
		}
		if (mObj[3].y >= 300)
		{
			mObj[3].y = objY[0];
			mObj[3].x = rand() % 50 + 150;
		}
		if (mObj[4].y >= 300)
		{
			mObj[4].y = objY[0];
			mObj[4].x = rand() % 50 + 200;
		}
			
			if (Sec1 <= 0)
			{
				END = true;
				activator = false;
				move = false;
			}
		}
		

	
	
	

	


	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Hit It By Salahuddin");
	initialize();

	srand(time(NULL));

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif



	Init();

	glutMainLoop();
}