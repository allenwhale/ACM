#include <stdio.h>
#include <vector>
#include <string.h>
#include <algorithm>
using namespace std;

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

int main(){
	int T;
	scanf("%d", &T);
	while(T--){
		int N, P;
		scanf("%d %d", &P, &N);
		BiMatch m(P, N);
		for(int i=0;i<P;i++){
			int num;
			scanf("%d", &num);
			while(num--){
				int s;
				scanf("%d", &s);
				s--;
				m.add(i, s);
			}
		}
		if(m.Solve() == P)puts("YES");
		else puts("NO");
	}
	return 0;
}
