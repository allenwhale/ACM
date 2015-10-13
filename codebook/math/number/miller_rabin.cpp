ll pow_mod(ll x, ll N, ll M) {
    ll res = 1;
    x %= M;
    while(N){
        if(N&1ll) res = mul_mod(res, x, M);
        x = mul_mod(x, x, M);
        N >>= 1;
    }
    return res;
}
bool PrimeTest(ll n, ll a, ll d) {
    if(n < 2) return false;
    if(n == 2 || n == a) return true;
    if((n&1) == 0) return false;
    while((d&1) == 0) d >>= 1;
    ll t = pow_mod(a, d, n);
    while((d!=n-1) && (t!=1) && (t!=n-1)){
        t = mul_mod(t, t, n);
        d <<= 1;
    }
    return (t==n-1) || ((d&1)==1);
}
bool MillerRabin(ll n){
    // test set
    vector<ll> a = {2, 325, 9375, 28178,  450775, 9780504, 1795265022};
    for(int i=0;i<(int)a.size();i++)
        if(!PrimeTest(n, a[i], n-1)) return false;
    return true;
}
