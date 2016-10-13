/* 
 * called by FFT 
 * build the sequence of a that used to calculate FFT
 * return a reversed sequence
 */
vector<Complex> reverse(const vector<Complex>& a){
	vector<Complex> res(a);
	for (int i=1,j=0;i<(int)res.size();i++){
		for(int k=((int)res.size())>>1;!((j^=k)&k);k>>=1);
		if(i > j) swap(res[i], res[j]);
	}
	return res;
}
/*
 * calculate the FFT of sequence
 * a.size() must be 2^k
 * flag = 1  -> FFT(a)
 * flag = -1 -> FFT-1(a)
 * return FFT(a) or FFT-1(a)
 */
vector<Complex> FFT(const vector<Complex>& a, int flag=1){
	vector<Complex> res = reverse(a);
	for(int k=2;k<=(int)res.size();k<<=1){
		double p0 = -pi / (k>>1) * flag;
		Complex unit_p0(cos(p0), sin(p0));
		for(int j=0;j<(int)res.size();j+=k){
			Complex unit(1.0, 0.0);
			for(int i=j;i<j+k/2;i++,unit*=unit_p0){
				Complex t1 = res[i], t2 = res[i+k/2] * unit;
				res[i] = t1 + t2;
				res[i+k/2] = t1 - t2;
			}
		}
	}
	if(flag == -1) for(auto &x:res) x /= (double)res.size();
	return res;
}
