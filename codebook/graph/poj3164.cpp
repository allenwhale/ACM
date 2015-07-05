#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

class LCA{
public:
    int N;
    vector<vector<int> > vc;
    vector<vector<int> > anc;
    vector<int> dep;
    LCA(int n=1): N(n), vc(vector<vector<int> >(N)) {}

    void add_edge(int a, int b){
        vc[a].push_back(b);
        vc[b].push_back(a);
    }

    void dfs(int x, int p, int d){
        dep[x] = d;
        anc[x][0] = p;
        for(int i=1;i<(int)anc[x].size();i++)
            anc[x][i] = anc[anc[x][i-1]][i-1];
        for(int i=0;i<(int)vc[x].size();i++){
            int v = vc[x][i];
            if(v == p)continue;
            dfs(v, x, d+1);
        }
    }
    void init(int x){
        anc = vector<vector<int> >(N+1, vector<int>((int)log2(N)+1));
        dep = vector<int>(N+1);
        dfs(x, x, 0);
    }
    void up(int &x, int h){
        for(int i=0;h;i++){
            if(h&1)x = anc[x][i];
            h >>= 1;
        }
    }
    int find(int x, int y){
        if(dep[x] > dep[y]) swap(x, y);
        up(y, dep[y]-dep[x]);
        if(x == y) return x;
        for(int i=anc[0].size()-1;i>=0;i--)
            if(anc[x][i] != anc[y][i])
                x = anc[x][i], y = anc[y][i];
        return anc[x][0];
    }
};
class DisjointSet {
public:
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
	void U(int a, int b){
		p[find(b)] = find(a);
	}
};

/*
 * Restricted MST
 * r = the node is limited
 * k = the limit
 * notice: <=k or ==k
 * Solve() returns value of rmst if there ia an answer else -1
 */
class RMST{
public:
#define to  first.first
#define eid first.second
#define v1  first.first
#define v2  first.second
#define w   second
    const static int INF = 0x3f3f3f3f;
    typedef pair<int, int> PI;
    typedef pair<PI, int> PII;
    int N;
    vector<vector<PII> > vc;
    vector<PII> E;
    DisjointSet djs;
    vector<bool> choose;
    vector<int> best;
    vector<PI> adj;
    RMST(int n=0): N(n), vc(vector<vector<PII> >(N+1)), djs(DisjointSet(N)) {}
    void add_edge(int a, int b, int w){
        E.push_back(PII(PI(a, b), w));
    }
    static bool cmp(PII a, PII b){
        return a.w < b.w;
    }
    void dfs(int x, int p, int r){
        for(int i=0;i<(int)vc[x].size();i++){
            PII e = vc[x][i];
            if(choose[e.eid] && e.to != p){
                if(x == r){
                    best[e.to] = -1;
                }else{
                    if(best[x] == -1 || E[best[x]].w < e.w){
                        best[e.to] = e.eid;
                    }else{
                        best[e.to] = best[x];
                    }
                }
                dfs(e.to, x, r);
            }
        }
    }
    int Solve(int r, int k){
        choose = vector<bool>((int)E.size()+1, false);
        best = vector<int>(N+1, -1);
        adj = vector<PI>(N+1, PI(INF, -1));
        sort(E.begin(), E.end(), RMST::cmp);
        int rmst = 0, m = 0;
        for(int i=0;i<(int)E.size();i++){
            PII e = E[i];
            vc[e.v1].push_back(PII(PI(e.v2, i), e.w));
            vc[e.v2].push_back(PII(PI(e.v1, i), e.w));
            if(e.v1 != r && e.v2 != r && djs.find(e.v1) != djs.find(e.v2)){
                choose[i] = true;
                djs.U(e.v1, e.v2);
                rmst += e.w;
            }
        }
        for(int i=0;i<(int)E.size();i++){
            PII e = E[i];
            if(e.v1 == r || e.v2 == r){
                int v = (e.v1 == r ? e.v2 : e.v1);
                adj[v] = min(adj[v], PI(e.w, i));
                if(djs.find(r) != djs.find(v)){
                    choose[i] = true;
                    rmst += e.w;
                    m++;
                    djs.U(r, v);
                }
            }
        }
        if(m > k) return -1;
        for(int j=m+1;j<=k;j++){
            fill(best.begin(), best.end(), -1);
            dfs(r, r, r);
            int chid = -1;
            int chmin = INF;
            int vid = -1;
            for(int i=0;i<N;i++){
                if(i != r && adj[i].first != INF && best[i] != -1){
                    if(chmin > adj[i].first - E[best[i]].w){
                        chmin = adj[i].first - E[best[i]].w;
                        chid = adj[i].second;
                        vid = i;
                    }
                }
            }
            /* if ==k 
            if(chid == -1) return -1;
            */
            /* if <=k */
            if(chmin >= 0) break;
            /**/
            choose[best[vid]] = false;
            choose[chid] = true;
            rmst += chmin;
        }
        return rmst;
    }
#undef to
#undef eid
#undef v1
#undef v2
#undef w
};
/*
 * Minimun Directed Spanning Tree
 */
class MDST{
public:
#define v1  first.first
#define v2  first.second
#define w   second
#define INF 1e9
    typedef pair<int, int> PI;
    typedef pair<PI, double> PII;
    int N;
    vector<PII> E;
    MDST(int n=0): N(n){}
    void add_edge(int a, int b, double w){
        E.push_back(PII(PI(a, b), w));
    }
    double Solve(int r){
        vector<bool> mrg(N+1, false);
        vector<double>  dis(N+1, 0);
        vector<int>  vis(N+1, 0);
        vector<int>  pre(N+1, 0);
        vector<int>  bln(N+1, 0);
        double allw = 0, tmpw = 0;
        while(true){
            tmpw = 0;
            fill(dis.begin(), dis.end(), INF);
            fill(vis.begin(), vis.end(), -1);
            fill(bln.begin(), bln.end(), -1);
            fill(pre.begin(), pre.end(), -1);
            for(int i=0;i<(int)E.size();i++){
                PII e = E[i];
                if(e.v1 != e.v2 && e.v2 != r && e.w < dis[e.v2])
                    dis[e.v2] = e.w, pre[e.v2] = e.v1;
            }
            bool tf = false;
            for(int i=0;i<N;i++){
                if(mrg[i]) continue;
                if(pre[i] == -1 && i != r) return INF;
                if(pre[i] != -1) tmpw += dis[i];
                int s;
                for(s=i;s!=-1&&vis[s]==-1;s=pre[s])
                    vis[s] = i;
                if(s != -1 && vis[s] == i){
                    tf = true;
                    int j = s;
                    do{
                        bln[j] = s;
                        mrg[j] = true;
                        allw += dis[j];
                        j = pre[j];
                    }while(j != s);
                    mrg[s] = false;
                }
            }
            if(tf == false) break;
            for(int i=0;i<(int)E.size();i++){
                PII &e = E[i];
                if(bln[e.v2] != -1) e.w -= dis[e.v2];
                if(bln[e.v1] != -1) e.v1 = bln[e.v1];
                if(bln[e.v2] != -1) e.v2 = bln[e.v2];
                if(e.v1 == e.v2) {
                    e = E.back();
                    E.pop_back();
                    i--;
                }
            }
        }
        return allw + tmpw;
    }
};
typedef pair<double, double> PD;
double dist(PD a, PD b){
    return sqrt((a.first-b.first)*(a.first-b.first) + (a.second-b.second)*(a.second-b.second));
}
PD node[110];

int main(){
    int N, M;
    while(~scanf("%d %d", &N, &M)){
        MDST mdst(N);
        for(int i=0;i<N;i++)
            scanf("%lf %lf", &node[i].first, &node[i].second);
        for(int i=0;i<M;i++){
            int a, b;
            scanf("%d %d", &a, &b);
            a--, b--;
            mdst.add_edge(a,b, dist(node[a], node[b]));
        }
        double ans = mdst.Solve(0);
        if(ans == INF)
            puts("poor snoopy");
        else
            printf("%.2f\n", ans);
    }
    return 0;
}
