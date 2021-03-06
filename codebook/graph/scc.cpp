/*
 * solve the strongly connected component(SCC)
 * store answer in scc
 * the value of scc[i] means the id of the SCC which i-th node in (id is based 0)
 * scc_cnt is the number of SCC
 */
vector<int> ed[MAXN];
int N, stk[MAXN], top, dfn[MAXN], low[MAXN], scc[MAXN], vis[MAXN], scc_cnt, tot;
void _SCC(int x){
	stk[++top] = x;
	dfn[x] = low[x] = ++tot;
	vis[x] = 1;
	for(int to:ed[x]){
		if(dfn[to] != -1){
			if(vis[to] == 1)
				low[x] = min(low[x], dfn[to]);
		}else{
			_SCC(to);
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
    tot = 0;
	scc_cnt = 0;
	top = -1;
	for(int i=0;i<N;i++)
		if(dfn[i] == -1)
			_SCC(i);
}
