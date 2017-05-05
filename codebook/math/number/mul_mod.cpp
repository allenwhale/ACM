ll mul_mod(ll a,ll b,ll c){
    a %= c;
    b %= c;
    ll r = 0;
    while(a){
        if(a & 1){
            r += b;
            if(r >= c) r -= c;
        }
        b <<= 1;
        if(b >= c)b -= c;
        a >>= 1;
    }
    return r;
}

