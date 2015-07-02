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
class wEdge {
public:
	int to, w;
	wEdge(int t=0, int _w=0): to(t), w(_w) {}
};

class Graph {
public:
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
	/* for dijkstra */
	vector<int> dis;

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
	int TwoSatGet(int x){
		int r = x > N/2 ? x-N/2 : x;
		if(twosatans[r] == -1)
			return -1;
		return x > N/2 ? !twosatans[r] : twosatans[r];
	}
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
	int Dijkstra(int s, int t){
		const int INF = 0x3f3f3f3f;
		dis = vector<int>(N+1, INF);
		priority_queue<PI> pq;
		dis[s] = 0;
		pq.push(PI(0, s));
		while(!pq.empty()){
			PI v = pq.top();
			pq.pop();
			if(v.FF > dis[v.SS])continue;
			if(v.SS == t)return dis[t];
			for(int i=0;i<(int)vc[v.SS].size();i++){
				E e = vc[v.SS][i];
				if(dis[e.to] > dis[v.SS] + e.w){
					dis[e.to] = dis[v.SS] + e.w;
					pq.push(PI(dis[e.to], e.to));
				}
			}
		}
		return -1;
	}
	void AllDijkstra(int s){
		const int INF = 0x3f3f3f3f;
		dis = vector<int>(N+1, INF);
		priority_queue<PI> pq;
		dis[s] = 0;
		pq.push(PI(0, s));
		while(!pq.empty()){
			PI v = pq.top();
			pq.pop();
			if(v.FF > dis[v.SS])continue;
			for(int i=0;i<(int)vc[v.SS].size();i++){
				E e = vc[v.SS][i];
				if(dis[e.to] > dis[v.SS] + e.w){
					dis[e.to] = dis[v.SS] + e.w;
					pq.push(PI(dis[e.to], e.to));
				}
			}
		}
	}
	int KthShortestPath(int s, int t, int k){
		Graph<wEdge> RG(N);
		for(int i=0;i<N;i++)
			for(int j=0;j<(int)vc[i].size();j++){
				E e = vc[i][j];
				RG.add_edge(e.to, wEdge(i, e.w));
			}
		RG.AllDijkstra(t);
		dis = RG.dis;
		priority_queue<PI> pq;
		pq.push(PI(-dis[s], s));
		while(!pq.empty()){
			PI v = pq.top();
			pq.pop();
			int real = -v.FF - dis[v.SS];
			if(v.SS == t && (!(--k)))
				return real;
			for(int i=0;i<(int)vc[v.SS].size();i++){
				E e = vc[v.SS][i];
				pq.push(PI(-(real+e.w+dis[e.to]), e.to));
			}
		}
		return -1;
	}
};

void add_edge(Graph<Edge> &G, int a, int b){
    G.add_edge(a, Edge(b));
    G.add_edge(b, Edge(a));
}
int main(){
	int N, M;
	while(~scanf("%d %d", &N, &M)){
		Graph<wEdge> G(N);
		for(int i=0;i<M;i++){
			int a, b, c;
			scanf("%d %d %d", &a, &b, &c);
			a--, b--;
			G.add_edge(a, wEdge(b, c));
		}
		int s, t, k;
		scanf("%d %d %d", &s, &t, &k);
		s--, t--;
		k += (s==t);
		printf("%d\n", G.KthShortestPath(s, t, k));
	}
    return 0;
}
