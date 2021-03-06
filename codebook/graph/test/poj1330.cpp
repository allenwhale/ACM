#include <bits/stdc++.h>
using namespace std;

class LCA{
public:
    int N;
    vector<vector<int> > vc;
    vector<vector<int> > anc;
    vector<int> dep;
    LCA(int n=1): N(n), vc(vector<vector<int> >(N)) {}

    void add_edge(int a, int b){
        vc[a].push_back(b);
        vc[b].push_back(a);
    }

    void dfs(int x, int p, int d){
        dep[x] = d;
        anc[x][0] = p;
        for(int i=1;i<(int)anc[x].size();i++)
            anc[x][i] = anc[anc[x][i-1]][i-1];
        for(int i=0;i<(int)vc[x].size();i++){
            int v = vc[x][i];
            if(v == p)continue;
            dfs(v, x, d+1);
        }
    }
    void init(int x){
        anc = vector<vector<int> >(N+1, vector<int>((int)log2(N)+1));
        dep = vector<int>(N+1);
        dfs(x, x, 0);
    }
    void up(int &x, int h){
        for(int i=0;h;i++){
            if(h&1)x = anc[x][i];
            h >>= 1;
        }
    }
    int find(int x, int y){
        if(dep[x] > dep[y]) swap(x, y);
        up(y, dep[y]-dep[x]);
        if(x == y) return x;
        for(int i=anc[0].size()-1;i>=0;i--)
            if(anc[x][i] != anc[y][i])
                x = anc[x][i], y = anc[y][i];
        return anc[x][0];
    }
};

int main(){
    int T;
    scanf("%d", &T);
    while(T--){
        int N;
        scanf("%d", &N);
        LCA lca(N);
        vector<int> in(N+1, 0);
        for(int i=0;i<N-1;i++){
            int a, b;
            scanf("%d %d", &a, &b);
            a--, b--;
            lca.add_edge(a, b);
            in[b]++;
        }
        int root = -1;
        for(int i=0;i<N;i++)
            if(in[i]==0)root=i;
        lca.init(root);
        int a, b;
        scanf("%d %d", &a, &b);
        a--, b--;
        printf("%d\n", lca.find(a, b)+1);
    }
    return 0;
}
