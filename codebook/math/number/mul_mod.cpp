ll mul_mod(ll a, ll b, ll m){
    return b?(mul_mod((a<<1)%m,b>>1,m)+(b&1?a:0))%m:0;
}

