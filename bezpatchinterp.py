# given a control patch and (u,v) values, find 
# the surface point and normal
bezpatchinterp(patch, u, v)
	 # build control points for a Bezier curve in v
	 vcurve[0] = bezcurveinterp(patch[0][0:3], u)
	 vcurve[1] = bezcurveinterp(patch[1][0:3], u)
	 vcurve[2] = bezcurveinterp(patch[2][0:3], u)
	 vcurve[3] = bezcurveinterp(patch[3][0:3], u)
	 
	 # build control points for a Bezier curve in u
	 ucurve[0] = bezcurveinterp(patch[0:3][0], v)
	 ucurve[1] = bezcurveinterp(patch[0:3][1], v)
	 ucurve[2] = bezcurveinterp(patch[0:3][2], v)
	 ucurve[3] = bezcurveinterp(patch[0:3][3], v)
	 
	 # evaluate surface and derivative for u and v
	 p, dPdv = bezcurveinterp(vcurve, v)
	 p, dPdu = bezcurveinterp(ucurve, u)
	 # take cross product of partials to find normal
	 n = cross(dPdu, dPdv)
	 n = n / length(n)
	 
	 return p, n