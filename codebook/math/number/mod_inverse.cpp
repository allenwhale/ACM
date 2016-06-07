/*
 * find the inverse of n modular p
 */
ll mod_inverse(ll n, ll p){
	ll x, y;
	ll d = ext_gcd(n, p, x, y);
	return (p + x % p) % p;
}
