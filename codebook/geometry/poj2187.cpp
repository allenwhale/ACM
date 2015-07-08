#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
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
    double Abs(){
        return sqrt(x*x + y*y);
    }
    double Dot(const Point &rhs){
        return (x*rhs.x + y*rhs.y);
    }
    double Cross(const Point &rhs){
        return (x*rhs.y - y*rhs.x);
    }
    double Dist(const Point &rhs){
        return (*this-rhs).Abs();
    }
    Point Rotate(double d){
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
    Point Intersection(const Line &rhs){
        if(Parallel(rhs)) return nilPoint;
        /* for segment */
        if(IsIntersect(rhs) == false) return nilPoint;
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
#define next(x) ((x+1)%N)
#define dist(a, b) ((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y))
    pair<int, pair<Point, Point> > Diameter(){
        if(N == 1)
            return make_pair(0, make_pair(s[0], s[0]));
        int maxd = 0;
        Point pa, pb;
        for(int i=0,j=1;i<N;i++){
            while(cmp((s[next(i)]-s[i]).Cross(s[j]-s[i])-(s[next(i)]-s[i]).Cross(s[next(j)]-s[i])) < 0)
                j = next(j);
            int d = dist(s[i], s[j]);
            if(d > maxd)
                maxd = d, pa = s[i], pb = s[j];
            d = dist(s[next(i)], s[next(j)]);
            if(d > maxd)
                maxd = d, pa = s[next(i)], pb = s[next(j)];
        }
        return make_pair(maxd, make_pair(pa, pb));
    }  
};

int main(){
    int N;
    scanf("%d", &N);
    Polygon poly(N);
    for(int i=0;i<N;i++){
        double x, y;
        scanf("%lf %lf", &x, &y);
        poly.add(Point(x, y));
    }
    poly = poly.ConvexHull();
    printf("%d\n", poly.Diameter().first);
    return 0;
}
