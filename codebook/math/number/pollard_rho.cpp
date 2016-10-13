//doesn't work when n is prime
ll f(ll x, ll mod){
	return (mul_mod(x, x, mod) + 1) % mod;
}
ll PollardRho(ll n){
	if(!(n&1)) return 2;
	while(true){
		ll y = 2, x = rand() % (n-1) + 1, res = 1;
		for(int sz=2;res==1;sz<<=1){
			for(int i=0;i<sz&&res==1;i++){
				x = f(x, n);
				res = __gcd(abs(x - y), n);
			}
			y = x;
		}
		if(res != 1 && res != n) return res;
	}
}
