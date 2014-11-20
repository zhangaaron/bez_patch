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

class Triangle {
	public:
		int vert1[3];
		int vert2[3];
		int vert3[3];
	private:

};
void display();
void run_glut(vector<Triangle> poly_list, int *argcp, char **argv);