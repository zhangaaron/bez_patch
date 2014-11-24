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
	Vector3f deriv, p;
};

struct deriv_point_adapative {
	Vector3f deriv, p;
	float u, v;
};
/*Which sides need to be redrawn*/
enum EDGES {
  EDGE_1 = 0x01,
  EDGE_2 = 0x02,
  EDGE_3 = 0x04
};

/* A bezier patch defined by 16 control points in a 4x4 of bezier curves parametrically defined along u and v*/
class bPatch {
	public:
		vector<Vector3f> ctrl_pts;
		bPatch(vector<Vector3f> input_pts);
		// to_String(); //for debug
		struct deriv_point *subdivide_patch(float sub_size, struct deriv_point *d_array);
		struct deriv_point *adaptive_subdivide(float sub_size, float eps, struct deriv_point *d_array);
		void bPatch::subdivide_triangle(Triangle tri, vector<Triangle> *finished);
		void bPatch::subdivide_triangle_helper(Triangle tri, vector<Triangle> *unfinished, unsigned char FLAG);
		void bPatch::convert_midpoint(adaptive_deriv_point *mid);
		struct deriv_point *p_interp(float u, float v, struct deriv_point *p_n);
		void print_bP();
	private:

};
/*
	A bezier curve, defined by four control points and can be evaluated parametrically by u. 
*/
class bCurve {
	public:
		vector<Vector3f> ctrl_pts;
		bCurve(Vector3f a, Vector3f b, Vector3f c, Vector3f d);
		// bCurve(Vector3f *arr);
		/* Implementation of deCastejau's algorithm up in this motherfucker*/
		struct deriv_point *c_interp(float u, struct deriv_point *d_p);
	private:
};

void printV(Vector3f v);
void printDeriv(struct deriv_point *d); 