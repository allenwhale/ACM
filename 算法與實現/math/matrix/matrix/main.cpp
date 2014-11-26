#include <stdio.h>
#include <string.h>
#include "matrix.cpp"
int main()
{
    Matrix a,b;
    a.clear();b.clear();
    a.n=a.m=b.n=b.m=5;
    for(int i=0;i<a.n;i++)
	for(int j=0;j<a.m;j++)
	    a.a[i][j]=b.a[i][j]=1;
    Matrix c=a+b;
    for(int i=0;i<a.n;i++,puts(""))
	for(int j=0;j<a.m;j++)
	    printf("%d ",c.a[i][j]);
    c=a-b;
    for(int i=0;i<a.n;i++,puts(""))
	for(int j=0;j<a.m;j++)
	    printf("%d ",c.a[i][j]);
    c=a*b;
    for(int i=0;i<a.n;i++,puts(""))
	for(int j=0;j<a.m;j++)
	    printf("%d ",c.a[i][j]);
    return 0;
}
