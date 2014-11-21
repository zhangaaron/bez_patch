/*
	Acknowledgements: Robert Bateman for 3d camera projection code
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "poly_render.h"

using namespace std;

float zoom = 15.0f;
float rotx = 0;
float roty = 0.001f;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};

//-------------------------------------------------------------------------------
/// \brief	Initialises the openGL scene
/// 
void Init() 
{
	glEnable(GL_DEPTH_TEST);
}
void displayTest()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);	

	// draw grid
	glBegin(GL_LINES);
	for(int i=-10;i<=10;++i) {
		glVertex3f(i,0,-10);
		glVertex3f(i,0,10);

		glVertex3f(10,0,i);
		glVertex3f(-10,0,i);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
		glVertex3f(0, 1, 1);
		glVertex3f(0, 1, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
		glVertex3f(0, 1, 1);
		glVertex3f(0, 1, 0);
	glEnd();
	glutWireTeapot(1.0);

	glutSwapBuffers();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);	

	// draw grid
	glBegin(GL_LINES);
	for(int i=-10;i<=10;++i) {
		glVertex3f(i,0,-10);
		glVertex3f(i,0,10);

		glVertex3f(10,0,i);
		glVertex3f(-10,0,i);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
		glVertex3f(0, 1, 1);
		glVertex3f(0, 1, 0);
	glEnd();
	glutWireTeapot(1.0);

	glutSwapBuffers();
}

//-------------------------------------------------------------------------------
/// \brief	Called when the screen gets resized
/// \param	w	-	the new width
/// \param	h	-	the new height
/// 
void reshape(int w, int h)
{
	// prevent divide by 0 error when minimised
	if(w==0) 
		h = 1;

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//-------------------------------------------------------------------------------
//
void Motion(int x,int y)
{
	int diffx = x-lastx;
	int diffy = y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[0] && Buttons[1] )
	{
		zoom -= (float) 0.05f * diffx;
	}
	else
		if( Buttons[0] )
		{
			rotx += (float) 0.5f * diffy;
			roty += (float) 0.5f * diffx;		
		}
		else
			if( Buttons[1] )
			{
				tx += (float) 0.05f * diffx;
				ty -= (float) 0.05f * diffy;
			}
			glutPostRedisplay();
}

//-------------------------------------------------------------------------------
//
void Mouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;		
	}
	glutPostRedisplay();
}
void run_glut(vector<Triangle> poly_list, int *argcp, char **argv){
	glutInit(argcp,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Maya Camera");


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	Init();

	glutMainLoop();


}




