/* 
 * called by TwoSat 
 * get the value of i-th
 * 1 = true, 0 = false, -1 = undefined
 */
int twosatans[MAXN * 4];
int TwoSatGet(int x){
	int r = x > N/2 ? x-N/2 : x;
	if(twosatans[r] == -1)
		return -1;
	return x > N/2 ? !twosatans[r] : twosatans[r];
}
bool TwoSat(){
	SCC();
    memset(twosatans, -1, sizeof(twosatans));
	for(int i=0;i<N/2;i++)
		if(scc[i] == scc[i+N/2])
			return false;
	vector<vector<int> > c(scc_cnt+1);
	for(int i=0;i<N;i++)
		c[scc[i]].push_back(i);
	for(int i=0;i<scc_cnt;i++){
		int val = 1;    
        for(int x:c[i]){
			if(TwoSatGet(x) == 0) val = 0;
            for(int y:ed[x])
				if(TwoSatGet(y) == 0) val = 0;
			if(!val) break;
		}
        for(int x:c[i]){
			if(x > N/2) twosatans[x-N/2] = !val;
			else twosatans[x] = val;
		}
	}
	return true;
}
