#include "bezier.h"

using namespace std;
using namespace Eigen;

IOFormat CommaInitFmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", " << ", ";");


Triangle::Triangle(struct deriv_point_adaptive a, struct deriv_point_adaptive b, struct deriv_point_adaptive c) {
	this->a = a;
	this->b = b;
	this->c = c;
}
/*Not sure if the linearly interpolated deriv means anything but it is never used*/
void Triangle::get_midpoint(int edge, struct deriv_point_adaptive *fill) {
	switch (edge) {
		case 1:
			fill->u = (a.u + c.u) / 2;
			fill->v = (a.v + c.v) / 2;
			fill->deriv = (a.deriv + c.deriv) / 2;
			fill->p = (a.p + c.p) / 2;
			break;
		case 2:
			fill->u = (a.u + b.u) / 2;
			fill->v = (a.v + b.v) / 2;
			fill->deriv = (a.deriv + b.deriv) / 2;
			fill->p = (a.p + b.p) / 2;
			break;
		case 3:
			fill->u = (c.u + b.u) / 2;
			fill->v = (c.v + b.v) / 2;
			fill->deriv = (c.deriv + b.deriv) / 2;
			fill->p = (c.p + b.p) /2;
			break;
		default: 
			printf("invalid edge asked for in get_midpoint: %d\n", edge);
			exit(0);
	}
}

bPatch::bPatch(vector<Vector3f> input_pts) {
	assert (input_pts.size() == 16);
	this->ctrl_pts = input_pts;
}


void printV(Vector3f v){
	cout << "\n" << v << "\n";
}

void bPatch::print_bP() {
	for (int v = 0; v < 4; v++) {
		for (int u = 0; u < 4; u++) {
			cout << ctrl_pts.at(u + v * 4).format(CommaInitFmt) << " ";
		}
		printf ("\n");
	}
}

void printDeriv(struct deriv_point *d) {
	cout << d->deriv.format(CommaInitFmt) << " ";
}

struct deriv_point *bPatch::subdivide_patch(float sub_size, struct deriv_point *d_array) {
	int num_divisions = (int)floor(1 / sub_size);
	// for each parametric value of u:
	for (int i = 0; i < num_divisions + 1; i++) {
		float u = i * sub_size;
		//for each parametric value of v:
		for (int j = 0; j < num_divisions + 1; j++) {
			float v = j * sub_size;
			//evaluate surface value and normal
			struct deriv_point p_n;
			// printf("u, v: %f %f. \n", u, v);
			d_array[i + j * (num_divisions + 1)] = *p_interp(u, v, &p_n);
		}
	}
	return d_array;
}

//Don't know how large this is so use a vector
vector<Triangle> *bPatch::adaptive_subdivide(float sub_size, float eps, struct deriv_point *d_array) {
	struct deriv_point m[4];
	subdivide_patch(1.0, m);
	struct deriv_point_adaptive corners[4]; 
	for (int i = 0; i < 4; i++) {
		corners[i].p = m[i].p;
		corners[i].deriv = m[i].deriv;
		corners[i].u = i % 2;
		corners[i].v = i / 2;
	}

	//each deriv_point needs to store own u,v value for checking? I think so
	vector<Triangle> *finished = new vector<Triangle>();
	vector<Triangle> *unfinished = new vector<Triangle>();
	/*Make initial split of 2x2 deriv_pt into triangles*/
	Triangle x = Triangle(corners[2], corners[3], corners[0]);
	Triangle y = Triangle(corners[3], corners[1], corners[0]);
	unfinished->push_back(x);
	unfinished->push_back(y);

	for (int i = 0; i < unfinished->size(); i++) {
		subdivide_triangle(unfinished->at(i), eps, finished);
	}
	unfinished->~vector();
	return finished;
}

void bPatch::subdivide_triangle(Triangle tri, float epsilon, vector<Triangle> *finished){
	unsigned char FLAG = 0;
	struct deriv_point_adaptive temp;
	struct deriv_point bez_temp;

	//Determines which edges need to be split
	tri.get_midpoint(1, &temp);
	if ((temp.p - p_interp(temp.u, temp.v, &bez_temp)->p).norm() > epsilon) FLAG | EDGE1;

	tri.get_midpoint(2, &temp);
	if ((temp.p - p_interp(temp.u, temp.v, &bez_temp)->p).norm() > epsilon) FLAG | EDGE2;

	tri.get_midpoint(3, &temp);
	if ((temp.p - p_interp(temp.u, temp.v, &bez_temp)->p).norm() > epsilon) FLAG | EDGE3;

	//base case: triangle doesn't need to be split anymore, push onto finished list and return.
	if (!FLAG) {
		finished->push_back(tri);
		return;
	}
	/*Otherwise recursively divide triangles with sub*/
	vector<Triangle> *unfinished = new vector<Triangle>();
	subdivide_triangle_helper(tri, unfinished, FLAG);
	for (int i = 0; i < unfinished->size(); i++) {
		subdivide_triangle(unfinished->at(i), epsilon, finished);
	}
	unfinished->~vector();
}

void bPatch::subdivide_triangle_helper(Triangle tri, vector<Triangle> *unfinished, unsigned char FLAG){
	
switch (FLAG) {
		 
		case EDGE1 | EDGE2 | EDGE3: {
			struct deriv_point_adaptive E1_mid, E2_mid, E3_mid;
			
			tri.get_midpoint(1, &E1_mid);
			tri.get_midpoint(2, &E2_mid);
			tri.get_midpoint(3, &E3_mid);

			convert_midpoint(&E1_mid);
			convert_midpoint(&E2_mid);
			convert_midpoint(&E3_mid);

			Triangle w = Triangle(tri.a, E2_mid, E1_mid);
			Triangle x = Triangle(E2_mid, tri.b, E3_mid);
			Triangle y = Triangle(E1_mid, E2_mid, E3_mid);
			Triangle z = Triangle(E1_mid, E3_mid, tri.c);

			unfinished->push_back(w);
			unfinished->push_back(x);
			unfinished->push_back(y);
			unfinished->push_back(z);
			break;

		} 
		case EDGE3 | EDGE1: {
			struct deriv_point_adaptive E1_mid, E3_mid;
			
			tri.get_midpoint(1, &E1_mid);
			tri.get_midpoint(3, &E3_mid);

			convert_midpoint(&E1_mid);
			convert_midpoint(&E3_mid);


			Triangle x = Triangle(tri.a, tri.b, E1_mid);
			Triangle y = Triangle(E1_mid, tri.b, E3_mid);
			Triangle z = Triangle(E1_mid, E3_mid, tri.c);

			unfinished->push_back(x);
			unfinished->push_back(y);
			unfinished->push_back(z);
			break;

		} 
		case EDGE2 | EDGE3: {
			struct deriv_point_adaptive E2_mid, E3_mid;
			
			tri.get_midpoint(2, &E2_mid);
			tri.get_midpoint(3, &E3_mid);

			convert_midpoint(&E2_mid);
			convert_midpoint(&E3_mid);

			Triangle x = Triangle(tri.a, E2_mid, E3_mid);
			Triangle y = Triangle(E2_mid, tri.b, E3_mid);
			Triangle z = Triangle(tri.a, E3_mid, tri.c);

			unfinished->push_back(x);
			unfinished->push_back(y);
			unfinished->push_back(z);
			break;

		} 
		case EDGE2 | EDGE1: {

			struct deriv_point_adaptive E1_mid, E2_mid;
			
			tri.get_midpoint(1, &E1_mid);
			tri.get_midpoint(2, &E2_mid);


			convert_midpoint(&E1_mid);
			convert_midpoint(&E2_mid);

			Triangle x = Triangle(tri.a, E2_mid, E1_mid);
			Triangle y = Triangle(E1_mid, E2_mid, tri.c);
			Triangle z = Triangle(E2_mid, tri.b, tri.c);

			unfinished->push_back(x);
			unfinished->push_back(y);
			unfinished->push_back(z);
			break;

		} 
		case EDGE3: {
			struct deriv_point_adaptive E3_mid;
			
			tri.get_midpoint(3, &E3_mid);

			convert_midpoint(&E3_mid);


			Triangle x = Triangle(tri.a, tri.b, E3_mid);
			Triangle y = Triangle(tri.a, E3_mid, tri.c);

			unfinished->push_back(x);
			unfinished->push_back(y);
			break;

		} 
		case EDGE2: {
			struct deriv_point_adaptive E2_mid;
			
			tri.get_midpoint(2, &E2_mid);

			convert_midpoint(&E2_mid);


			Triangle x = Triangle(tri.a, E2_mid, tri.c);
			Triangle y = Triangle(E2_mid, tri.b, tri.c);

			unfinished->push_back(x);
			unfinished->push_back(y);
			break;

		} 
		case EDGE1: {
			struct deriv_point_adaptive E1_mid;
			
			tri.get_midpoint(1, &E1_mid);

			convert_midpoint(&E1_mid);


			Triangle x = Triangle(tri.a, tri.b, E1_mid);
			Triangle y = Triangle(E1_mid, tri.b, tri.c);

			unfinished->push_back(x);
			unfinished->push_back(y);
			break;
		}
		default: {
			printf("Exception: flag does not match any valid value: %d", FLAG);
			exit(0);
		}

	}
}

void bPatch::convert_midpoint(deriv_point_adaptive *mid){
	struct deriv_point bez_mid;
	p_interp(mid->u, mid->v, &bez_mid);
	mid->deriv = bez_mid.deriv;
	mid->p = bez_mid.p;
}

struct deriv_point *bPatch::p_interp(float u, float v, struct deriv_point *p_n) {
	assert (u >= 0.0 && u <= 1.0);
	assert (v >= 0.0 && v <= 1.0);
	//Build control points for a bez_curve along V.
	vector<Vector3f> v_points;
	struct deriv_point dp;
	for (int i = 0; i < 4; i ++) {
		bCurve bcurve_i =  bCurve(ctrl_pts.at(0 + i * 4),ctrl_pts.at(1 + i * 4), 
									ctrl_pts.at(2 + i * 4), ctrl_pts.at(3 + i * 4));
		v_points.push_back(bcurve_i.c_interp(u, &dp)->p);
	}

	vector<Vector3f> u_points;

	for (int j = 0; j < 4; j++) {
		bCurve bcurve_j = bCurve(ctrl_pts.at(0 + j), ctrl_pts.at(4 + j), 
							ctrl_pts.at(8 + j), ctrl_pts.at(12 + j));
		u_points.push_back((bcurve_j.c_interp(v, &dp)->p));
	}

	//now evaluate each curve for point and derivative. 
	struct deriv_point d_p_u;
	struct deriv_point d_p_v;
	bCurve(v_points.at(0), v_points.at(1), v_points.at(2), v_points.at(3)).c_interp(v, &d_p_v);
	bCurve(u_points.at(0), u_points.at(1), u_points.at(2), u_points.at(3)).c_interp(u, &d_p_u);
	// They should match within some reasonable value. 
	assert (abs(d_p_v.p[0] - d_p_u.p[0]) < 0.0001);
	assert (abs(d_p_v.p[1] - d_p_u.p[1]) < 0.0001);
	assert (abs(d_p_v.p[2] - d_p_u.p[2]) < 0.0001);

	//take cross of partials to get normal 
	Vector3f normal = d_p_v.deriv.cross(d_p_u.deriv);
	// cout << "Deriv 1:" << d_p_v.deriv.format(CommaInitFmt) << " Deriv 2" 
	// 	<< d_p_v.deriv.format(CommaInitFmt) << " Cross" << normal.format(CommaInitFmt) << "\n";
	normal.normalized(); 
	p_n->p = d_p_u.p;
	p_n->deriv = normal;
	return p_n;
}

bCurve::bCurve(Vector3f a, Vector3f b, Vector3f c, Vector3f d) {
	this->ctrl_pts.push_back(a);
	this->ctrl_pts.push_back(b);
	this->ctrl_pts.push_back(c);
	this->ctrl_pts.push_back(d);
}

struct deriv_point *bCurve::c_interp(float u, struct deriv_point *d_p) {
	assert (u >= 0.0 && u <= 1.0);
	//forming line segments AB and BC from control points
	//cout << "control points: \n" << ctrl_pts[0] << "\n\n" << ctrl_pts[1] <<"\n"; 
	Vector3f A = ctrl_pts.at(0) * (1.0 - u) + ctrl_pts.at(1) *  u;
	Vector3f B = ctrl_pts[1] * (1.0 - u) + ctrl_pts[2] * u;
	Vector3f C = ctrl_pts[2] * (1.0 - u) + ctrl_pts[3] * u;

	//form line segment DE from AB and BC:
	Vector3f D = A * (1.0 - u) + B * u;
	Vector3f E = B * (1.0 - u) + C * u;

	//our point to return lies on DE
	Vector3f point_to_return = D * (1.0 - u) + E * u;
	//??? Not exactly sure about this but gives us derivative w/ respect to u.
	Vector3f dPdu = 3 * (E - D);

	d_p->deriv = dPdu;
	d_p->p = point_to_return;
	return d_p;
}