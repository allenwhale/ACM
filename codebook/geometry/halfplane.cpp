class HalfPlane{
public:
	Point a, b;
	/* a -> b left side */
	HalfPlane(const Point &_a=Point(), const Point &_b=Point()): a(_a), b(_b) {}
	double Value(const Point &rhs) const {
		return (rhs - a).Cross(b - a) ;
	}
	bool Satisfy(const Point &rhs) const {
		return cmp(Value(rhs)) <= 0;
	}
	Point Intersection(const Point &rhs1, const Point &rhs2){
		return Line(a, b).Intersection(Line(rhs1, rhs2));
	}
	Point Intersection(const HalfPlane &rhs){
		return Line(a, b).Intersection(Line(rhs.a, rhs.b));
	}
	/*
	 * return the polygon cut by halfplane
	 */
	Polygon Cut(const Polygon &rhs){
		Polygon res;
		const vector<Point> &w = rhs.s;
		int N = w.size();
		for(int i=0;i<(int)w.size();i++){
			if(cmp(Value(w[i])) <= 0)
				res.s.push_back(w[i]);
			else{
				if(cmp(Value(w[prev(i)])) < 0)
					res.s.push_back(Intersection(w[prev(i)], w[i]));
				if(cmp(Value(w[next(i)])) < 0)
					res.s.push_back(Intersection(w[i], w[next(i)]));
			}
		}
		res.N = res.s.size();
		return res;
	}
	bool operator < (const HalfPlane &rhs) const {
		int res = cmp((b - a).Arg() - (rhs.b - rhs.a).Arg());
		return res == 0 ? rhs.Satisfy(a) : (res < 0);
	}
	friend ostream& operator << (ostream& out, const HalfPlane &rhs){
		out << "{" << rhs.a << ", " << rhs.b << "}";
		return out;
	}
};
