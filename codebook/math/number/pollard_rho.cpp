ll f(ll a, ll M){
	return (mul_mod(a, a, M)+1)%M;
}
ll pollard_rho(ll n){
	if(MillerRabin(n))return n;
	ll res=n,x,y, c;
	x=((ll)rand()*rand()+2)%n,y=x;
	while(res==1||res==n){
		x = f(x, n);
		y = f(y, n);
		y = f(y, n);
		if(x==y)x=((ll)rand()*rand()+2)%n,y=x;
		res = __gcd(abs(x-y), n);
	}
	return res;
}
typedef unordered_map<ll, int> factor;
factor Factor(ll n){
	if(n==1ll)return factor();
	if(MillerRabin(n)){
		factor res;
		res[n] = 1;
		return res;
	}
	factor res;
	vector<int> prime={2,3,5,7};
	for(auto p:prime){
		int cnt=0;
		while((n%p)==0)cnt++, n/=p;
		res[p] = cnt;
	}
	if(n==1)return res;
	ll x=pollard_rho(n), y=n;
	int nx=0;
	while(y%x==0)y/=x,nx++;
	factor fx=Factor(x), fy=Factor(y);
	for(auto p: fx)res[p.first] += p.second*nx;
	for(auto p: fy)res[p.first] += p.second;
	return res;
}
