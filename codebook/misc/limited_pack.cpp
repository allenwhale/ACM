#include <bits/stdc++.h>
using namespace std;
#define MAXV 1000010
#define MAXN 100010
// wieght, value, num
int w[MAXN], v[MAXN], n[MAXN];
int dp[MAXV], p[MAXV], q[MAXV];

int N, W; //N kinds, total wieght W
void pack(int x) {
	for(int i=0;i<w[x];i++){
		int *pf=p,*pb=p-1;
		int *qf=q,*qb=q-1;
		for(int j=i,k=0;j<=W;j+=w[x],k++) {
			if(pb==pf+n[x]) {
				if(*qf==*pf)qf++;
				pf++;
			}
			int tmp=dp[j]-k*v[x];
			*++pb=tmp;
			while(qb>=qf&&*qb<tmp)qb--;
			*++qb=tmp;
			dp[j]=*qf+k*v[x];
		}
	}
}
int main()
{
    scanf("%d %d", &N, &W);
    for(int i=0;i<N;i++)
        scanf("%d %d %d", &n[i], &v[i], &w[i]);
    for(int i=0;i<N;i++) pack(i);
    printf("%d\n", dp[W]);
    return 0;
}
