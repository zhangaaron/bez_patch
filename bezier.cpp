#include "bezier.h"

using namespace std;
using namespace Eigen;


bPatch::bPatch(Vector3f *patch_pts) {
	for (int i = 0; i < 16; i++){
		ctrl_pts[ i / 4][ i % 4] = patch_pts[i];
	}
}
//FOR NOW WITHOUT NORMALS STORED. 
struct deriv_point *bPatch::subdivide_patch(float sub_size, struct deriv_point *d_array) {
	int num_divisions = (int)floor(1 / sub_size);
	// for each parametric value of u
	for (int i = 0; i < num_divisions + 1; i++) {
		float u = i * sub_size;
		//for each parametric value of v:
		for (int j = 0; j < num_divisions + 1; j++) {
			float v = j * sub_size;

			//evaluate surface value and normal
			struct deriv_point p_n;
			d_array[i + j * (num_divisions + 1)] = *p_interp(u, v, &p_n); 
		}
	}

}

//FOR NOW WITHOUT NORMALS STORED. 
struct deriv_point *bPatch::subdivide_patch(float sub_size, struct deriv_point *d_array) {
	int num_divisions = (int)floor(1 / sub_size);
	// for each parametric value of u
	for (int i = 0; i < num_divisions + 1; i++) {
		float u = i * sub_size;
		//for each parametric value of v:
		for (int j = 0; j < num_divisions + 1; j++) {
			float v = j * sub_size;

			//evaluate surface value and normal
			struct deriv_point p_n;
			d_array[i + j * (num_divisions + 1)] = *p_interp(u, v, &p_n); 
		}
	}

}

struct deriv_point *bPatch::p_interp(float u, float v, deriv_point *p_n) {
	assert (u >= 0.0 && u <= 1.0);
	assert (v >= 0.0 && v <= 1.0);
	//Build control points for a bez_curve along V. HARDCODED FOR NOW. MAYBE LOOP IS BETTER
	Vector3f v_points[4];
	struct deriv_point d_p;
	v_points[0] = (bCurve(ctrl_pts[0]).c_interp(u, &d_p))->p;
	v_points[1] = (bCurve(ctrl_pts[1]).c_interp(u, &d_p))->p;
	v_points[2] = (bCurve(ctrl_pts[2]).c_interp(u, &d_p))->p;
	v_points[3] = (bCurve(ctrl_pts[3]).c_interp(u, &d_p))->p;

	//Build control points for a bez_curve along U. BETTER WAY TO DO THIS? 
	Vector3f u_points[4];
	u_points[0] = (bCurve(ctrl_pts[0][0], ctrl_pts[1][0],
							ctrl_pts[2][0], ctrl_pts[3][0]).c_interp(u, &d_p))->p;
	u_points[1] = (bCurve(ctrl_pts[0][1], ctrl_pts[1][1],
							ctrl_pts[2][1], ctrl_pts[3][1]).c_interp(u, &d_p))->p;
	u_points[2] = (bCurve(ctrl_pts[0][2], ctrl_pts[1][2],
							ctrl_pts[2][2], ctrl_pts[3][2]).c_interp(u, &d_p))->p;
	u_points[3] = (bCurve(ctrl_pts[0][3], ctrl_pts[1][3],
							ctrl_pts[2][3], ctrl_pts[3][3]).c_interp(u, &d_p))->p;

	//now evaluate each curve for point and derivative. 
	struct deriv_point d_p_u;
	struct deriv_point d_p_v;
	bCurve(v_points).c_interp(v, &d_p_v);
	bCurve(u_points).c_interp(u, &d_p_u);
	//They should match within some reasonable value. 
	assert (abs(d_p_v.p[0] - d_p_u.p[0]) < 0.001);
	assert (abs(d_p_v.p[1] - d_p_u.p[1]) < 0.001);
	assert (abs(d_p_v.p[2] - d_p_u.p[2]) < 0.001);
	//take cross of partials to get normal 
	Vector3f normal = d_p_v.deriv.cross(d_p_v.deriv);
	normal = normal.normalized(); 
	p_n->p = d_p_u.p;
	p_n->deriv = normal;
	return p_n;
}

bCurve::bCurve(Vector3f a, Vector3f b, Vector3f c, Vector3f d) {
	ctrl_pts[0] = a;
	ctrl_pts[1] = b;
	ctrl_pts[2] = c;
	ctrl_pts[3] = d;
}

bCurve::bCurve(Vector3f  *arr) {
	bCurve(arr[0], arr[1], arr[2], arr[3]);
}

struct deriv_point *bCurve::c_interp(float u, deriv_point *d_p) {
	assert (u >= 0.0 && u <= 1.0);
	//forming line segments AB and BC from control points
	Vector3f A = ctrl_pts[0] * (1.0 - u) + ctrl_pts[1] *  u;
	Vector3f B = ctrl_pts[1] * (1.0 - u) + ctrl_pts[2] * u;
	Vector3f C = ctrl_pts[2] * (1.0 - u) + ctrl_pts[3] * u;

	//form line segment DE from AB and BC:
	Vector3f D = A * (1.0 - u) + B * u;
	Vector3f E = B * (1.0 - u) + C * u;

	//our point to return lies on DE
	Vector3f point_to_return = D * (1.0 -u) + E * u;

	//??? Not exactly sure about this but gives us derivative w/ respect to u.
	Vector3f dPdu = 3 * (E -D);

	d_p->deriv = dPdu;
	d_p->p = point_to_return;
	return d_p;
}