bool PrimeTest(ll a, ll n, ll u, int t) {
    ll x = pow_mod(a, u, n);
    for(int i=0;i<t;i++){
        ll nx = pow_mod(x, 2, n);
        if(nx == 1 && x != 1 && x != n - 1)
            return true;
        x = nx;
    }
    return x != 1;
}
// n < 4,759,123,141        3 :  2, 7, 61
// n < 1,122,004,669,633    4 :  2, 13, 23, 1662803
// n < 3,474,749,660,383    6 :  pirmes <= 13
// n < 2^64                 7 : 2, 325, 9375, 28178, 450775, 9780504, 1795265022
bool MillerRabin(ll n){
    if(n == 2 || n == 3) return true;
    if(n < 2 || !(n & 1)) return false;
    ll u = n - 1, t = 0;
    while(!(u&1))u >>= 1, t++;
    vector<ll> a = {2, 7, 61};
    for(int x:a)
        if(x % n && PrimeTest(x % n, n, u, t))
            return false;
    return true;
}
