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
