class Line{
public:
	Point a, b;
	Line(Point _a=Point(), Point _b=Point()): a(_a), b(_b) {}
	double Dist(const Point &rhs){
		if(cmp((rhs-a).Dot(b-a)) < 0) return (rhs-a).Abs();
		if(cmp((rhs-b).Dot(a-b)) < 0) return (rhs-b).Abs();
		return fabs((a-rhs).Cross(b-rhs) / a.Dist(b));
	}
	/*
	 * the pedal of rhs on line
	 */
	Point Proj(const Point &rhs){
		double r = (a-b).Dot(rhs-b) / (a-b).Dot(a-b);
		return b+(a-b)*r;
	}
	bool OnLine(const Point &rhs){
		/* for segment */
		return cmp((rhs-b).Cross(a-b)) == 0 && cmp((rhs-b).Dot(rhs-a)) <= 0;
		/* for line */
		return cmp((rhs-b).Cross(a-b)) == 0;
	}
	bool Parallel(const Line &rhs){
		return !cmp((a-b).Cross(rhs.a-rhs.b));
	}
	bool IsIntersect(const Line &rhs){
		if(cmp((rhs.a-a).Cross(rhs.b-a) * (rhs.a-b).Cross(rhs.b-b)) > 0) return false;
		if(cmp((a-rhs.a).Cross(b-rhs.a) * (a-rhs.b).Cross(b-rhs.b)) > 0) return false;
		return true;
	}
	/* default is line */
	Point Intersection(const Line &rhs, bool flag=false){
		if(Parallel(rhs)) return nilPoint;
		/* for segment */
		if(flag && IsIntersect(rhs) == false) return nilPoint;
		/* end */
		double s1 = (a-rhs.a).Cross(rhs.b-rhs.a);
		double s2 = (b-rhs.a).Cross(rhs.b-rhs.a);
		return (b*s1-a*s2) / (s1-s2);
	}
	/*
	 * move d units along the direction of line
	 * example: {(0, 0) -> (1, 1)} move _/2 becomes {(1, 1) -> (2, 2)}
	 */
	Line Move(const double &d){
		Point tmp = b - a;
		tmp = tmp / tmp.Abs();
		tmp = tmp.Rotate(PI/2);
		return Line(a+tmp*d, b+tmp*d);
	}

	friend ostream& operator << (ostream &out, const Line &rhs){
		out << "[" << rhs.a << ", " << rhs.b << "]";
		return out;
	}
}nilLine(nilPoint, nilPoint);
