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

using namespace std;
using namespace Eigen;

class Shape {
	public:
		int numVerticies;
		//virtual Vector3f *getVerts();
	private:
};
class Triangle: public Shape {
	public:
		int numVerticies;
		Vector3f verts[3];
	private:

};
class Quad: public Shape {
	public:
		int numVerticies;
		Vector3f verts[4];
	private:

};
void display();
void run_glut(Shape *shape_list, int list_size, int MODE, int *argcp, char **argv);