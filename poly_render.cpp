/*
	Acknowledgements: Robert Bateman for Motion function and Mouse function. 

	References: GLProgramming.com, opengl.org forums for lighting, shading, keyboard guide.
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "poly_render.h"

using namespace std;
using namespace Eigen;

float zoom = 15.0f;
float rotx = 0;
float roty = 0.001f;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};
vector<Quad> *q_list;
vector<Triangle> *t_list;
int SMOOTH = 1;
int WIREFRAME = 0;
//-------------------------------------------------------------------------------
/// \brief	Initialises the openGL scene
/// 
void init() 
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = {0, 3, 0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

}

void display()
{
	if (SMOOTH) glShadeModel (GL_SMOOTH);
	else glShadeModel(GL_FLAT);
	if (WIREFRAME) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);
	glEnable(GL_NORMALIZE);


	glBegin(GL_QUADS); //q_list->size()
		for (int i = 0; i < q_list->size(); i++) {
			Quad quad_to_draw = q_list->at(i);
			glNormal3f(quad_to_draw.a.deriv[0], quad_to_draw.a.deriv[1], quad_to_draw.a.deriv[2]);
			glVertex3f(quad_to_draw.a.p[0], quad_to_draw.a.p[1], quad_to_draw.a.p[2]);

			glNormal3f(quad_to_draw.b.deriv[0], quad_to_draw.b.deriv[1], quad_to_draw.b.deriv[2]);
			glVertex3f(quad_to_draw.b.p[0], quad_to_draw.b.p[1], quad_to_draw.b.p[2]);

			glNormal3f(quad_to_draw.c.deriv[0], quad_to_draw.c.deriv[1], quad_to_draw.c.deriv[2]);
			glVertex3f(quad_to_draw.c.p[0], quad_to_draw.c.p[1], quad_to_draw.c.p[2]);

			glNormal3f(quad_to_draw.d.deriv[0], quad_to_draw.d.deriv[1], quad_to_draw.d.deriv[2]);
			glVertex3f(quad_to_draw.d.p[0], quad_to_draw.d.p[1], quad_to_draw.d.p[2]);
		}
	glEnd();


	glutSwapBuffers();
}

void display_triangles()
{
	if (SMOOTH) glShadeModel (GL_SMOOTH);
	else glShadeModel(GL_FLAT);
	if (WIREFRAME) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);
	glEnable(GL_NORMALIZE);


	glBegin(GL_TRIANGLES); //q_list->size()
		for (int i = 0; i < t_list->size(); i++) {
			Triangle triangle_to_draw = t_list->at(i);
			glNormal3f(triangle_to_draw.a.deriv[0], triangle_to_draw.a.deriv[1], triangle_to_draw.a.deriv[2]);
			glVertex3f(triangle_to_draw.a.p[0], triangle_to_draw.a.p[1], triangle_to_draw.a.p[2]);

			glNormal3f(triangle_to_draw.b.deriv[0], triangle_to_draw.b.deriv[1], triangle_to_draw.b.deriv[2]);
			glVertex3f(triangle_to_draw.b.p[0], triangle_to_draw.b.p[1], triangle_to_draw.b.p[2]);

			glNormal3f(triangle_to_draw.c.deriv[0], triangle_to_draw.c.deriv[1], triangle_to_draw.c.deriv[2]);
			glVertex3f(triangle_to_draw.c.p[0], triangle_to_draw.c.p[1], triangle_to_draw.c.p[2]);

		}
	glEnd();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

void MyKeyboardFunc(unsigned char Key, int x, int y){
	switch(Key){
		case 'w':
			WIREFRAME ? WIREFRAME = 0: WIREFRAME =1;
		case 's':			
			SMOOTH ? SMOOTH = 0: SMOOTH = 1;

	}
	glutPostRedisplay();
}

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
void run_glut(vector<Quad> *quad_list, int list_size, int MODE, int *argcp, char **argv){
	glutInit(argcp,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("basicBezier v4.20");

	q_list = quad_list;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(MyKeyboardFunc);
	glutMotionFunc(Motion);

	init();

	glutMainLoop();


}

void run_glut_triangles(vector<Triangle> *triangle_list, int *argcp, char **argv){
	glutInit(argcp,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("basicBezier v4.20");

	t_list = triangle_list;
	glutDisplayFunc(display_triangles);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(MyKeyboardFunc);
	glutMotionFunc(Motion);

	init();

	glutMainLoop();


}



Quad::Quad(struct deriv_point *a, struct deriv_point *b, struct deriv_point *c, struct deriv_point *d) {
	this->a = *a;
	this->b = *b;
	this->c = *c;
	this->d = *d;
}