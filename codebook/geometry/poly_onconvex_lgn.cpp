/*
 * O(lg N)
 */
int OnConvex(const Point &rhs){
	Point rfn = (s[0]+s[N/3]+s[2*N/3]) / 3.0;
	int l = 0, r = N;
	while(l+1 < r){
		int mid = (l+r) / 2;
		if(cmp((s[l]-rfn).Cross(s[mid]-rfn)) > 0){
			if(cmp((s[l]-rfn).Cross(rhs-rfn)) >= 0 && cmp((s[mid]-rfn).Cross(rhs-rfn)) < 0)
				r = mid;
			else l = mid;
		}else{
			if(cmp((s[l]-rfn).Cross(rhs-rfn)) < 0 && cmp((s[mid]-rfn).Cross(rhs-rfn)) >= 0)
				l = mid;
			else r = mid;
		}
	}
	r %= N;
	int z = cmp((s[r]-rhs).Cross(s[l]-rhs));
	if(z == 0) return ONEDGE;
	else if(z == 1) return OUTSIDE;
	else return INSIDE;
}
