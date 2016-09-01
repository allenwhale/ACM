/*
 * BBC Vertex
 * solve the biconnected components(BCC)
 * store answer in bcc
 * bbc.size() is the number of BCC
 * bcc[i] is the sequence of a BCC
 */
vector<int> ed[MAXN];
int N, stk[MAXN], top, dfn[MAXN], low[MAXN];
vector<vector<int> > bcc;
void _BBC(int x, int d){
    stk[++top] = x;
    dfn[x] = low[x] = d;
    for(int to:ed[x]){
        if(dfn[to] == -1){
            _BBC(to, d + 1);
            if(low[to] >= dfn[x]){
                vector<int> l;
                do{
                    l.push_back(stk[top]);
                    top--;
                }while(stk[top + 1] != to);
                l.push_back(x);
                bcc.push_back(l);
            }
            low[x] = min(low[x], low[to]);
        }else low[x] = min(low[x], dfn[to]);
    }
}
void BCC(){
    memset(dfn, -1, sizeof(dfn));
    memset(low, -1, sizeof(low));
    bcc.clear();
    top = -1;
    for(int i=0;i<N;i++)
        if(dfn[i] == -1)
            _BBC(i, 0);
}
