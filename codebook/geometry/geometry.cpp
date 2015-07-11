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
    /*
     * range: 0 ~ 2*PI
     */
    double Arg() const {
        double res = atan2(y, x);
        if(cmp(res) < 0) res += PI*2.0;
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
    /*
     * unit of d is radian
     */
    Point Rotate(double d) const {
        return Rotate(cos(d), sin(d));
    }
    Point Rotate(double cost, double sint) const {
        return Point(x*cost-y*sint, x*sint+y*cost);
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
	Polygon& add(const Point &n){
		s.push_back(n);
        return *this;
	}
    /*
     * counterclockwise or clockwise
     * defined as above
     */
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
    /*
     * O(lg N)
     */
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
/*
 * the kernel oh the polygon
 */
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
     * default not includes on the edge
     */
    bool InCircle(const Point &rhs) const {
        return cmp(O.Dist(rhs) - R) <= 0;
    }
    /*
     * circumcircle of two points
     */
    static Circle Center(const Point &rhs1, const Point &rhs2){
        return Circle((rhs1+rhs2)/2.0, rhs1.Dist(rhs2)/2.0);
    }
    /*
     * circumcircle of three points
     */
    static Circle Center(const Point &rhs1, const Point &rhs2, const Point &rhs3){
        Circle res(rhs1, 0);
        Point d1 = rhs2 - rhs1, d2 = rhs3 - rhs1;
        double c1 = (d1.x*d1.x+d1.y*d1.y) / 2.0, c2 = (d2.x*d2.x+d2.y*d2.y) / 2.0;
        double d = d1.Cross(d2);
        res.O.x += (c1*d2.y-c2*d1.y) / d;
        res.O.y += (c2*d1.x-c1*d2.x) / d;
        res.R = res.O.Dist(rhs1);
        return res;
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
            if(cmp(t1-t2) != 0 && cmp(t2-1) <= 0 && cmp(t2) >= 0)
                res.push_back(rhs.a + d1*t2);
        }
        return res;
    }
    double SectorArea(const Point &rhs1, const Point &rhs2){
        double theta = rhs1.Arg() - rhs2.Arg();
        while(cmp(theta) <= 0) theta += 2.0 * PI;
        while(cmp(theta - 2.0*PI) >  0) theta -= 2.0 * PI;
        theta = min(theta, 2.0*PI - theta);
        return R * R * theta / 2.0;
    }
    /* called by Area(const Polygon&) */
    double calc(const Point &rhs1, const Point &rhs2){
        vector<Point> p;
        bool in1 = (cmp(rhs1.Abs()-R) < 0);
        bool in2 = (cmp(rhs2.Abs()-R) < 0);
        if(in1){
            if(in2)
                return fabs(rhs1.Cross(rhs2)) / 2.0;
            else{
                p = Intersection(Line(rhs1, rhs2));
                return SectorArea(rhs2, p[0]) + fabs(rhs1.Cross(p[0])) / 2.0;
            }
        }else{
            if(in2){
                p = Intersection(Line(rhs1, rhs2));
                return SectorArea(p[0], rhs1) + fabs(rhs2.Cross(p[0])) / 2.0;
            }else{
                p = Intersection(Line(rhs1, rhs2));
                if((int)p.size() == 2){
                    return SectorArea(rhs1, p[0]) + SectorArea(p[1], rhs2) + fabs(p[0].Cross(p[1])) / 2.0;
                }else{
                    return SectorArea(rhs1, rhs2);
                }
            }
        }
    }
    /*
     * the area of overlap between circle and polygon
     */
    double Area(const Polygon &rhs){
        Polygon that = rhs;
        for(int i=0;i<that.N;i++){
            that.s[i] = that.s[i] - O;
        }
        double res = 0;
        for(int i=0;i<that.N;i++){
            int sng = cmp(that.s[i].Cross(that.s[(i+1)%that.N]));
            if(sng){
                res += sng * calc(that.s[i], that.s[(i+1)%that.N]);
            }
        }
        return res;
    }
    static Circle MinCircleCover(vector<Point> rhs){
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
    /*
     * the intersections of two circle
     */
    pair<Point, Point> Intersection(const Circle &rhs) const {
        double d = (O-rhs.O).Abs();
        double cost = (R*R+d*d-rhs.R*rhs.R) / (2.0*R*d);
        double sint = sqrt(1.0 - cost*cost);
        Point rfn = (rhs.O-O) / d * R;
        return make_pair(O+rfn.Rotate(cost, sint), O+rfn.Rotate(cost, -sint));
    }
    friend ostream& operator << (ostream& out, const Circle &rhs){
        out << "C{" << rhs.O << ", " << rhs.R << "}";
        return out;
    }
    bool operator < (const Circle &rhs) const {
        if(cmp(R-rhs.R) != 0) return cmp(R-rhs.R) < 0;
        return O < rhs.O;
    }
    bool operator == (const Circle &rhs) const {
        return cmp(R-rhs.R) == 0 && O == rhs.O;
    }
};
/*
 * Area that covered by circles in tc
 */
double AreaCombine(vector<Circle> tc){
    vector<Circle> c;
    double res = 0;
    sort(tc.begin(), tc.end());
    tc.erase(unique(tc.begin(), tc.end()), tc.end());
    for(int i=0;i<(int)tc.size();i++){
        bool tf = true;
        for(int j=i+1;j<(int)tc.size();j++){
            if(cmp((tc[i].O-tc[j].O).Abs()-fabs(tc[i].R-tc[j].R)) <= 0){
                tf = false;
                break;
            }
        }
        if(tf) c.push_back(tc[i]);
    }
    typedef pair<pair<double, int>, Point> EV;
#define arg first.first
#define d   first.second
#define p   second
    for(int i=0;i<(int)c.size();i++){
        vector<EV> ev;
        Point boundary = c[i].O + Point(c[i].R, 0);
        ev.push_back(EV(make_pair(0, 0), boundary));
        ev.push_back(EV(make_pair(2.0*PI, 0), boundary));
        for(int j=0;j<(int)c.size();j++)if(i != j){
            if(cmp((c[i].O-c[j].O).Abs()-(c[i].R+c[j].R)) < 0){
                pair<Point, Point> crs = c[i].Intersection(c[j]);
                double x = (crs.first-c[i].O).Arg();
                double y = (crs.second-c[i].O).Arg();
                if(cmp(x-y) < 0){
                    ev.push_back(EV(make_pair(0, -1), boundary));
                    ev.push_back(EV(make_pair(2*PI, 1), boundary));
                }                
                ev.push_back(EV(make_pair(x, 1), crs.first));
                ev.push_back(EV(make_pair(y, -1), crs.second));
            }
        }
        sort(ev.begin(), ev.end());
        int sum = ev[0].d;
        for(int j=1;j<(int)ev.size();j++){
            if(sum == 0){
                res += ev[j-1].p.Cross(ev[j].p) / 2.0;
                double ta = c[i].R * c[i].R * (ev[j].arg-ev[j-1].arg) / 2.0;
                ta -= (ev[j-1].p-c[i].O).Cross(ev[j].p-c[i].O) / 2.0;
                res += ta;
            }
            sum += ev[j].d;
        }
    }
    return res;
#undef arg
#undef d
#undef p
}

int main(){
    int N;
    scanf("%d", &N);
    vector<Circle> c;
    for(int i=0;i<N;i++){
        double x, y, r;
        scanf("%lf %lf %lf", &x, &y, &r);
        c.push_back({{x, y}, r});
    }
    printf("%.3f\n", AreaCombine(c));

    return 0;
}
