/*
	Reads from patch data to return a list of points.  
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


void parse_file(char * file_name, vector<bPatch> *patch_list);


