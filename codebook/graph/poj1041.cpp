#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <string.h>
#include <queue>
#include <map>
using namespace std;
typedef pair<int, int> PI;
#define FF first
#define SS second
class DisjointSet {
public:
	int N;
	vector<int> p;
	DisjointSet(int n): N(n), p(vector<int>(N)){
		for(int i=0;i<N;i++)
			p[i] = i;
	}
	int find(int x){
		return p[x] == x ? x : p[x] = find(p[x]);
	}
	void U(int a, int b){
		p[find(a)] = find(b);
	}
};
class Edge {
public:
	int to, w;
	Edge(int t=0, int _w=0): to(t), w(_w) {}
	bool operator < (const Edge &rhs) const {
		return w < rhs.w;
	}
};
class Graph {
public:
    int N, M;
    vector<vector<Edge> > vc;
    Graph(int n=0): N(n), M(0), vc(vector<vector<Edge> >(N)){}
    vector<int> vis;
	/* for dijkstra */
	vector<int> dis;
	/* for euler circuit*/
	vector<int>eulercircuit;

    void add_edge(int f, Edge e){
        vc[f].push_back(e);
        M++;
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
				Edge e = vc[v.SS][i];
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
				Edge e = vc[v.SS][i];
				if(dis[e.to] > dis[v.SS] + e.w){
					dis[e.to] = dis[v.SS] + e.w;
					pq.push(PI(dis[e.to], e.to));
				}
			}
		}
	}
	int KthShortestPath(int s, int t, int k){
		Graph RG(N);
		for(int i=0;i<N;i++)
			for(int j=0;j<(int)vc[i].size();j++){
				Edge e = vc[i][j];
				RG.add_edge(e.to, Edge(i, e.w));
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
				Edge e = vc[v.SS][i];
				pq.push(PI(-(real+e.w+dis[e.to]), e.to));
			}
		}
		return -1;
	}
	bool Connected(){
		DisjointSet ds(N);
		for(int i=0;i<N;i++)
			for(int j=0;j<(int)vc[i].size();j++)
				ds.U(i, vc[i][j].to);
		for(int i=0;i<N-1;i++)
			if(ds.find(i) != ds.find(i+1))
				return false;
		return true;
	}
#define eid w
	void _EulerCircuit(int x){
		for(int i=0;i<(int)vc[x].size();i++){
			Edge e = vc[x][i];
			if(vis[e.eid]) continue;
			vis[e.eid] = 1;
			_EulerCircuit(e.to);
			eulercircuit.push_back(e.eid);
		}
	}
#undef eid
	bool EulerCircuit(){ // undirected
		if(!Connected()) return false;
		vis = vector<int>(M+1, 0);
		for(int i=0;i<N;i++){
			if(vc[i].size()&1)
				return false;
			//sort
			sort(vc[i].begin(), vc[i].end());
		}
		eulercircuit.clear();
		_EulerCircuit(0);
		//sort
		reverse(eulercircuit.begin(), eulercircuit.end());
		return true;
	}
};
map<int, int> mp;
int sn;
int find(int x){
	if(mp.find(x) == mp.end())
		mp[x] = sn++;
	return mp[x];
}
vector<pair<PI, int> >e;
int main(){
	int x, y;
	int flag = 0;
	while(~scanf("%d %d", &x, &y)){
		if(x == 0 && y == 0){
			if(flag == 1) break;
			flag = 1;
			Graph G(sn);
			for(int i=0;i<(int)e.size();i++){
				pair<PI, int> ee = e[i];
				G.add_edge(ee.FF.FF, Edge(ee.FF.SS, ee.SS));
				G.add_edge(ee.FF.SS, Edge(ee.FF.FF, ee.SS));
			}
			if(G.EulerCircuit() == false)puts("Round trip does not exist.");
			else{
				for(int i=0;i<(int)G.eulercircuit.size();i++){
					printf("%d%c", G.eulercircuit[i], i==G.eulercircuit.size()-1?'\n':' ');
				}
			}
			mp.clear();
			sn = 0;
			e.clear();
			continue;
		}
		int id;
		scanf("%d", &id);
		x = find(x); y = find(y);
		e.push_back(pair<PI, int>(PI(x, y), id));
		flag = 0;
	}

	return 0;
}
