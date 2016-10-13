/*
 * BIpartite Matching
 * Nx = number of x nodes
 * Ny = number of y nodes
 * store matching answer in mx, my
 * Solve() returns the number of matching
 */
struct BiMatch{
	int Nx, Ny;
	vector<vector<int> > vc;
	vector<int> mx, my;
	vector<int> visy;

	BiMatch(int _x=0, int _y=0): Nx(_x), Ny(_y), vc(vector<vector<int> >(Nx+1)){}

	void add(int x, int y){
		vc[x].push_back(y);
	}

	bool Match(int x){
		for(int y:vc[x]){
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
		visy = vector<int>(Ny+1, 0);
		int ans = 0;
		for(int i=0;i<Nx;i++){
			if(mx[i] == -1){
				fill(visy.begin(), visy.end(), 0);
				ans += Match(i);
			}
		}
		return ans;
	}
};
