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
