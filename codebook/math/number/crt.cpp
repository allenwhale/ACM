/*
 * solve the chinese remainder theorem(CRT)
 * if a.size() != m.size(), return -1
 * return the minimun positive answer of CRT
 * x = a[i] (mod m[i])
 */
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
