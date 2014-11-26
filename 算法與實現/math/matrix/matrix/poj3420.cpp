#include <stdio.h>
#include <string.h>
int N;
long long M;
const int MAXN=5;
const int MAXM=5;
struct Matrix
{
    int n,m;
    long long a[MAXN][MAXM];
    void clear()
    {
	n=m=0;
	memset(a,0,sizeof(a));
    }
    Matrix operator * (const Matrix& b)const
    {
	Matrix tmp;
	tmp.clear();
	tmp.n=n,tmp.m=m;
	for(int i=0;i<n;i++)
	{
	    for(int j=0;j<m;j++)
	    {
		for(int k=0;k<m;k++)
		{
		    tmp.a[i][j]+=(a[i][k]*b.a[k][j])%M;
		    tmp.a[i][j]%=M;
		}
	    }
	}
	return tmp;
    }
    Matrix pow(int x)
    {
	Matrix res,tmp;
	res.clear();
	tmp.n=res.n=n;
	tmp.m=res.m=m;
	memcpy(tmp.a,this->a,sizeof(this->a));
	for(int i=0;i<res.n;i++)
	    res.a[i][i]=1;
	while(x)
	{
	    if(x&1)res=res*tmp;
	    tmp=tmp*tmp;
	    x>>=1;
	}
	return res;
    }
};
int main()
{
    while(~scanf("%d %lld",&N,&M)&&N&&M)
    {
	Matrix t;t.clear();
	t.n=t.m=4;
	t.a[0][0]=1;t.a[0][1]=5;t.a[0][2]=1;t.a[0][3]=-1;
	t.a[1][0]=t.a[2][1]=t.a[3][2]=1;
	t=t.pow(N);
	int ans=0;
	ans+=(11*t.a[3][0])%M;
	ans=(ans+5*t.a[3][1]%M)%M;
	ans=(ans+t.a[3][2])%M;
	ans=(ans+t.a[3][3])%M;
	printf("%d\n",(ans+M)%M);
    }
}
