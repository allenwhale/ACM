ll pollard_rho(ll n){
	if(MillerRabin(n))return n;
	ll res=1,x=2,x_fixed=x;
	while(res==1||res==n){
		x = (mul_mod(x, x, n)+rand()%(n-1)+1)%n;
		x_fixed = (mul_mod(x_fixed, x_fixed, n)+rand()%(n-1)+1)%n;
		x_fixed = (mul_mod(x_fixed, x_fixed, n)+rand()%(n-1)+1)%n;
		res = __gcd(abs(x-x_fixed), n);
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
	ll x=pollard_rho(n), y=n/x;
	int nx=1, ny=1;
	if(x==y)y=1,nx++;
	factor fx=Factor(x), fy=Factor(y), res;
	while(x!=1&&n%x==0)n/=x, nx++;
	while(y!=1&&n%y==0)n/=y, ny++;
	for(auto p: fx)
		res[p.first] += p.second*nx;
	for(auto p: fy)
		res[p.first] += p.second*nx;
	return res;
}
