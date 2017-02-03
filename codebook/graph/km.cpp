/*
 * solve Maximun Bipartite Matching
 * store matching answer in mx ,my
 * Solve() returns the maximun weight of perfect matching
 */
struct KM{
#define FF first
#define SS second
#define INF 0x3f3f3f3f
	typedef pair<int, int> PI;
	int N;
	vector<vector<int> >mp;
	vector<int> visx, visy;
	vector<int> lx, ly, slack;
	vector<int> mx, my;
	KM(int n): N(n), mp(vector<vector<int> >(N+1, vector<int>(N+1, 0))) {}
	void add(int x, int y, int w){
		mp[x][y] = w;
	}

	bool Match(int x){
		visx[x] = 1;
		for(int i=0;i<N;i++){
			int y = i;
			if(visy[y]) continue;
			int tmp = lx[x] + ly[y] - mp[x][y];
			if(tmp == 0){
				visy[y] = 1;
				if(my[y] == -1 || Match(my[y])){
					mx[x] = y, my[y] = x;
					return true;
				}
			}else{
				slack[y] = min(slack[y], tmp);
			}
		}
		return false;
	}

	int Solve(){
		mx = vector<int>(N+1, -1);
		my = vector<int>(N+1, -1);
		lx = vector<int>(N+1, -INF);
		ly = vector<int>(N+1, 0);
		slack = vector<int>(N+1, INF);
		visx = vector<int>(N+1, 0);
		visy = vector<int>(N+1, 0);
		for(int i=0;i<N;i++)
			for(int j=0;j<N;j++)
				lx[i] = max(lx[i], mp[i][j]);
		for(int i=0;i<N;i++){
			fill(slack.begin(), slack.end(), INF);
			while(true){
				fill(visx.begin(), visx.end(), 0);
				fill(visy.begin(), visy.end(), 0);
				if(Match(i)) break;
				int d = INF;
				for(int j=0;j<N;j++)
					if(!visy[j]) d = min(d, slack[j]);
				if(d == INF)break;
				for(int i=0;i<N;i++)
					if(visx[i]) lx[i] -= d;
				for(int i=0;i<N;i++)
					if(visy[i]) ly[i] += d;
					else slack[i] -= d;
			}
		}
		int res = 0;
		for(int i=0;i<N;i++)
			if(mx[i] != -1)
				res += mp[i][mx[i]];
		return res;
	}
};
