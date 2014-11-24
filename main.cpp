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

void do_adaptive(char **argv) {
	float epsilon = atof(argv[2]);
	vector<bPatch> *patch_list = new vector<bPatch>();
	parse_file(argv[1], patch_list);
	printf("epsilon size: %f, number of patches: %d\n", epsilon, patch_list->size());
	vector<Triangle> *triangle_list = new vector<Triangle>();
	triangle_list->reserve(3200);
	for (int i = 0; i < patch_list->size(); i ++){
		vector<Triangle> *patch_triangles = patch_list->at(i).adaptive_subdivide(epsilon);
		triangle_list->insert(triangle_list->end(), patch_triangles->begin(), patch_triangles->end());
		patch_triangles->~vector();
	}
	int argc = 0; //hack
	run_glut_triangles(triangle_list, &argc, argv);
}


void do_uniform(char **argv) {
	float subdivision_size = atof(argv[2]);
	float num_steps = floor (1/ subdivision_size);
	int num = (int) num_steps; //for array indexing
	int numsq = num * num;
	subdivision_size = 1 / num_steps; //so we get subdivision size.
	vector<bPatch> *patch_list = new vector<bPatch>();
	parse_file(argv[1], patch_list);

	printf("subdivision size: %f, number of patches: %d\n", subdivision_size, patch_list->size());
	printf("num: %d\n", num);
		int patch_count = patch_list->size();
		vector<Quad> *quad_list = new vector<Quad>();
		for(int i = 0; i < patch_count; i++) {
			struct deriv_point patch_pts[(num + 1) * (num + 1)]; //fill up a num + 1 ^2 array of pts. from a single patch
			patch_list->at(i).subdivide_patch(subdivision_size, patch_pts);
			for (int j = 0; j < numsq ; j++) {
				int base = (j / num) * (num + 1) + j % num;
				quad_list->push_back(Quad(patch_pts + base + num + 1, patch_pts + base,
											patch_pts + base + 1, patch_pts + base + num + 2));
				//printf("Patch no. %d subdivision: %d    ", i, j);
				// printDeriv(patch_pts + base + num + 1);
				// printDeriv(patch_pts + base);
				// printDeriv(patch_pts + base + 1);
				// printDeriv(patch_pts + base + num + 2);
				//printf("\n");


			}
		}
	//delete[] patch_list;
	int argc = 0; //hack
	run_glut(quad_list, quad_list->size(), MODE, &argc, argv);

}
int main(int argc, char *argv[]) {


	if (argc == 4)  { //Use adaptive.

		do_adaptive(argv);
	}
	else {
		do_uniform(argv);
	}
	return 0;
}