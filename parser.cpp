/*
	Reads from patch data to return a list of points.  
*/

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "parser.h"

using namespace std;
using namespace Eigen;

void parse_file(char * file_name, vector<bPatch> *patch_list){
	FILE *fp = fopen(file_name, "r"); //File pointer to the file we want to read. 
	if (!fp) {//file could not be read, exit. 
		printf("%s could not be read, exiting. ", file_name);
		exit(0);
	}

	char line[200];
	fgets(line, 200, fp);
	vector<Vector3f> patch_pts_temp; //keeps track of a list of 16 Vector3f patch points that we supply to patch constructor
	vector<float> float_list;
	while (fgets(line, 200, fp) != NULL){ //Get next line with limit of 200 character lines until we hit the EOF. 
		//printf("line is %s,%d", line, strlen(line));
		if (strlen(line) > 2) {
		char *value = strtok(line, " \r\n");
			while (value){
				float_list.push_back(atof(value));
				value = strtok(NULL, " \r\n");
			}
		}
	}
	for (int i = 0; i < float_list.size() / 48; i ++ ) {
		vector<Vector3f> patch_pts;
		for (int j = 0; j < 16; j++) { //form (x,y,z) pts from float_list
			int start_i = i * 48 + j * 3;
			patch_pts.push_back(Vector3f(float_list.at(start_i),float_list.at(start_i + 1), float_list.at(start_i + 2)));
		}
		patch_list->push_back(bPatch(patch_pts));
		patch_pts.clear();
	}
}