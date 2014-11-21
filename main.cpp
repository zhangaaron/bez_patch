#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <Eigen/Dense>

#include "poly_render.h" 
#include "parser.h"
#include "bezier.h"
// #include "geometry.h"

using namespace std;
using namespace Eigen;
int MODE;


int main(int argc, char *argv[]) {
	if (argc == 4) MODE = 1; //Use adaptive.
	float subdivision_size = atof(argv[2]);
	float num_steps = floor (1/ subdivision_size);
	int num = (int) num_steps; //for array indexing
	int numsq = num * num;
	subdivision_size = 1 / num_steps; //so we get subdivision size.

	vector<bPatch> *patch_list = new vector<bPatch>();
	parse_file(argv[1], patch_list);
	if (MODE) { //Do adapative tesselation
		// for(int i = 0; i < patch_list->size(); i++) {
		// 	patch_list->at(i).subdivide_patch(subdivision_size, v_array + num * num * i);
		// }
	}
	else { //WILL BREAK FOR TOO SMALL NUM = 1
		int patch_count = patch_list->size();
		Quad *quad_list = (Quad *)malloc(sizeof(Quad) *  patch_count * numsq); 
		for(int i = 0; i < patch_count; i++) {
			struct deriv_point patch_pts[numsq];
			patch_list->at(i).subdivide_patch(subdivision_size, patch_pts);
			for (int j = 0; j < numsq ; j++) {
				Quad *quad_base_ptr = quad_list + i * numsq;
				quad_base_ptr[j].verts[0]  = patch_pts[j].p;
				quad_base_ptr[j].verts[1]  = patch_pts[j + 1].p;
				quad_base_ptr[j].verts[2]  = patch_pts[num + j].p;
				quad_base_ptr[j].verts[3]  = patch_pts[num + j + 1].p;
			}
		}
		delete[] patch_list; //free memory occupied by patch_list
		run_glut(quad_list, patch_count * numsq, MODE, &argc, argv);
	}

	// run_glut(, &argc, argv);
	return 0;
}