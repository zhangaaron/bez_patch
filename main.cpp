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

int main(int argc, char *argv[]) {
	vector<bPatch> *patch_list = new vector<bPatch>();
	int subdivision_size = atof(argv[2]);
	// for(int i = 0; i < patch_list.size(); i++) {

	// }
	parse_file(argv[1], patch_list);
	run_glut(vector<Triangle>(), &argc, argv);
	return 0;
}