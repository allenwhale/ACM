/* 
 * called by find 
 * 1 = positive, -1 = negative, 0 = zero
 */
int sign(double x){
	return x < -EPS ? -1 : x > EPS;
}
/* called by equation */
template<class T>
double find(const T &f, double lo, double hi){
	int sign_lo, sign_hi;
	if((sign_lo=sign(f(lo))) == 0) return lo;
	if((sign_hi=sign(f(hi))) == 0) return hi;
	if(sign_hi * sign_lo > 0) return INF;
	while(hi-lo>EPS){
		double m = (hi+lo) / 2;
		int sign_mid = sign(f(m));
		if(sign_mid == 0) return m;
		if(sign_lo * sign_mid < 0)
			hi = m;
		else lo = m;
	}
	return (lo+hi) / 2;
}
/*
 * return a set of answer of f(x) = 0
 */
template<class T>
vector<double> equation(const T &f){
	vector<double> res;
	if(f.degree() == 1){
		if(sign(f.coef[1]))res.push_back(-f.coef[0]/f.coef[1]);
		return res;
	}
	vector<double> droot = equation(f.derivative());
	droot.insert(droot.begin(), -INF);
	droot.push_back(INF);
	for(int i=0;i<(int)droot.size()-1;i++){
		double tmp = find(f, droot[i], droot[i+1]);
		if(tmp < INF) res.push_back(tmp);
	}
	return res;
}
