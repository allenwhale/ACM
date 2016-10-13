#define EPS 1e-6 
#define INF 1e9
const double PI = acos(-1);
int cmp(double x){
	if(fabs(x) < EPS) return 0;
	if(x > 0) return 1;
	else return -1;
}
struct Point{
	double x ,y;
	Point(double _x=0, double _y=0): x(_x), y(_y) {}
	Point operator + (const Point &rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator - (const Point &rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
	Point operator * (const double &rhs) const {
		return Point(x * rhs, y * rhs);
	}
	Point operator / (const double &rhs) const {
		return Point(x / rhs, y / rhs);
	}
	bool operator == (const Point &rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	double Abs() const {
		return sqrt(Dot(*this));
	}
	/*
	 * range: 0 ~ 2*PI
	 */
	double Arg() const {
		double res = atan2(y, x);
		if(cmp(res) < 0) res += PI * 2.0;
		return res;
	}
	double Dot(const Point &rhs) const {
		return (x * rhs.x + y * rhs.y);
	}
	double Cross(const Point &rhs) const {
		return (x * rhs.y - y * rhs.x);
	}
	double Dist(const Point &rhs) const {
		return (*this - rhs).Abs();
	}
	/*
	 * unit of d is radian(counterclockwise)
	 */
	Point Rotate(double d) const {
		return Rotate(cos(d), sin(d));
	}
	Point Rotate(double cost, double sint) const {
		return Point(x * cost - y * sint, x * sint + y * cost);
	}
	bool operator < (const Point &rhs) const {
		if(x == rhs.x)
			return y < rhs.y;
		return x < rhs.x;
	}
	friend ostream& operator << (ostream &out, const Point &rhs){
		out << "(" << rhs.x << ", " << rhs.y << ")";
		return out;
	}
	Point& update(){
		if(cmp(x) == 0) x = 0;
		if(cmp(y) == 0) y = 0;
		return *this;
	}
}nilPoint(INF, INF);
