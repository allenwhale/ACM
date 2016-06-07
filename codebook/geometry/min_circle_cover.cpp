/*
 * circumcircle of two points
 */
Circle Center(const Point &rhs1, const Point &rhs2){
	return Circle((rhs1+rhs2)/2.0, rhs1.Dist(rhs2)/2.0);
}
/*
 * circumcircle of three points
 */
Circle Center(const Point &rhs1, const Point &rhs2, const Point &rhs3){
	Circle res(rhs1, 0);
	Point d1 = rhs2 - rhs1, d2 = rhs3 - rhs1;
	double c1 = (d1.x * d1.x + d1.y * d1.y) / 2.0, 
           c2 = (d2.x * d2.x + d2.y * d2.y) / 2.0;
	double d = d1.Cross(d2);
	res.O.x += (c1 * d2.y - c2 * d1.y) / d;
	res.O.y += (c2 * d1.x - c1 * d2.x) / d;
	res.R = res.O.Dist(rhs1);
	return res;
}
Circle MinCircleCover(vector<Point> rhs){
	random_shuffle(rhs.begin(), rhs.end());
	Circle res(rhs[0], 0);
	for(int i=1;i<(int)rhs.size();i++){
		if(!res.InCircle(rhs[i])){
			res = Circle(rhs[i], 0);
			for(int j=0;j<i;j++){
				if(!res.InCircle(rhs[j])){
					res = Center(rhs[i], rhs[j]);
					for(int k=0;k<j;k++){
						if(!res.InCircle(rhs[k])){
							res = Center(rhs[i], rhs[j], rhs[k]);
						}
					}
				}
			}
		}
	}
	return res;
}
