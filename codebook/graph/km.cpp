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
