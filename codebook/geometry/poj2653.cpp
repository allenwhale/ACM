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
Line l[100010];
int main(){
    int N;
    while(~scanf("%d", &N) && N){
        vector<Line> vc;
        vector<int> ans;
        for(int i=0;i<N;i++){
            scanf("%lf %lf %lf %lf", &l[i].a.x, &l[i].a.y, &l[i].b.x, &l[i].b.y);
        }
        for(int i=0;i<N;i++){
            bool tf = false;
            for(int j=i+1;j<N&&!tf;j++){
                tf = l[j].isintersect(l[i]);
            }
            if(tf == false)
                ans.push_back(i+1);
        }
        printf("Top sticks:");
        for(int i=0;i<(int)ans.size();i++)
            printf(" %d%c", ans[i], i==((int)ans.size()-1)?'.':',');
        puts("");
    }
    return 0;
}
