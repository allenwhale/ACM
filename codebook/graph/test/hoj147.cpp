#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <string.h>
#include <queue>
using namespace std;
#define MAXN 10010
/* 
 * called by TwoSat 
 * get the value of i-th
 * 1 = true, 0 = false, -1 = undefined
 */
vector<int> ed[MAXN];
int N, stk[MAXN], top, dfn[MAXN], low[MAXN], scc[MAXN], vis[MAXN], scc_cnt;
void _SCC(int x, int d){
	stk[++top] = x;
	dfn[x] = low[x] = d;
	vis[x] = 1;
	for(int i=0;i<(int)ed[x].size();i++){
		int to = ed[x][i];
		if(dfn[to] != -1){
			if(vis[to] == 1)
				low[x] = min(low[x], dfn[to]);
		}else{
			_SCC(to, d+1);
			low[x] = min(low[x], low[to]);
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
    memset(dfn, -1, sizeof(dfn));
    memset(low, -1, sizeof(low));
    memset(vis, 0, sizeof(vis));
    memset(scc, -1, sizeof(scc));
    scc_cnt = 0;
    top = -1;
	for(int i=0;i<N;i++)
		if(dfn[i] == -1)
			_SCC(i, 0);
}
int twosatans[MAXN * 4];
int TwoSatGet(int x){
	int r = x > N/2 ? x-N/2 : x;
	if(twosatans[r] == -1)
		return -1;
	return x > N/2 ? !twosatans[r] : twosatans[r];
}
bool TwoSat(){
	SCC();
    memset(twosatans, -1, sizeof(twosatans));
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
			for(int k=0;k<(int)ed[x].size();k++)
				if(TwoSatGet(ed[x][k]) == 0)
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
int main(){
	int N, M;
	scanf("%d %d", &N, &M);
    N *= 4;
	for(int i=1;i<=N;i++){
        ed[2*i-1].push_back(2*i-2+2*N);
        ed[2*i-2].push_back(2*i-1+2*N);
        ed[2*i-1+2*N].push_back(2*i-2);
        ed[2*i-2+2*N].push_back(2*i-1);
		//G.add_edge(2*i-1, Edge(2*i-2+2*N));
		//G.add_edge(2*i-2, Edge(2*i-1+2*N));
		//G.add_edge(2*i-1+2*N, Edge(2*i-2));
		//G.add_edge(2*i-2+2*N, Edge(2*i-1));
	}
	for(int i=0;i<M;i++){
		int a, b;
		scanf("%d %d", &a, &b);
		a--, b--;
        ed[a].push_back(b+2*N);
        ed[b].push_back(a+2*N);
		//G.add_edge(a, Edge(b+2*N));
		//G.add_edge(b, Edge(a+2*N));
	}
	if(TwoSat() == false)puts("-1");
	else{
		for(int i=0;i<N/2;i++){
			if(twosatans[i])
				printf("%d\n", i+1);
		}
	}
    return 0;
}
