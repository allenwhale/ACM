/* P is the modular number */
#define P 24851
int fact[P+1];
/* called by Cmod */
int mod_fact(int n,int &e){
    e = 0;
    if(n == 0) return 1;
    int res = mod_fact(n/P, e);
    e += n / P;
    if((n/P) % 2 == 0)
        return res * (fact[n%P]%P);
    return res * ((P-fact[n%P])%P);
}
/*
 * return C(n, m) mod P
 */
int Cmod(int n,int m){
    /* this section only need to be done once */
    fact[0] = 1;
    for(int i=1;i<=P;i++){
        fact[i] = fact[i-1] * i%P;
    }
    /* end */
    int a1, a2, a3, e1, e2, e3;
    a1 = mod_fact(n, e1);
    a2 = mod_fact(m, e2);
    a3 = mod_fact(n-m, e3);
    if(e1 > e2 + e3)return 0;
    return a1 * mod_inverse(a2 * (a3%P), P) % P;
}
