#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <deque>
using namespace std;
#define next(x) ((x+1)%N)
#define prev(x) ((x-1+N)%N)
const double EPS = 1e-9;
const double INF = 1e9;
const double PI = acos(-1);
int cmp(double x){
    if(fabs(x) < EPS) return 0;
    if(x < 0) return -1;
    return 1;
}
class Point{
public:
    double x ,y;
    Point(double _x=0, double _y=0): x(_x), y(_y) {}
    Point operator + (const Point &rhs) const {
        return Point(x+rhs.x, y+rhs.y);
    }
    Point operator - (const Point &rhs) const {
        return Point(x-rhs.x, y-rhs.y);
    }
    Point operator * (const double &rhs) const {
        return Point(x*rhs, y*rhs);
    }
    Point operator / (const double &rhs) const {
        return Point(x/rhs, y/rhs);
    }
    bool operator == (const Point &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    double Abs() const {
        return sqrt(x*x + y*y);
    }
    double Arg() const {
        double res = atan2(y, x);
        if(cmp(res+PI/2.0) <= 0) res += PI*2.0;
        return res;
    }
    double Dot(const Point &rhs) const {
        return (x*rhs.x + y*rhs.y);
    }
    double Cross(const Point &rhs) const {
        return (x*rhs.y - y*rhs.x);
    }
    double Dist(const Point &rhs) const {
        return (*this-rhs).Abs();
    }
    Point Rotate(double d) const {
        return Point(x*cos(d)-y*sin(d), x*sin(d)+y*cos(d));
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
        if(cmp(x) == 0)
            x = 0;
        if(cmp(y) == 0)
            y = 0;
        return *this;
    }
}nilPoint(INF, INF);

class Line{
public:
    Point a, b;
    Line(Point _a=Point(), Point _b=Point()): a(_a), b(_b) {}
    double Dist(const Point &rhs){
        if(cmp((rhs-a).Dot(b-a)) < 0) return (rhs-a).Abs();
        if(cmp((rhs-b).Dot(a-b)) < 0) return (rhs-b).Abs();
        return fabs((a-rhs).Cross(b-rhs) / a.Dist(b));
    }
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
    Line move(const double &d){
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

/*
 * default is counterclockwise
 */
class Polygon{
#define COUNTERCLOCKWISE 1
#define CLOCKWISE       -1
public:
	int N;
	vector<Point> s;
    vector<double> A;
	Polygon(int n=0): N(n) {}
	void add(const Point &n){
		s.push_back(n);
	}
    int Order(){
        int t = 0;
        for(int i=0;i<N&&t==0;i++){
            int a = i, b = (i+1)%N, c = (i+2)%N;
            t = (s[b]-s[a]).Cross(s[c]-s[b]);
        }
        return t;
    }
	double Perimeter(){
		double res = 0;
		for(int i=0;i<N;i++)
			res += s[i].Dist(s[(i+1)%N]);
		return res;
	}
	double Area(){
		double res = 0;
		for(int i=0;i<N;i++)
			res += s[i].Cross(s[(i+1)%N]);
		return fabs(res/2.0);
	}
#define INSIDE	1
#define ONEDGE	2
#define OUTSIDE	0
	int OnPolygon(const Point &n){
		Point rfn = Point(-INF, n.y);
		Line l = Line(n, rfn);
		int cnt = 0;
		for(int i=0;i<N;i++){
			if(Line(s[i], s[(i+1)%N]).OnLine(n))
				return ONEDGE;
			if(cmp(s[i].y - s[(i+1)%N].y) == 0)
				continue;
			if(l.OnLine(s[i])){
				if(cmp(s[i].y - s[(i+1)%N].y) >= 0)
					cnt++;
			}else if(l.OnLine(s[(i+1)%N])){
				if(cmp(s[(i+1)%N].y - s[i].y) >= 0)
					cnt++;
			}else if(l.IsIntersect(Line(s[i], s[(i+1)%N])))
				cnt++;
		}
		return (cnt&1);
	}
    Point MassCenter(){
        if(cmp(Area()) == 0)return nilPoint;
        Point res;
        for(int i=0;i<N;i++)
            res = res + (s[i] + s[(i+1)%N]) * s[i].Cross(s[(i+1)%N]);
        return res / Area() / 6.0;
    }
    int PointsOnedge(){
        int res = 0;
        for(int i=0;i<N;i++)
            res += __gcd(abs(int(s[(i+1)%N].x-s[i].x)), abs(int(s[(i+1)%N].y-s[i].y)));
        return res;
    }
    int PointsInside(){
        return int(Area()) + 1 - PointsOnedge()/2;
    }
    Polygon ConvexHull(){
        Polygon res, that = *this;
        sort(that.s.begin(), that.s.end());
        that.s.erase(unique(that.s.begin(), that.s.end()), that.s.end());
        vector<Point> &w = res.s;
        for(int i=0;i<(int)that.s.size();i++){
            int sz;
            while((sz=w.size()), 
                    sz > 1 && cmp((w[sz-1]-w[sz-2]).Cross(that.s[i]-w[sz-2])) <= 0)
                w.pop_back();
            w.push_back(that.s[i]);
        }
        int k = w.size();
        for(int i=(int)that.s.size()-2;i>=0;i--){
            int sz;
            while((sz=w.size()), 
                    sz > k && cmp((w[sz-1]-w[sz-2]).Cross(that.s[i]-w[sz-2])) <= 0)
                w.pop_back();
            w.push_back(that.s[i]);
        }
        if((int)that.s.size() > 1) w.pop_back();
        res.N = w.size();
        res.A = vector<double>(res.N);
        for(int i=0;i<res.N;i++)
            res.A[i] = (res.s[(i+1)%res.N]-res.s[i]).Arg();
        return res;
    }
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
    /*
     * farthest node pair
     */
    pair<double, pair<Point, Point> > Diameter(){
        if(N == 1)
            return make_pair(0, make_pair(s[0], s[0]));
        double maxd = 0;
        Point pa, pb;
        for(int i=0,j=1;i<N;i++){
            while(cmp((s[next(i)]-s[i]).Cross(s[j]-s[i])-(s[next(i)]-s[i]).Cross(s[next(j)]-s[i])) < 0)
                j = next(j);
            double d = s[i].Dist(s[j]);
            if(d > maxd)
                maxd = d, pa = s[i], pb = s[j];
            d = s[next(i)].Dist(s[next(j)]);
            if(d > maxd)
                maxd = d, pa = s[next(i)], pb = s[next(j)];
        }
        return make_pair(maxd, make_pair(pa, pb));
    }  
    bool IsIntersect(const Line &rhs){
        int i = (upper_bound(A.begin(), A.end(), (rhs.b-rhs.a).Arg()) - A.begin()) % N;
        int j = (upper_bound(A.begin(), A.end(), (rhs.a-rhs.b).Arg()) - A.begin()) % N;
        if(cmp((rhs.b-rhs.a).Cross(s[i]-rhs.a)*(rhs.b-rhs.a).Cross(s[j]-rhs.a)) <= 0)
            return true;
        return false;
    }
};

class HalfPlane{
public:
    Point a, b;
    /* rhs1 -> rhs2 left side */
    HalfPlane(const Point &_a=Point(), const Point &_b=Point()): a(_a), b(_b) {}
    double Value(const Point &rhs) const {
        return (rhs-a).Cross(b-a) ;
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
        int res = cmp((b-a).Arg() - (rhs.b-rhs.a).Arg());
        return res == 0 ? rhs.Satisfy(a) : (res<0);
    }
    friend ostream& operator << (ostream& out, const HalfPlane &rhs){
        out << "{" << rhs.a << ", " << rhs.b << "}";
        return out;
    }
};
class HalfPlaneSet{
public:
    vector<HalfPlane> s;
    void add(const HalfPlane &rhs){
        s.push_back(rhs);
    }
    Polygon Solve(){
        Polygon res;
        sort(s.begin(), s.end());
        deque<HalfPlane> q;
        deque<Point> ans;
        q.push_back(s[0]);
        for(int i=1;i<(int)s.size();i++){
            if(cmp((s[i].b-s[i].a).Arg()-(s[i-1].b-s[i-1].a).Arg()) == 0) continue;
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

Polygon Kernel(const Polygon &rhs){
    HalfPlaneSet hlps;
    for(int i=0;i<rhs.N;i++)
        hlps.add(HalfPlane(rhs.s[i], rhs.s[(i+1)%rhs.N]));
    return hlps.Solve();
}
class Circle{
public:
    Point O;
    double R;
    Circle(const Point &o, const double &r): O(o), R(r) {}
    double Area() const {
        return PI * R * R;
    }
    double Perimeter() const {
        return 2.0 * PI * R;
    }
    /*
     * default is segment
     * if want to change it to line, remove the if which judge t
     */
    vector<Point> Intersection(const Line &rhs){
        vector<Point> res;
        Point d1 = rhs.b - rhs.a, d2 = rhs.a - O; 
        double A = d1.x*d1.x + d1.y*d1.y;
        double B = 2.0 * d1.Dot(rhs.a-O);
        double C = d2.x*d2.x + d2.y*d2.y - R*R;
        double D = B*B -4*A*C;
        if(cmp(D) >= 0){
            double t1 = (-B - sqrt(max(0.0, D))) / (2.0*A);
            double t2 = (-B + sqrt(max(0.0, D))) / (2.0*A);
            if(cmp(t1-1) <= 0 && cmp(t1) >= 0)
                res.push_back(rhs.a + d1*t1);
            if(cmp(t2-1) <= 0 && cmp(t2) >= 0)
                res.push_back(rhs.a + d1*t2);
        }
        return res;
    }
};

int main(){
    Circle c(Point(0, 0), 4);
    vector<Point> ans = c.Intersection(Line(Point(4, 1), Point(4, 0)));
    for(auto p: ans)
        cout << p << endl;

    return 0;
}
