/*
 * Stoer Wanger
 * Undirected Min Cut
 * Solve() returns answer if graph is connected else 0
 */
class StoerWanger{
public:
    int N, wN;
    vector<vector<int> >G;
    vector<int> bln, dis;
    StoerWanger(int n=0): N(n), G(vector<vector<int> >(N, vector<int>(N))), bln(vector<int>(N, -1)), dis(vector<int>(N)) {}
    void add_edge(int a, int b, int c){
        G[a][b] += c;
        G[b][a] += c;
    }
    int Mst(int r, int &x, int &y){
        int t;
        bln[t=0] = r;
        for(int i=0;i<wN;i++)
            if(bln[i] != r)
                dis[i] = G[0][i];
        for(int k=0;k<wN-1;k++){
            x = t; t = 0;
            for(int i=0;i<wN;i++)
                if(bln[i] != r && (!t || dis[i] > dis[t]))
                    t = i;
            bln[t] = r;
            for(int i=0;i<wN;i++)
                if(bln[i] != r)
                    dis[i] += G[t][i];
        }
        y = t;
        return dis[t];
    }
    void Merge(int x, int y){
        if(x > y) swap(x, y);
        for(int i=0;i<wN;i++)
            if(i != x && i != y)
                G[i][x] += G[i][y], G[x][i] += G[y][i];
        if(y == wN-1) return;
        for(int i=0;i<wN-1;i++)
            if(i != y)
                swap(G[i][y], G[i][wN-1]), swap(G[y][i], G[wN-1][i]);
    }
    int Solve(){
        wN = N;
        int res = 0x3f3f3f3f;
        for(int i=0;wN>1;i++, wN--){
            int x, y;
            res = min(res, Mst(i, x, y));
            Merge(x, y);
        }
        return res;
    }
};
