#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <Eigen/Dense>

#include "poly_render.h" 
#include "parser.h"

using namespace std;
using namespace Eigen;

int main(int argc, char *argv[]) {
	vector<bPatch> *patch_list = new vector<bPatch>();
	int subdivision_size = atof(argv[2]);
	parse_file(argv[1], patch_list);
	run_glut(vector<Triangle>(), &argc, argv);
	return 0;
}