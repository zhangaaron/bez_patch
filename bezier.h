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
#include <assert.h> 

using namespace std;
using namespace Eigen;

struct deriv_point {
	Vector3f deriv;
	Vector3f p;
};
/* A bezier patch defined by 16 control points in a 4x4 of bezier curves parametrically defined along u and v*/
class bPatch {
	public:
		Vector3f ctrl_pts[4][4];
		bPatch(Vector3f *patch_pts);
		// to_String(); //for debug
		Vector3f *subdivide_patch(float step_size, Vector3f v_arr);
		struct deriv_point *p_interp(float u, float v, deriv_point *p_n);
	private:

};
/*
	A bezier curve, defined by four control points and can be evaluated parametrically by u. 
*/
class bCurve {
	public:
		Vector3f ctrl_pts[4];
		bCurve(Vector3f a, Vector3f b, Vector3f c, Vector3f d);
		bCurve(Vector3f *arr);
		/* Implementation of deCastejau's algorithm up in this motherfucker*/
		struct deriv_point *c_interp(float u, deriv_point *d_p);
	private:
};

