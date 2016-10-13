/*
 * KMP: does T in P
 */
#define MAXN 10010
char T[MAXN], P[MAXN];
int fail[MAXN], lenT, lenP;
void build() {
	lenT = strlen(T);
	for(int i=1, j=fail[0]=-1;i<lenT;i++) {
		while(j >= 0 && T[j + 1] != T[i])
			j = fail[j];
		if(T[j + 1] == T[i]) j++;
		fail[i] = j;
	}
}
bool find() {
	lenP = strlen(P);
	for(int i=0, j=-1;i<lenP;i++) {
		while(j >= 0 && T[j + 1] != P[i])
			j = fail[j];
		if(T[j + 1] == P[i]) j++;
		if(j == lenT - 1) {
			j = fail[j];
			return true;
		}
	}
	return false;
}
