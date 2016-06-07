/*
 * Restricted MST
 * r = the node is limited
 * k = the limit
 * notice: <=k or ==k
 * Solve() returns value of rmst if there ia an answer else -1
 */
struct RMST{
#define to  first.first
#define eid first.second
#define v1  first.first
#define v2  first.second
#define w   second
	const static int INF = 0x3f3f3f3f;
	typedef pair<int, int> PI;
	typedef pair<PI, int> PII;
	int N;
	vector<vector<PII> > vc;
	vector<PII> E;
	DisjointSet djs;
	vector<bool> choose;
	vector<int> best;
	vector<PI> adj;
	RMST(int n=0): N(n), vc(vector<vector<PII> >(N+1)), djs(DisjointSet(N)) {}
	void add_edge(int a, int b, int w){
		E.push_back(PII(PI(a, b), w));
	}
	static bool cmp(PII a, PII b){
		return a.w < b.w;
	}
	void dfs(int x, int p, int r){
		for(int i=0;i<(int)vc[x].size();i++){
			PII e = vc[x][i];
			if(choose[e.eid] && e.to != p){
				if(x == r){
					best[e.to] = -1;
				}else{
					if(best[x] == -1 || E[best[x]].w < e.w){
						best[e.to] = e.eid;
					}else{
						best[e.to] = best[x];
					}
				}
				dfs(e.to, x, r);
			}
		}
	}
	int Solve(int r, int k){
		choose = vector<bool>((int)E.size()+1, false);
		best = vector<int>(N+1, -1);
		adj = vector<PI>(N+1, PI(INF, -1));
		sort(E.begin(), E.end(), RMST::cmp);
		int rmst = 0, m = 0;
		for(int i=0;i<(int)E.size();i++){
			PII e = E[i];
			vc[e.v1].push_back(PII(PI(e.v2, i), e.w));
			vc[e.v2].push_back(PII(PI(e.v1, i), e.w));
			if(e.v1 != r && e.v2 != r && djs.find(e.v1) != djs.find(e.v2)){
				choose[i] = true;
				djs.U(e.v1, e.v2);
				rmst += e.w;
			}
		}
		for(int i=0;i<(int)E.size();i++){
			PII e = E[i];
			if(e.v1 == r || e.v2 == r){
				int v = (e.v1 == r ? e.v2 : e.v1);
				adj[v] = min(adj[v], PI(e.w, i));
				if(djs.find(r) != djs.find(v)){
					choose[i] = true;
					rmst += e.w;
					m++;
					djs.U(r, v);
				}
			}
		}
		if(m > k) return -1;
		for(int j=m+1;j<=k;j++){
			fill(best.begin(), best.end(), -1);
			dfs(r, r, r);
			int chid = -1;
			int chmin = INF;
			int vid = -1;
			for(int i=0;i<N;i++){
				if(i != r && adj[i].first != INF && best[i] != -1){
					if(chmin > adj[i].first - E[best[i]].w){
						chmin = adj[i].first - E[best[i]].w;
						chid = adj[i].second;
						vid = i;
					}
				}
			}
			/* if ==k 
			   if(chid == -1) return -1;
			   */
			/* if <=k */
			if(chmin >= 0) break;
			/**/
			choose[best[vid]] = false;
			choose[chid] = true;
			rmst += chmin;
		}
		return rmst;
	}
};
