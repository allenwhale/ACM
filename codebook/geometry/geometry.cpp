#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;
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
    double abs(){
        return sqrt(x*x + y*y);
    }
    double dot(const Point &rhs){
        return (x*rhs.x + y*rhs.y);
    }
    double cross(const Point &rhs){
        return (x*rhs.y - y*rhs.x);
    }
    double dist(const Point &rhs){
        return (*this-rhs).abs();
    }
    Point rotate(double d){
        return Point(x*cos(d)-y*sin(d), x*sin(d)+y*cos(d));
    }
    friend ostream& operator << (ostream &out, const Point &rhs){
        out << "(" << rhs.x << ", " << rhs.y << ")";
        return out;
    }
}nilPoint(INF, INF);

class Line{
public:
    Point a, b;
    Line(Point _a=Point(), Point _b=Point()): a(_a), b(_b) {}
    double dist(const Point &rhs){
        if(cmp((rhs-a).dot(b-a)) < 0) return (rhs-a).abs();
        if(cmp((rhs-b).dot(a-b)) < 0) return (rhs-b).abs();
        return fabs((a-rhs).cross(b-rhs) / a.dist(b));
    }
    Point proj(const Point &rhs){
        double r = (a-b).dot(rhs-b) / (a-b).dot(a-b);
        return b+(a-b)*r;
    }
    bool online(const Point &rhs){
        /* for segment */
        return cmp((rhs-b).cross(a-b)) == 0 && cmp((rhs-b).dot(rhs-a)) <= 0;
        /* for line */
        return cmp((rhs-b).cross(a-b)) == 0;
    }
    bool parallel(const Line &rhs){
        return !cmp((a-b).cross(rhs.a-rhs.b));
    }
    bool isintersect(const Line &rhs){
        if(cmp((rhs.a-a).cross(rhs.b-a) * (rhs.a-b).cross(rhs.b-b)) > 0) return false;
        if(cmp((a-rhs.a).cross(b-rhs.a) * (a-rhs.b).cross(b-rhs.b)) > 0) return false;
        return true;
    }
    Point intersection(const Line &rhs){
        if(parallel(rhs)) return nilPoint;
        /* for segment */
        if(isintersect(rhs) == false) return nilPoint;
        /* end */
        double s1 = (a-rhs.a).cross(rhs.b-rhs.a);
        double s2 = (b-rhs.a).cross(rhs.b-rhs.a);
        return (b*s1-a*s2) / (s1-s2);
    }
    Line move(const double &d){
        Point tmp = b - a;
        tmp = tmp / tmp.abs();
        tmp = tmp.rotate(PI/2);
        return Line(a+tmp*d, b+tmp*d);
    }

    friend ostream& operator << (ostream &out, const Line &rhs){
        out << "[" << rhs.a << ", " << rhs.b << "]";
        return out;
    }
}nilLine(nilPoint, nilPoint);

class Polygon{
public:
	int N;
	vector<Point> s;
	Polygon(int n=0): N(n) {}
	void add(const Point &n){
		s.push_back(n);
	}
	double perimeter(){
		double res = 0;
		for(int i=0;i<N;i++)
			res += s[i].dist(s[(i+1)%N]);
		return res;
	}
	double area(){
		double res = 0;
		for(int i=0;i<N;i++)
			res += s[i].cross(s[(i+1)%N]);
		return fabs(res/2.0);
		return res;
	}
#define INSIDE	1
#define ONEDGE	2
#define OUTSIDE	0
	int inpoly(const Point &n){
		Point rfn = Point(-INF, n.y);
		Line l = Line(n, rfn);
		int cnt = 0;
		for(int i=0;i<N;i++){
			if(Line(s[i], s[(i+1)%N]).online(n))
				return ONEDGE;
			if(cmp(s[i].y - s[(i+1)%N].y) == 0)
				continue;
			if(l.online(s[i])){
				if(cmp(s[i].y - s[(i+1)%N].y) >= 0)
					cnt++;
			}else if(l.online(s[(i+1)%N])){
				if(cmp(s[(i+1)%N].y - s[i].y) >= 0)
					cnt++;
			}else if(l.isintersect(Line(s[i], s[(i+1)%N])))
				cnt++;
		}
		return (cnt&1);
	}
};

int main(){
    Point O(0, 0);
    Point I(1, 1);
    Line  L1(O, I), L2(Point(0, 3), Point(3, 0));
    Line L3(Point(-1, 1), Point(0, 1)), L4(Point(1, 1), Point(1, 0));
//    printf("%f\n", L1.dist(Point(0, 1)));
//    cout << L1.proj(Point(0, 0)) << endl;
//    printf("%d\n", L1.online(Point(0.5, 0.5)));
//    cout << L1 << endl;
//    cout << nilPoint << " " << nilLine << endl;
//    cout << L1.intersection(L2) << endl;
//    cout << L4.intersection(L3) << endl;
//    cout << L1.move(sqrt(2)) << endl;
//  cout << L1.isintersect(Line(Point(0, 0), Point(-1, -1)));

    
    return 0;
}
