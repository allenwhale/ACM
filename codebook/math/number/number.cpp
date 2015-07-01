#include <stdio.h>
#include <vector>
#include <math.h>
#include <complex>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
#define EPS 1e-10
#define INF 1e15
typedef complex<double> Complex;
const double pi = acos(-1);

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
        double e = 1.0;
        for(int i=0;i<(int)coef.size();i++,e*=rhs)
            res += e * coef[i];
        return res;
    }
    
    Function derivative() const {
        vector<double> dc((int)this->coef.size()-1);
        for(int i=0;i<(int)dc.size();i++)
            dc[i] = coef[i+1] * (i+1);
        return Function(dc);
    }

    int degree() const {
        return (int)coef.size()-1;
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

int sign(double x){
    return x < -EPS ? -1 : x > EPS;
}

template<class T>
double find(const T &f, double lo, double hi){
    int sign_lo, sign_hi;
    if((sign_lo=sign(f(lo))) == 0) return lo;
    if((sign_hi=sign(f(hi))) == 0) return hi;
    if(sign_hi * sign_lo > 0) return INF;
    for(int i=0;i<100&&hi-lo>EPS;i++){
        double m = (hi+lo) / 2;
        int sign_mid = sign(f(m));
        if(sign_mid == 0) return m;
        if(sign_lo * sign_mid < 0)
            hi = m;
        else lo = m;
    }
    return (lo+hi) / 2;
}

template<class T>
vector<double> eqation(const T &f){
    vector<double> res;
    if(f.degree() == 1){
        if(sign(f.coef[1]))res.push_back(-f.coef[0]/f.coef[1]);
        return res;
    }
    vector<double> droot = eqation(f.derivative());
    droot.insert(droot.begin(), -INF);
    droot.push_back(INF);
    for(int i=0;i<(int)droot.size()-1;i++){
        double tmp = find(f, droot[i], droot[i+1]);
        if(tmp < INF) res.push_back(tmp);
    }
    return res;
}

vector<Complex> reverse(vector<Complex> a, int sz){
    vector<Complex> res(a);
    for (int i=1,j=0;i<sz;i++){
        for(int k=sz>>1;!((j^=k)&k);k>>=1);
        if(i > j) swap(res[i], res[j]);
    }
    return res;
}

vector<Complex> FFT(vector<Complex> a, int sz, int flag=1){
    vector<Complex> res = reverse(a, sz);
    for(int k=2;k<=sz;k<<=1){
        double p0 = -pi / (k>>1) * flag;
        Complex unit_p0(cos(p0), sin(p0));
        for(int j=0;j<sz;j+=k){
            Complex unit(1.0, 0.0);
            for(int i=j;i<j+k/2;i++,unit*=unit_p0){
                Complex t1 = res[i], t2 = res[i+k/2] * unit;
                res[i] = t1 + t2;
                res[i+k/2] = t1 - t2;
            }
        }
    }
    return res;
}

// max 12!
int factorial[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
vector<int> idx2permutation(int x, int n){
    vector<bool> used(n+1, false);
    vector<int> res(n);
    for(int i=0;i<n;i++){
        int tmp = x / factorial[n-i-1];
        int j;
        for(j=1;j<=n;j++)if(!used[j]){
            if(tmp == 0) break;
            tmp--;
        }
        res[i] = j, used[j] = true;
        x %= factorial[n-i-1];
    }
    return res;
}

int permutation2idx(vector<int> a){
    int res = 0;
    for(int i=0;i<(int)a.size();i++){
        int tmp = a[i] - 1;
        for(int j=0;j<i;j++)
            if(a[j] < a[i]) tmp--;
        res += factorial[(int)a.size()-i-1] * tmp;
    }
    return res;
}


int main(){
    srand(time(0));
    vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    random_shuffle(a.begin(), a.end());
    for(auto _: a)
        printf("%d\n", _);
    printf("test = %d\n", idx2permutation(permutation2idx(a), a.size()) == a);
}
