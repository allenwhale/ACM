#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll mul_mod(ll a, ll b, ll M){
	return b?(mul_mod((a<<1)%M, b>>1, M) + (b&1?a:0))%M:0;
}
ll pow_mod(ll a, ll n, ll M){
	ll res=1;
	while(n){
		if(n&1)res=mul_mod(res,a,M);
		a=mul_mod(a,a,M);
		n>>=1;
	}
	return res;
}
#include "miller_rabin.cpp"
#include "pollard_rho.cpp"
int main(){
	srand(time(0));
	ll a=221009235;
	while(a++){
		clock_t now=clock();
		printf("factor %lld ", a);
		fflush(stdout);
		factor f = Factor(a);
		printf("%f\n", (double)(clock()-now)/CLOCKS_PER_SEC);
		fflush(stdout);
		//for(auto p: f)
		//	printf("%lld %d\n", p.first, p.second);
	}
	return 0;
}
