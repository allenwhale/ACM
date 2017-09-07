#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;
char o[100000],s[100000];
void build(char *ori,char *res){
	vector<int> v[30];
	int len = strlen(ori);
	for(int i = 0 ; i < len ; ++i){
		v[ori[i] - 'a'].push_back(i);
	}
	int tmp = 0;
	vector<int> a;
	for(int i = 0 ; i < 27; ++i){
		for(int j = 0 ; j < int(v[i].size()) ; ++j){
			a.push_back(v[i][j]);
			ori[tmp++] = 'a'+ i;
		}
	}
	tmp = 0;
	for(int i = 0 ; i < len ; ++i){
		res[i] = ori[a[tmp]];
		tmp = a[tmp];
	}
	res[len]='\0';
}
int main(){
	scanf("%s",o);
	build(o,s);
	printf("%s",s);
}

