#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
#define MAXN 110
vector<int> ed[MAXN];
bool cut[MAXN], bridge[MAXN][MAXN];
int N, dfn[MAXN], low[MAXN];
void _cut_bridge(int x, int f, int d){
	dfn[x] = low[x] = d;
	int children = 0;
	for(int i=0;i<(int)ed[x].size();i++){
		int to = ed[x][i];
		if(to != f && dfn[to] != -1)
			low[x] = min(low[x], dfn[to]);
		if(dfn[to] == -1){
			_cut_bridge(to, x, d+1);
			children++;
			low[x] = min(low[x], low[to]);
			if((f == -1 && children > 1) || (f != -1 && low[to] >= dfn[x]))
				cut[x] = true;
			if(low[to] > dfn[x])
				bridge[x][to] = bridge[to][x] = true;
		}
	}
}
void cut_bridge(){
    memset(dfn, -1, sizeof(dfn));
    memset(low, 1, sizeof(low));
    memset(cut, false, sizeof(cut));
    memset(bridge, false, sizeof(bridge));
	for(int i=0;i<N;i++){
		if(dfn[i] == -1)
			_cut_bridge(i, -1, 0);
	}
}
int main(){
    char in[1000];
    while(~scanf("%d%*c", &N) && N){
        for(int i=0;i<=N;i++)
            ed[i].clear();
        while(gets(in)){
            char *ptr = strtok(in, " ");
            int v;
            sscanf(ptr, "%d", &v);
            if(v == 0) break;
            v--;
            ptr = strtok(NULL, " ");
            while(ptr){
                int tmp;
                sscanf(ptr, "%d", &tmp);
                tmp--;
                ed[v].push_back(tmp);
                ed[tmp].push_back(v);
                ptr = strtok(NULL, " ");
            }
        }
        cut_bridge();
        int ans = 0;
        for(int i=0;i<N;i++)
            ans += (int)cut[i];
        printf("%d\n", ans);
    }
    return 0;
}
