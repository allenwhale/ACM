#include <stdio.h>
#include <vector>
#include <math.h>
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
ll mod_inverse(ll n, ll p){
    ll x, y;
    ll d = ext_gcd(n, p, x, y);
    return (p+x%p) % p;
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
    return a1 * mod_inverse(a2 * (a3%P), P) % P;
}
//Chinese remainder theorem
int CRT(vector<int> a, vector<int> m) {
    if(a.size() != m.size()) return -1;
    int M = 1;
    for(int i=0;i<(int)m.size();i++)
        M *= m[i];
    int res = 0;
    for(int i=0;i<(int)a.size();i++)
        res = (res + (M/m[i])*mod_inverse(M/m[i], m[i])*a[i]) % M;
    return (res + M) % M;
}

ll pow_mod(ll x, ll N, ll M) {
    ll res = 1;
    x %= M;
    while(N){
        if(N&1ll) res = (res*x) % M;
        x *= x; x %= M;
        N >>= 1;
    }
    return res;
}

bool prime_test(ll n, ll a, ll d) {
    if(n == 2 || n == a) return true;
    if((n&1) == 0) return false;
    while((d&1) == 0) d >>= 1;
    ll t = pow_mod(a, d, n);
    while((d!=n-1) && (t!=1) && (t!=n-1)){
        t = (t*t) % n;
        d <<= 1;
    }
    return (t==n-1) || ((d&1)==1);
}

bool miller_rabin(ll n) {
    vector<ll> a = {2, 7 ,61};
    for(int i=0;i<(int)a.size();i++)
        if(!prime_test(n, a[i], n-1)) return false;
    return true;
}

#define MAXN 100
int mindiv[MAXN], phi[MAXN], sum[MAXN];
void genphi(){
    for(int i=1;i<MAXN;i++)
        mindiv[i] = i;
    for(int i=2;i*i<MAXN;i++)
        if(mindiv[i] == i)
            for(int j=i*i;j<MAXN;j+=i)
                mindiv[j] = i;
    phi[1] = 1;
    for(int i=2;i<MAXN;i++){
        phi[i] = phi[i/mindiv[i]];
        if((i/mindiv[i])%mindiv[i] == 0)
            phi[i] *= mindiv[i];
        else phi[i] *= (mindiv[i]-1);
    }
}

class Function {
public:
    Function(const vector<double> c=vector<double>()): coef(c){}
    vector<double> coef;
    double operator () (const double &rhs) const {
        double res = 0.0;
        for(int i=0;i<(int)coef.size();i++)
            res += pow(rhs, i) * coef[i];
        return res;
    }
};

template<class T>
double simpson(const T &f, double a, double b, int n){
    double h = (b-a) / n;
    double ans = f(a) + f(b);
    for(int i=1;i<n;i+=2)
        ans += 4 * f(a+h*i);
    for(int i=2;i<n;i+=2)
        ans += 2 * f(a+h*i);
    return ans * h / 3;
}

int main(){
    vector<double> c = {1, 2, 3, 4};
    //2x+1
    Function f(c);
    printf("%f\n", simpson(f, 10, 1000, 100));
}
