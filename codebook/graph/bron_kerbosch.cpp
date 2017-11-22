/*
 * Maximal Clique
 * store maximal cliques in ans
 * __builtin_ctz: returns the number of trailing 0-bits
 * R = 0, P = all, X = 0
 */
typedef unsigned long long ull;
ull ed[64];
vector<ull> ans;
void BronKerbosch(ull R, ull P, ull X){
	if(P == 0 && X == 0){
		ans.push_back(R);
		return;
	}
	ull Q = P & ~ed[__builtin_ctzll(P | X)];
	while(Q){
		int i = __builtin_ctzll(Q);
		BronKerbosch(R | (1ULL << i), P & ed[i], X & ed[i]);
		Q &= ~(1ULL << i); P &= ~(1ULL << i); X |= (1ULL << i);
	}
}
