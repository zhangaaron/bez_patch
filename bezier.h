/*
This is a file that provides most of the bezier methods for getting verticies from control points to build polygons. 
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


class bPatch {
	public:
		Vector3f surface_pts[4][4];
		Vector3f control_pts[4][4];
		bPatch(Vector3f *patch_pts);
		// to_String(); //for debug
	private:

};