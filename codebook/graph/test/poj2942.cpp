#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <string.h>
#include <queue>
using namespace std;
#define MAXN 1010
vector<int> ed[MAXN];
int N, stk[MAXN], top, dfn[MAXN], low[MAXN];
vector<vector<int> > bcc;
void _BBC(int x, int d){
	stk[++top] = x;
	dfn[x] = low[x] = d;
	for(int i=0;i<(int)ed[x].size();i++){
		int to = ed[x][i];
		if(dfn[to] == -1){
			_BBC(to, d + 1);
			if(low[to] >= dfn[x]){
				vector<int> l;
				do{
					l.push_back(stk[top]);
					top--;
				}while(stk[top + 1] != to);
				l.push_back(x);
				bcc.push_back(l);
			}
			low[x] = min(low[x], low[to]);
		}else low[x] = min(low[x], dfn[to]);
	}
}
void BCC(){
    memset(dfn, -1, sizeof(dfn));
    memset(low, -1, sizeof(low));
    bcc.clear();
    top = -1;
	for(int i=0;i<N;i++)
		if(dfn[i] == -1)
			_BBC(i, 0);
}
bool in[MAXN];
int color[MAXN];
bool odd_cycle(vector<int> &bcc){
    memset(in, false, sizeof(in));
    memset(color, -1, sizeof(color));
    for(int i=0;i<(int)bcc.size();i++)
        in[bcc[i]] = true;
    queue<int> q;
    q.push(bcc[0]);
    color[bcc[0]] = 1;
    while(!q.empty()){
        int t = q.front();
        q.pop();
        for(int i=0;i<(int)ed[t].size();i++){
            int to = ed[t][i];
            if(in[to] == false) continue;
            if(color[to] != -1 && color[to] == color[t])
                return false;
            if(color[to] == -1){
                color[to] = color[t] ^ 1;
                q.push(to);
            }
        }
    }
    return true;
}
bool RG[1010][1010];
int main(){
    int M;
    while(~scanf("%d %d", &N, &M) && N && M){
        for(int i=0;i<=N;i++)
            ed[i].clear();
        vector<bool> odd(N+1, false);
        memset(RG, true, sizeof(RG));
        for(int i=0;i<M;i++){
            int a, b;
            scanf("%d %d", &a, &b);
            a--, b--;
            RG[a][b] = RG[b][a] = false;
        }
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                if(i != j && RG[i][j])
                    ed[i].push_back(j);
        BCC();
        int ans = 0;
        for(int i=0;i<(int)bcc.size();i++){
            vector<int> vc = bcc[i];
            if((int)vc.size()>=3&&!odd_cycle(vc)){
                for(int j=0;j<(int)vc.size();j++)
                    odd[vc[j]] = true;
            }
        }
        for(int i=0;i<N;i++)
            ans += (!odd[i]);
        printf("%d\n", ans);
    }
    return 0;
}
