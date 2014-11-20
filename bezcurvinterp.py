# given the control points of a bezier curve
# and a parametric value, return the curve 
# point and derivative

bezcurveinterp(curve, u)
	# first, split each of the three segments
	# to form two new ones AB and BC
	A = curve[0] * (1.0-u) + curve[1] * u
	B = curve[1] * (1.0-u) + curve[2] * u
	C = curve[2] * (1.0-u) + curve[3] * u

	# now, split AB and BC to form a new segment DE
	 D = A * (1.0-u) + B * u
	 E = B * (1.0-u) + C * u

	# finally, pick the right point on DE,
	# this is the point on the curve
	 p = D * (1.0-u) + E * u

	# compute derivative also
	dPdu = 3 * (E - D)

