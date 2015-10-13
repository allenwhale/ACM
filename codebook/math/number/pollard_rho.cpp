#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll mul_mod(ll a, ll b, ll M){
    return b?(mul_mod((a<<1)%M, b>>1, M)+(b&1)?a:0)%M:a;
}
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
bool PrimeTest(ll a, ll n, ll u, int t) {
    ll x = pow_mod(a, u, n);
    for(int i=0;i<t;i++){
        ll nx = pow_mod(x, 2, n);
        if(nx==1&&x!=1&&x!=n-1)return true;
        x = nx;
    }
    return x != 1;
}
bool MillerRabin(ll n){
    if(n < 2) return false;
    if(!(n&1)) return false;
    ll u = n-1, t=0;
    while(!(u&1))u>>=1, t++;
    vector<ll> a = {2, 7, 61, 325, 9375, 28178,  450775, 9780504, 1795265022};
    for(int i=0;i<(int)a.size();i++)
        if(PrimeTest(a[i]%(n-1)+1, n, u, t))return false;
    for(int i=0;i<10000;i++)
        if(PrimeTest(rand()%(n-1)+1, n, u, t))return false;
    return true;
}
bool isprime(ll n){
    if(n==2||n==3)return true;
    if(n<2||!(n&1))return false;
    int s=0, d=n-1;
    while(!(d&1))d>>=1, s++;
    for(int i=0;i<100;i++){
        ll a = rand()%(n-2)+1;
        ll x = pow_mod(a, d, n);
        if(x==1||x==n-1)continue;
        for(int r=1;r<s;r++){
            x = pow_mod(x, 2, n);
            if(x==1)return false;
            if(x==n-1)goto loop;
        }
        return false;
loop:;
     continue;
    }
    return true;
}
ll factor(ll n){
    //if(isprime(n))return n;
    if(MillerRabin(n))return n;
    puts("start");
    ll res=1,x=2,x_fixed=x;
    while(res==1||res==n){
        x = (mul_mod(x, x, n)+rand()%(n-1)+1)%n;
        x_fixed = (mul_mod(x_fixed, x_fixed, n)+rand()%(n-1)+1)%n;
        x_fixed = (mul_mod(x_fixed, x_fixed, n)+rand()%(n-1)+1)%n;
        res = __gcd(abs(x-x_fixed), n);
        printf("  %lld %lld %lld %lld\n", x, x_fixed, abs(x-x_fixed), res);
    }
    return res;
}
int main(){
    ll a;
    while(~scanf("%lld", &a))
        printf("%lld\n", factor(a));
    return 0;
}
