#include <stdio.h>
#include <vector>
#include <string.h>
#include <algorithm>
using namespace std;

/*
 * BIpartite Matching
 * Nx = number of x nodes
 * Ny = number of y nodes
 * mx, my are the answer
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
#define INF 0x3f3f3f3f
	typedef pair<int, int> PI;
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

int main(){
    int N;
    scanf("%d", &N);
    KM km(N, N);
    for(int i=0;i<N;i++){
        int tw = 0;
        vector<int> tmp;
        for(int j=0;j<N;j++){
            int w;
            scanf("%d", &w);
            tw += w;
            tmp.push_back(w);
        }
        for(int j=0;j<N;j++)
            km.add(i, j, -(tw-tmp[j]));
    }
    printf("%d\n", -km.Solve());
	return 0;
}
