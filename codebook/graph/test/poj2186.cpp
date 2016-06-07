#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <string.h>
#include <queue>
using namespace std;
#define MAXN 10010
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
int main(){
    int M;
    scanf("%d %d", &N, &M);
    for(int i=0;i<M;i++){
        int a, b;
        scanf("%d %d", &a, &b);
        a--, b--;
        ed[a].push_back(b);
    }
    SCC();
    vector<int> out(scc_cnt+1, 0);
    for(int i=0;i<N;i++)
        for(int j=0;j<(int)ed[i].size();j++)
            if(scc[i] != scc[ed[i][j]])
                out[scc[i]]++;
    int r = -1;
    for(int i=0;i<scc_cnt;i++){
        if(out[i] == 0){
            if(r == -1)
                r = i;
            else{
                r = -1;
                break;
            }
        }
    }
    int ans = 0;
    for(int i=0;i<N;i++)
        if(scc[i] == r)
            ans++;
    printf("%d\n", ans);

    return 0;
}
