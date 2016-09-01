/*
 * Mixed Euler
 * a graph contains directed & undirected edges
 * Solve() returns if there is a euler circuit or not
 */
struct MEuler{
	struct Edge{
#define DIRECTED    1
#define UNDIRECTED  0
			int to, dir;
	};
	int N;
	Dinic dinic;
	vector<int> deg;
	vector<vector<Edge> > vc;

	MEuler(int n=0): N(n), dinic(Dinic(N + 2)), deg(vector<int>(N, 0)), vc(vector<vector<Edge> >(N)) {}
	void add_edge(int a, int b, int d){
		vc[a].push_back({b, d});
		deg[a]++, deg[b]--;
	}
	bool Solve(){
		for(int i=0;i<N;i++)
			if(abs(deg[i]) & 1) return false;
		for(int i=0;i<N;i++)
            for(Edge e:vc[i]){
				if(e.dir == UNDIRECTED)
					dinic.add_edge(i, e.to, 1);
			}
		int ans = 0;
		for(int i=0;i<N;i++){
			if(deg[i] > 0){
				dinic.add_edge(N, i, deg[i] / 2);
			}else if(deg[i] < 0){
				dinic.add_edge(i, N + 1, -deg[i] / 2);
				ans += -deg[i] / 2;
			}
		}
		if(dinic.Solve(N, N + 1) < ans) return false;
		return true;
	}
};
