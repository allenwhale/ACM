#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <string.h>
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
        stk = vector<int>(N+1);
        top = -1;
        for(int i=0;i<N;i++)
            if(dfn[i] == -1)
                _BBC(i, 0);
    }
};
template<class G>
void add_edge(G &g, int a, int b){
    g.add_edge(a, Edge(b));
    g.add_edge(b, Edge(a));
}
int main(){
    Graph<Edge> G(5);
    add_edge(G, 0, 1);
    add_edge(G, 0, 2);
    add_edge(G, 0, 3);
    add_edge(G, 1, 4);
    add_edge(G, 1, 2);
    //add_edge(G, 4, 3);
    G.cut_bridge();

    for(int i=0;i<G.N;i++)
        printf("%d %d\n", i, (int)G.cut[i]);
    for(int i=0;i<G.N;i++)
        for(int j=0;j<G.N;j++)
            if(G.bridge[i][j])
                printf("E(%d, %d) = %d\n", i, j, (int)G.bridge[i][j]);

    G.BCC();
    printf("BCC\n");
    for(auto l: G.bcc){
        for(auto ll: l){
            printf("%d ", ll);
        }
        puts("");
    }

    return 0;
}
