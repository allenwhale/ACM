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

    void _BBC(int x, int d){
        stk[++top] = x;
        dfn[x] = low[x] = d;
        for(int i=0;i<(int)vc[x].size();i++){
            E e = vc[x][i];
            if(dfn[e.to] == -1){
                _BBC(e.to, d+1);
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
                low[x] = min(low[x], low[e.to]);
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
                _BBC(i, 0);
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
        scc_cnt = 0;
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
void add_edge(Graph<Edge> &G, int a, int b){
    G.add_edge(a, Edge(b));
    G.add_edge(b, Edge(a));
}
int N, M;
int AG[30][30];
bool cycle(){
    for(int k=0;k<26;k++)
        for(int i=0;i<26;i++)
            for(int j=0;j<26;j++)
                AG[i][j] |= (AG[i][k] & AG[k][j]);
    for(int i=0;i<26;i++)
        if(AG[i][i])
            return true;
    return false;
}
bool determine(){
    vector<int> deg(30, 0);
    for(int i=0;i<26;i++)
        for(int j=0;j<26;j++)
            if(AG[i][j])
                deg[i]++, deg[j]++;
    for(int i=0;i<N;i++)
        if(deg[i] != N-1)
            return false;
    return true;
}
int main(){
    while(~scanf("%d %d", &N, &M) && N && M){
        memset(AG, 0, sizeof(AG));
        char in[10];
        int t;
        for(t=1;t<=M;t++){
            scanf("%s", in);
            AG[in[0]-'A'][in[2]-'A'] = 1;
            if(cycle()){
                printf("Inconsistency found after %d relations.\n", t);
                goto END;
            }else if(determine()){
                Graph<Edge> G(N);
                for(int i=0;i<26;i++)
                    for(int j=0;j<26;j++)
                        if(AG[i][j])
                            G.add_edge(i, Edge(j));
                G.Toposort();
                string s = "";
                for(int i=0;i<G.N;i++)
                    s += ('A'+G.toposort[i]);
                printf("Sorted sequence determined after %d relations: %s.\n", t, s.c_str());
                goto END;
            }
        }
        printf("Sorted sequence cannot be determined.\n");
END:;
        for(;t<M;t++)
            scanf("%*s");
    }
    return 0;
}
