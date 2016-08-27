#include <bits/stdc++.h>
using namespace std;
struct DisjointSet {
	int N;
	vector<int> p;
	DisjointSet(int n): N(n), p(vector<int>(N)){
        init();
	}
    void init(){
		for(int i=0;i<N;i++)
			p[i] = i;
    }
	int find(int x){
		return p[x] == x ? x : p[x] = find(p[x]);
	}
	void merge(int a, int b){
		p[find(b)] = find(a);
	}
};
struct Bit{
#define val first
#define pos second
#define lb(x) ((x)&(-(x)))
    typedef pair<int, int> T;
    int N;
    vector<T> bit;
    Bit(int _N=0): N(_N), bit(vector<T>(_N+1)) {}
    void init(int _N){
        N = _N;
        for(int i=0;i<=N;i++)
            bit[i] = {INT_MAX, -1};
    }
    void update(int x, const T &b){
        for(;x;x-=lb(x))
            if(bit[x].val > b.val)
                bit[x] = b;
            //bit[x] = min(bit[x], b);
    }
    int query(int x){
        T res = {INT_MAX, -1};
        for(;x<=N;x+=lb(x))
            if(res.val > bit[x].val)
                res = bit[x];
            //res = min(res, bit[x]);
        return res.pos;
    }
};
struct Manhattan_MST{
#define x first.first
#define y first.second
#define id second
#define dist(a, b) (abs(a.x-b.x)+abs(a.y-b.y))
#define v1 second.first
#define v2 second.second
#define w first
    typedef pair<pair<int, int>, int> Point;
    typedef pair<int, pair<int, int>> Edge;
    vector<Point> points;
    vector<Edge> edges;
    void add_point(const Point &p){
        points.push_back(p);
    }

    void add_edge(const Edge &e){
        edges.push_back(e);
    }

    void find_edges(){
        edges.clear();
        Bit bit(points.size());
        for(int dir=0;dir<4;dir++){
            if(dir == 1 || dir == 3){
                for(int i=0;i<(int)points.size();i++)
                    swap(points[i].x, points[i].y);
            } else if(dir == 2){
                for(int i=0;i<(int)points.size();i++)
                    points[i].x = -points[i].x;
            }
            sort(points.begin(), points.end());
            vector<int> hs(points.size()), T(points.size());
            for(int i=0;i<(int)points.size();i++)
                T[i] = hs[i] = points[i].y - points[i].x;
            sort(hs.begin(), hs.end());
            hs.erase(unique(hs.begin(), hs.end()), hs.end());
            bit.init(hs.size());
            for(int i=(int)points.size()-1;i>=0;i--){
                int idx = lower_bound(hs.begin(), hs.end(), T[i]) - hs.begin() + 1;
                int m = bit.query(idx); 
                if(m != -1){
                    add_edge({dist(points[i], points[m]), {points[i].id, points[m].id}});
                }
                bit.update(idx, {points[i].x+points[i].y, i});
            }
        }
    }

    int MST(){
        DisjointSet d(points.size());        
        int ans = 0;
        sort(edges.begin(), edges.end());
        int cnt = points.size() - 1;
        for(auto &edge: edges){
            if(d.find(edge.v1) != d.find(edge.v2)){
                ans += edge.w;
                d.merge(edge.v1, edge.v2);
                if(!--cnt)break;
            }
        }
        return ans;
    }
    int Solve(){
        find_edges();
        return MST();
    }

};
int main(){
    int t = 1;
    int N;
    while(~scanf("%d", &N) && N){
        Manhattan_MST mst;
        while(N--){
            int a, b;
            scanf("%d%d", &a, &b);
            mst.add_point({N, {a, b}});
        }
        printf("Case %d: Total Weight = %d\n", t++, mst.Solve());
    }
    return 0;
}
