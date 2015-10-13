/*
 * return the sequence of x-th of n!
 * max(n) = 12
 * 0 of 3! -> 123
 * 5 of 3! -> 321
 */
int factorial[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
vector<int> idx2permutation(int x, int n){
	vector<bool> used(n+1, false);
	vector<int> res(n);
	for(int i=0;i<n;i++){
		int tmp = x / factorial[n-i-1];
		int j;
		for(j=1;j<=n;j++)if(!used[j]){
			if(tmp == 0) break;
			tmp--;
		}
		res[i] = j, used[j] = true;
		x %= factorial[n-i-1];
	}
	return res;
}
/*
 * a is x-th og n!
 * return x(0~n!)
 * 123 of 3! -> 0
 * 321 of 3! -> 5
 */
int permutation2idx(vector<int> a){
	int res = 0;
	for(int i=0;i<(int)a.size();i++){
		int tmp = a[i] - 1;
		for(int j=0;j<i;j++)
			if(a[j] < a[i]) tmp--;
		res += factorial[(int)a.size()-i-1] * tmp;
	}
	return res;
}
