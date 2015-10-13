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
#define INSIDE  1
#define ONEDGE  2
#define OUTSIDE 0
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
    bool IsIntersect(const Line &rhs){
        int i = (upper_bound(A.begin(), A.end(), (rhs.b-rhs.a).Arg()) - A.begin()) % N;
        int j = (upper_bound(A.begin(), A.end(), (rhs.a-rhs.b).Arg()) - A.begin()) % N;
        if(cmp((rhs.b-rhs.a).Cross(s[i]-rhs.a)*(rhs.b-rhs.a).Cross(s[j]-rhs.a)) <= 0)
            return true;
        return false;
    }
};
