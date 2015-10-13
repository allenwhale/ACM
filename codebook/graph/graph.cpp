#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <string.h>
#include <queue>
using namespace std;
typedef pair<int, int> PI;
#define FF first
#define SS second
class Edge {
public:
    int to;
    Edge(int t=0): to(t){}
};

class Graph {
public:
    /* Node num = N, Edge num = M*/
    int N, M;
    vector<vector<Edge> > vc;
    Graph(int n=0): N(n), M(0), vc(vector<vector<Edge> >(N)){}
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
    /* for 2sat */
    vector<int> twosatans;

    void add_edge(int f, Edge e){
        vc[f].push_back(e);
        M++;
    }

    /* called by cut_bridge */
    void _cut_bridge(int x, int f, int d){
        vis[x] = 1;
        dfn[x] = low[x] = d;
        int children = 0;
        for(int i=0;i<(int)vc[x].size();i++){
            Edge e = vc[x][i];
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
    /*
     * solve the cut and bridge
     * store answer in cut(vector<bool>) ans bridge(vector<vector<bool> >) 
     * cut[i] == true iff i-th node is cut
     * bridge[i][j] == true iff edge between i-th ans j-th is bridge
     */
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

    /* called by BCC */
    void _BBC(int x, int d){
        stk[++top] = x;
        dfn[x] = low[x] = d;
        for(int i=0;i<(int)vc[x].size();i++){
            Edge e = vc[x][i];
            if(dfn[e.to] == -1){
                _BBC(e.to, d+1);
                if(low[e.to] >= dfn[x]){
                    vector<int> l;
                    do{
                        l.push_back(stk[top]);
                        top--;
                    }while(stk[top+1] != e.to);
                    l.push_back(x);
                    bcc.push_back(l);
                }
                low[x] = min(low[x], low[e.to]);
            }else low[x] = min(low[x], dfn[e.to]);
        }
    }
    /*
     * solve the biconnected components(BCC)
     * store answer in bcc(vector<vector<int> >)
     * bbc.size() is the number of BCC
     * bcc[i] is the sequence of a BCC
     */
    void BCC(){
        dfn = low = vector<int>(N+1, -1);
        bcc = vector<vector<int> >();
        stk = vector<int>(N+1, -1);
        top = -1;
        for(int i=0;i<N;i++)
            if(dfn[i] == -1)
                _BBC(i, 0);
    }

    /* called by SCC */
    void _SCC(int x, int d){
        stk[++top] = x;
        dfn[x] = low[x] = d;
        vis[x] = 1;
        for(int i=0;i<(int)vc[x].size();i++){
            Edge e = vc[x][i];
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
    /*
     * solve the strongly connected component(SCC)
     * store answer in scc(vector<int>)
     * the value of scc[i] means the id of the SCC which i-th node in (id is based 0)
     * scc_cnt id the number of SCC
     */
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

    /*
     * generate a toposort of graph
     * store in toposort
     */
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
                Edge e = vc[v][i];
                in_deg[e.to]--;
                if(in_deg[e.to] == 0)
                    q.push(e.to);
            }
        }
    }

    /* 
     * called by TwoSat 
     * get the value of i-th
     * 1 = true, 0 = false, -1 = undefined
     */
    int TwoSatGet(int x){
        int r = x > N/2 ? x-N/2 : x;
        if(twosatans[r] == -1)
            return -1;
        return x > N/2 ? !twosatans[r] : twosatans[r];
    }
    /*
     * solve the 2SAT
     * return true if there exists a set of answer
     * store the answer in twosatans
     */
    bool TwoSat(){
        SCC();
        twosatans = vector<int>(N/2+1, -1);
        for(int i=0;i<N/2;i++)
            if(scc[i] == scc[i+N/2])
                return false;
        vector<vector<int> > c(scc_cnt+1);
        for(int i=0;i<N;i++)
            c[scc[i]].push_back(i);
        for(int i=0;i<scc_cnt;i++){
            int val = 1;    
            for(int j=0;j<(int)c[i].size();j++){
                int x = c[i][j];
                if(TwoSatGet(x) == 0)
                    val = 0;
                for(int k=0;k<(int)vc[x].size();k++)
                    if(TwoSatGet(vc[x][k].to) == 0)
                        val = 0;
                if(!val)
                    break;
            }
            for(int j=0;j<(int)c[i].size();j++){
                if(c[i][j] > N/2)
                    twosatans[c[i][j]-N/2] = !val;
                else
                    twosatans[c[i][j]] = val;
            }
        }
        return true;
    }
};

int main(){
    return 0;
}
