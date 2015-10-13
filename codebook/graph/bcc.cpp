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
