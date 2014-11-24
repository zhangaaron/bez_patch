/*
	Given set of polygons, we generate a scene. 
*/
#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <Eigen/Dense>
#include <GL/glut.h>
#include <GL/glu.h>
#include "bezier.h"


using namespace std;
using namespace Eigen;

class Shape {
	public:
		int numVerticies;
		//virtual Vector3f *getVerts();
	private:
};


/* PATENT PENDING POLYGON
		  b-> c 
		  ^   |
		  |  `/
          a   d
*/  
class Quad: public Shape {
	public:
		int numVerticies;
		struct deriv_point a;
		struct deriv_point b;
		struct deriv_point c;
		struct deriv_point d;
		Quad(struct deriv_point *a, struct deriv_point *b, struct deriv_point *c, struct deriv_point *d);
	private:

};
void display();
void run_glut(vector<Quad> *quad_list, int list_size, int MODE, int *argcp, char **argv);