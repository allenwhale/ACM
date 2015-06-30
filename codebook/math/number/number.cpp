#include <stdio.h>
#include <vector>
using namespace std;
typedef long long ll;

ll ext_gcd(ll a,ll b,ll &x,ll &y){
    ll d=a;
    if(b!=0ll){
        d=ext_gcd(b,a%b,y,x);
        y-=(a/b)*x;
    }
    else x=1ll,y=0ll;
    return d;
}
//ax%n=b
vector<ll> line_mod_equation(ll a,ll b,ll n){
    ll x, y, d;
    d = ext_gcd(a, n, x, y);
    vector<ll> ans;
    if(b%d==0ll){
        x = (x%n + n) % n;
        ans.push_back((x*(b/d))%(n/d));
        for(ll i=1;i<d;i++)
            ans.push_back((ans[0]+i*n/d)%n);
    }
    return ans;
}
#define P 24851
ll mod_inverse(ll n){
    ll x, y;
    ll d = ext_gcd(n, P, x, y);
    return (P+x%P) % P;
}
int fact[P+1];
int mod_fact(int n,int &e){
    e = 0;
    if(n == 0) return 1;
    int res = mod_fact(n/P, e);
    e += n / P;
    if((n/P) % 2 == 0)
        return res * (fact[n%P]%P);
    return res * ((P-fact[n%P])%P);
}
int Cmod(int n,int m){
    fact[0] = 1;
    for(int i=1;i<=P;i++){
        fact[i] = fact[i-1] * i%P;
    }
    int a1, a2, a3, e1, e2, e3;
    a1 = mod_fact(n, e1);
    a2 = mod_fact(m, e2);
    a3 = mod_fact(n-m, e3);
    if(e1 > e2 + e3)return 0;
    return a1 * (int)mod_inverse(a2 * (a3%P)) % P;
}

int main(){
    printf("%d %d %d\n", Cmod(10, 5), Cmod(3, 2), Cmod(P-1, P/2));
}
