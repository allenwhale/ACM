#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <string.h>
#include <queue>
using namespace std;
class Edge {
public:
    int to;
    Edge(int t=0): to(t){}
};

template<class E>
class Graph {
public:
    int N, M;
    vector<vector<E> > vc;
    Graph(int n): N(n), M(0), vc(vector<vector<E> >(N)){}
    vector<int> vis;
    /* for cut bridge */
    vector<bool> cut;
    vector<vector<bool> > bridge;
    vector<int> dfn, low;
    /* for BBC */
    //vector<int> dfn, low;
    vector<vector<int> > bcc;
    vector<int> bcc_seq;
    vector<int>stk;
    int top;
    /* for SCC */
    //vector<int> dfn, low;
    //vector<int>stk;
    //int top;
    vector<int> scc;
    int scc_cnt;
    /* for toposort */
    vector<int> toposort;

    void add_edge(int f, E e){
        vc[f].push_back(e);
        M++;
    }
    int Node(){
        return N;
    }
    int Edge(){
        return M;
    }
    void _cut_bridge(int x, int f, int d){
        vis[x] = 1;
        dfn[x] = low[x] = d;
        int children = 0;
        for(int i=0;i<(int)vc[x].size();i++){
            E e = vc[x][i];
            if(e.to != f && vis[e.to] == 1)
                low[x] = min(low[x], dfn[e.to]);
            if(vis[e.to] == 0){
                _cut_bridge(e.to, x, d+1);
                children++;
                low[x] = min(low[x], low[e.to]);
                if((f == -1 && children > 1) || (f != -1 && low[e.to] >= dfn[x]))
                    cut[x] = true;
                if(low[e.to] > dfn[x])
                    bridge[x][e.to] = bridge[e.to][x] = true;

            }
        }
    }
    void cut_bridge(){
        vis = vector<int>(N+1, 0);
        dfn = low = vector<int>(N+1);
        cut = vector<bool>(N+1);
        bridge = vector<vector<bool> >(N+1, vector<bool>(N+1, false));
        for(int i=0;i<N;i++){
            if(!vis[i])
                _cut_bridge(i, -1, 0);
        }
    }

    void _BCC(int x, int f, int d){
        stk[++top] = x;
        dfn[x] = low[x] = d;
        for(int i=0;i<(int)vc[x].size();i++){
            E e = vc[x][i];
            if(e.to == f) continue;
            if(dfn[e.to] == -1){
                _BCC(e.to, x, d+1);
                low[x] = min(low[x], low[e.to]);
                if(low[e.to] >= dfn[x]){
                    vector<int> l;
                    do{
                        bcc_seq[stk[top]] = bcc.size();
                        l.push_back(stk[top]);
                        top--;
                    }while(stk[top+1] != e.to);
                    bcc_seq[x] = bcc.size();
                    l.push_back(x);
                    bcc.push_back(l);
                }
            }else low[x] = min(low[x], dfn[e.to]);
        }
    }

    void BCC(){
        dfn = low = vector<int>(N+1, -1);
        bcc = vector<vector<int> >();
        bcc_seq = vector<int>(N+1);
        stk = vector<int>(N+1, -1);
        top = -1;
        for(int i=0;i<N;i++)
            if(dfn[i] == -1)
                _BCC(i, -1, 0);
    }
    
    void _SCC(int x, int d){
        stk[++top] = x;
        dfn[x] = low[x] = d;
        vis[x] = 1;
        for(int i=0;i<(int)vc[x].size();i++){
            E e = vc[x][i];
            if(dfn[e.to] != -1){
                if(vis[e.to] == 1)
                    low[x] = min(low[x], dfn[e.to]);
            }else{
                _SCC(e.to, d+1);
                low[x] = min(low[x], low[e.to]);
            }
        }
        if(low[x] == dfn[x]){
            while(stk[top] != x){
                scc[stk[top]] = scc_cnt;
                vis[stk[top]] = 2;
                top--;
            }
            scc[stk[top]] = scc_cnt++;
            vis[stk[top]] = 2;
            top--;
        }
    }
    void SCC(){
        dfn = low = vector<int>(N+1, -1);
        vis = vector<int>(N+1, 0);
        scc = vector<int>(N+1, 0);
        scc_cnt = 1;
        stk = vector<int>(N+1, -1);
        top = -1;
        for(int i=0;i<N;i++)
            if(dfn[i] == -1)
                _SCC(i, 0);
    }

    void Toposort(){
        toposort = vector<int>();
        vector<int> in_deg(N+1, 0);
        queue<int> q;
        for(int i=0;i<N;i++)
            for(int j=0;j<(int)vc[i].size();j++)
                in_deg[vc[i][j].to]++;
        for(int i=0;i<N;i++)
            if(in_deg[i] == 0)
                q.push(i);
        while(!q.empty()){
            int v = q.front();
            q.pop();
            toposort.push_back(v);
            for(int i=0;i<(int)vc[v].size();i++){
                E e = vc[v][i];
                in_deg[e.to]--;
                if(in_deg[e.to] == 0)
                    q.push(e.to);
            }
        }
    }
};
void add_edge(Graph<Edge> &g, int a, int b){
    g.add_edge(a, Edge(b));
    g.add_edge(b, Edge(a));
}
bool odd_cycle(Graph<Edge> &G, vector<int> &bcc){
    vector<bool> in(G.N+1, false);
    vector<int> color(G.N+1, -1);
    for(int i=0;i<(int)bcc.size();i++)
        in[bcc[i]] = true;
    queue<int> q;
    q.push(bcc[0]);
    color[bcc[0]] = 1;
    while(!q.empty()){
        int t = q.front();
        q.pop();
        for(int i=0;i<(int)G.vc[t].size();i++){
            Edge e = G.vc[t][i];
            if(in[e.to] == false) continue;
            if(color[e.to] != -1 && color[e.to] == color[t])
                return false;
            if(color[e.to] == -1){
                color[e.to] = color[t] ^ 1;
                q.push(e.to);
            }
        }
    }
    return true;
}
int RG[1010][1010];
int main(){
    int N, M;
    while(~scanf("%d %d", &N, &M) && N && M){
        vector<bool> odd(N+1, false);
        memset(RG, true, sizeof(RG));
        Graph<Edge> G(N);
        for(int i=0;i<M;i++){
            int a, b;
            scanf("%d %d", &a, &b);
            a--, b--;
            RG[a][b] = RG[b][a] = false;
        }
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                if(i != j && RG[i][j])
                    G.add_edge(i, Edge(j));
        G.BCC();
        int ans = 0;
        for(int i=0;i<(int)G.bcc.size();i++){
            vector<int> vc = G.bcc[i];
            if((int)vc.size()>=3&&!odd_cycle(G, vc)){
                for(int j=0;j<(int)vc.size();j++)
                    odd[vc[j]] = true;
            }
        }
        for(int i=0;i<N;i++)
            ans += (!odd[i]);
        printf("%d\n", ans);
    }
    return 0;
}
