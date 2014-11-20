# given a patch, perform uniform subdivision
subdividepatch(patch, step)
	 # compute how many subdivisions there 
	 # are for this step size
	 numdiv = ((1 + epsilon) / step)
	 
	 # for each parametric value of u
	 for (iu = 0 to numdiv)
		 u = iu * step
		 
		 # for each parametric value of v
		 for (iv = 0 to numdiv)
			 v = iv * step
			 
			 # evaluate surface
			 p, n = bezpatchinterp(patch, u, v)
			 savesurfacepointandnormal(p,n)