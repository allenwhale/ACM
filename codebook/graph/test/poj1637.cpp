#include <stdio.h>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
using namespace std;
/*
 * Maximum Flow Dinic
 * Solve() returns answer
 */
class Dinic{
public:
    class Edge{
    public:
        int v1, v2, f, c;
        Edge(int _v1=0, int _v2=0, int _f=0, int _c=0): v1(_v1), v2(_v2), f(_f), c(_c){}
    };
    int N;
    vector<vector<int> >vc;
    vector<Edge> E;
    vector<int> dep;

    Dinic(int n=0): N(n), vc(vector<vector<int> >(N+1)), dep(vector<int>(N+1)) {}
    void add_edge(int a, int b, int c){
        vc[a].push_back(E.size());
        E.push_back(Edge(a, b, c, c));
        vc[b].push_back(E.size());
        E.push_back(Edge(b, a, 0, c));
    }
    int Bfs(int s, int t){
        fill(dep.begin(), dep.end(), -1);
        dep[s] = 0;
        queue<int> q;
        q.push(s);
        while(!q.empty()){
            int v = q.front(); q.pop();
            for(int i=0;i<(int)vc[v].size();i++){
                Edge e = E[vc[v][i]];
                if(e.f > 0 && dep[e.v2] == -1){
                    dep[e.v2] = dep[v] + 1;
                    q.push(e.v2);
                }
            }
        }
        return dep[t];
    }
    int Dfs(int x, int df, int t){
        if(x == t) return df;
        int res = 0;
        for(int i=0;i<(int)vc[x].size();i++){
            Edge &e = E[vc[x][i]];
            if(e.f > 0 && dep[e.v2] == dep[x] + 1){
                int f = Dfs(e.v2, min(df, e.f), t);
                e.f -= f;
                E[vc[x][i]^1].f += f;
                df -= f;
                res += f;
            }
        }
        return res;
    }
    int Solve(int s, int t){
        int flow = 0;
        while(Bfs(s, t) != -1){
            flow += Dfs(s, 0x3f3f3f3f, t);
        }
        return flow;
    }
};
/*
 * Stoer Wanger
 * Undirected Min Cut
 * Solve() returns answer if graph is connected else 0
 */
class StoerWanger{
public:
    int N, wN;
    vector<vector<int> >G;
    vector<int> bln, dis;
    StoerWanger(int n=0): N(n), G(vector<vector<int> >(N, vector<int>(N))), bln(vector<int>(N, -1)), dis(vector<int>(N)) {}
    void add_edge(int a, int b, int c){
        G[a][b] += c;
        G[b][a] += c;
    }
    int Mst(int r, int &x, int &y){
        int t;
        bln[t=0] = r;
        for(int i=0;i<wN;i++)
            if(bln[i] != r)
                dis[i] = G[0][i];
        for(int k=0;k<wN-1;k++){
            x = t; t = 0;
            for(int i=0;i<wN;i++)
                if(bln[i] != r && (!t || dis[i] > dis[t]))
                    t = i;
            bln[t] = r;
            for(int i=0;i<wN;i++)
                if(bln[i] != r)
                    dis[i] += G[t][i];
        }
        y = t;
        return dis[t];
    }
    void Merge(int x, int y){
        if(x > y) swap(x, y);
        for(int i=0;i<wN;i++)
            if(i != x && i != y)
                G[i][x] += G[i][y], G[x][i] += G[y][i];
        if(y == wN-1) return;
        for(int i=0;i<wN-1;i++)
            if(i != y)
                swap(G[i][y], G[i][wN-1]), swap(G[y][i], G[wN-1][i]);
    }
    int Solve(){
        wN = N;
        int res = 0x3f3f3f3f;
        for(int i=0;wN>1;i++, wN--){
            int x, y;
            res = min(res, Mst(i, x, y));
            Merge(x, y);
        }
        return res;
    }
};
/*
 * Mixed Euler
 */
class MEuler{
public:
    class Edge{
#define DIRECTED    1
#define UNDIRECTED  0
    public:
        int to, dir;
        Edge(int t=0, int d=0): to(t), dir(d){}
    };
    int N;
    Dinic dinic;
    vector<int> deg;
    vector<vector<Edge> > vc;
    
    MEuler(int n=0): N(n), dinic(Dinic(N+2)), deg(vector<int>(N, 0)), vc(vector<vector<Edge> >(N)) {}
    void add_edge(int a, int b, int d){
        vc[a].push_back(Edge(b, d));
        deg[a]++, deg[b]--;
    }
    bool Solve(){
        for(int i=0;i<N;i++)
            if(abs(deg[i])&1) return false;
        for(int i=0;i<N;i++)
            for(int j=0;j<(int)vc[i].size();j++){
                Edge e = vc[i][j];
                if(e.dir == UNDIRECTED)
                    dinic.add_edge(i, e.to, 1);
            }
        int ans = 0;
        for(int i=0;i<N;i++){
            if(deg[i] > 0){
                dinic.add_edge(N, i, deg[i]/2);
            }else if(deg[i] < 0){
                dinic.add_edge(i, N+1, -deg[i]/2);
                ans += -deg[i] / 2;
            }
        }
        if(dinic.Solve(N, N+1) < ans) return false;
        return true;
    }
#undef DIRECTED
#undef UNDIRECTED
};
int main(){
    int T;
    scanf("%d", &T);
    while(T--){
        int N, M;
        scanf("%d %d", &N, &M);
        MEuler G(N);
        for(int i=0;i<M;i++){
            int a, b, d;
            scanf("%d %d %d", &a, &b, &d);
            a--, b--;
            G.add_edge(a, b, d);
        }
        if(G.Solve())puts("possible");
        else puts("impossible");
    }
}
