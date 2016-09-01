struct HalfPlaneSet{
	vector<HalfPlane> s;
	HalfPlaneSet& add(const HalfPlane &rhs){
		s.push_back(rhs);
		return *this;
	}
	/*
	 * return the polygon that satisfies all halfplanes
	 */
	Polygon Solve(){
		Polygon res;
		sort(s.begin(), s.end());
		deque<HalfPlane> q;
		deque<Point> ans;
		q.push_back(s[0]);
		for(int i=1;i<(int)s.size();i++){
			if(cmp((s[i].b - s[i].a).Arg() - (s[i - 1].b - s[i - 1].a).Arg()) == 0) continue;
			while(ans.size() > 0 && cmp(s[i].Value(ans.back())) >= 0){
				ans.pop_back();
				q.pop_back();
			}
			while(ans.size() > 0 && cmp(s[i].Value(ans.front())) >= 0){
				ans.pop_front();
				q.pop_front();
			}
			ans.push_back(q.back().Intersection(s[i]));
			q.push_back(s[i]);
		}
		while(ans.size() > 0 && cmp(q.front().Value(ans.back())) >= 0){
			ans.pop_back();
			q.pop_back();
		}
		while(ans.size() > 0 && cmp(q.back().Value(ans.front())) >= 0){
			ans.pop_front();
			q.pop_front();
		}
		ans.push_back(q.back().Intersection(q.front()));
		for(int i=0;i<(int)ans.size();i++)
			res.add(ans[i]);
		res.N = res.s.size();
		return res;
	}
};
