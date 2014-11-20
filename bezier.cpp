#include "bezier.h"

using namespace std;
using namespace Eigen;


bPatch::bPatch(Vector3f *patch_pts) {
	for (int i = 0; i < 16; i++){
		surface_pts[ i / 4][ i % 4] = patch_pts[i];
	}
}