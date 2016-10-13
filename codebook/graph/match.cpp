#include <stdio.h>
#include <vector>
#include <string.h>
#include <algorithm>
#include <queue>
using namespace std;

/*
 * BIpartite Matching
 * Nx = number of x nodes
 * Ny = number of y nodes
 * store matching answer in mx, my
 * Solve() returns the number of matching
 */
class BiMatch{
	public:
		int Nx, Ny;
		vector<vector<int> > vc;
		vector<int> mx, my;
		vector<int> visy;

		BiMatch(int _x=0, int _y=0): Nx(_x), Ny(_y), vc(vector<vector<int> >(Nx+1)){}

		void add(int x, int y){
			vc[x].push_back(y);
		}

		bool Match(int x){
			for(int i=0;i<(int)vc[x].size();i++){
				int y = vc[x][i];
				if(!visy[y]){
					visy[y] = 1;
					if(my[y] == -1 || Match(my[y])){
						mx[x] = y, my[y] = x;
						return true;
					}
				}
			}
			return false;
		}
		int Solve(){
			mx = vector<int>(Nx+1, -1);
			my = vector<int>(Ny+1, -1);
			int ans = 0;
			for(int i=0;i<Nx;i++){
				visy = vector<int>(Ny+1, 0);
				ans += Match(i);
			}
			return ans;
		}
};

/*
 * solve Maximun Bipartite Matching
 * store matching answer in mx ,my
 * Solve() returns themaximun weight of perfect matching
 */
class KM{
	public:
#define FF first
#define SS second
		typedef pair<int, int> PI;
		const static int INF = 0x3f3f3f3f;
		int Nx, Ny;
		vector<vector<int> >mp;
		vector<int> visx, visy;
		vector<int> lx, ly, slack;
		vector<int> mx, my;
		KM(int x=0, int y=0): Nx(x), Ny(y), mp(vector<vector<int> >(Nx+1, vector<int>(Ny+1, 0))) {}
		void add(int x, int y, int w){
			mp[x][y] = w;
		}

		bool Match(int x){
			visx[x] = 1;
			for(int i=0;i<Ny;i++){
				int y = i;
				if(visy[y]) continue;
				if(lx[x] + ly [y] > mp[x][y])
					slack[y] = min(slack[y], lx[x] + ly[y] - mp[x][y]);
				else{
					visy[y] = 1;
					if(my[y] == -1 || Match(my[y])){
						mx[x] = y, my[y] = x;
						return true;
					}
				}
			}
			return false;
		}

		int Solve(){
			mx = vector<int>(Nx+1, -1);
			my = vector<int>(Ny+1, -1);
			lx = vector<int>(Nx+1, -INF);
			ly = vector<int>(Ny+1, 0);
			for(int i=0;i<Nx;i++)
				for(int j=0;j<Ny;j++)
					lx[i] = max(lx[i], mp[i][j]);
			for(int i=0;i<Nx;i++){
				slack = vector<int>(Ny+1, INF);
				while(true){
					visx = vector<int>(Nx+1, 0);
					visy = vector<int>(Ny+1, 0);
					if(Match(i)) break;
					int d = INF;
					for(int j=0;j<Ny;j++)
						if(!visy[j]) d = min(d, slack[j]);
					if(d == INF)break;
					for(int i=0;i<Nx;i++)
						if(visx[i]) lx[i] -= d;
					for(int i=0;i<Ny;i++)
						if(visy[i]) ly[i] += d;
						else slack[i] -= d;
				}
			}
			int res = 0;
			for(int i=0;i<Nx;i++)
				if(mx[i] != -1)
					res += mp[i][mx[i]];
			return res;
		}
};

class DisjointSet {
	public:
		int N;
		vector<int> p;
		DisjointSet(int n): N(n), p(vector<int>(N)){
			init();
		}
		void init(){
			for(int i=0;i<N;i++)
				p[i] = i;
		}
		int find(int x){
			return p[x] == x ? x : p[x] = find(p[x]);
		}
		void U(int a, int b){
			p[find(b)] = find(a);
		}
};

/*
 * Maximun General Graph Matching
 * store answer in m
 * Solve() returns the number of matching
 * important!!!
 * notice the order of disjoint set when unioning
 */
class GMatch{
	public:
		int N;
		vector<vector<int> > vc;
		DisjointSet djs;
		vector<int> m, d, c1, c2, p, vis;
		queue<int> q;
		int ts;
		GMatch(int n): N(n), vc(vector<vector<int> >(N+1)), djs(DisjointSet(N)), ts(0){}

		void add(int a, int b){
			vc[a].push_back(b);
			vc[b].push_back(a);
		}

		void path(int x, int r){
			if(x==r)return;
			if(d[x] == 0){
				int i = p[x], j = p[p[x]];
				path(j, r);
				m[i] = j, m[j] = i;
			}
			else if(d[x] == 1){
				int i = c1[x], j = c2[x];
				path(i, m[x]);
				path(j, r);
				m[i] = j, m[j] = i;
			}
		}

		void blossom(int x, int y, int bi){
			for(int i=djs.find(x);i!=bi;i=djs.find(p[i])){
				djs.U(bi, i);
				if(d[i] == 1)
					c1[i] = x, c2[i] = y, q.push(i);
			}
		}

		int lca(int x,int y,int r){
			ts++;
			vis[r] = ts;
			for(int i=djs.find(x);i!=r;i=djs.find(p[i]))
				vis[i] = ts;
			int b;
			for(b=djs.find(y);vis[b]!=ts;b=djs.find(p[b]));
			return b;
		}

		bool Match(int x){
			djs.init();
			d = vector<int>(N+1, -1);
			d[x] = 0;
			q = queue<int>();
			q.push(x);
			while(!q.empty()){
				int u = q.front(); q.pop();
				for(int i=0;i<(int)vc[u].size();i++){
					int v = vc[u][i];
					if(m[v] != v && djs.find(u) != djs.find(v)){
						if(d[v] == -1){
							if(m[v] == -1){
								path(u, x);
								m[u] = v, m[v] = u;
								return true;
							}else{
								p[v] = u, p[m[v]] = v;
								d[v] = 1, d[m[v]] = 0;
								q.push(m[v]);
							}
						}else{
							if(d[djs.find(v)] == 0){
								int bi=lca(u, v, x);
								blossom(u, v, bi);
								blossom(v, u, bi);
							}
						}
					}
				}
			}
			return false;
		}

		int Solve(){
			m = c1 = c2 = d = p = vis = vector<int>(N+1, -1);
			int ans = 0;
			for(int i=0;i<N;i++){
				if(m[i] == -1){
					if(Match(i)) ans++;
					else m[i]=i;
				}
			}
			return ans;
		}
};

int main(){
	GMatch m(5);
	for(int i=0;i<5-1;i++)
		m.add(i, (i+1)%5);
	printf("%d\n", m.Solve());
	return 0;
}
