Polygon ConvexHull(){
    //it is clockwise
	Polygon res, that = *this;
	sort(that.s.begin(), that.s.end());
	that.s.erase(unique(that.s.begin(), that.s.end()), that.s.end());
	vector<Point> &w = res.s;
	for(int i=0;i<(int)that.s.size();i++){
		int sz;
		while((sz=w.size()), 
				sz > 1 && cmp((w[sz - 1] - w[sz - 2]).Cross(that.s[i] - w[sz - 2])) <= 0)
			w.pop_back();
		w.push_back(that.s[i]);
	}
	int k = w.size();
	for(int i=(int)that.s.size()-2;i>=0;i--){
		int sz;
		while((sz=w.size()), 
				sz > k && cmp((w[sz - 1] - w[sz - 2]).Cross(that.s[i] - w[sz - 2])) <= 0)
			w.pop_back();
		w.push_back(that.s[i]);
	}
	if((int)that.s.size() > 1) w.pop_back();
	res.N = w.size();
	//res.A = vector<double>(res.N);
	//for(int i=0;i<res.N;i++)
		//res.A[i] = (res.s[(i + 1) % res.N] - res.s[i]).Arg();
	return res;
}
