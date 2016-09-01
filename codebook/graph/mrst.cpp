/*
 * Minimum Ratio Spanning Tree
 * Solve() returns answer of MRST if there exists an answer else -1
 */
struct MRST {
#define w  first
#define u  second
    typedef pair<double, double> PD;
    int N;
    vector<vector<PD> > G;
    vector<vector<double> > wG;
    MRST(int n=0): N(n), G(vector<vector<PD> >(N, vector<PD>(N))), wG(vector<vector<double> >(N, vector<double>(N))) {
    }
    void add_edge(int a, int b, double _w, double _u){
        G[a][b] = PD(_w, _u);
    }
    void build(double chk){
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                wG[i][j] = G[i][j].w - chk * G[i][j].u;
    }
    double Mst(double chk){
        build(chk);
        vector<bool> vis(N+1, false);
        vector<double> dis(N+1, 1e9);
        vector<int> pre(N+1);
        double W = 0, U = 0;
        int v = 0;
        int times = 0;
        while(++times < N){
            vis[v] = true;
            for(int i=0;i<N;i++)
                if(!vis[i] && dis[i] > wG[v][i])
                    dis[i] = wG[v][i], pre[i] = v;
            double mn = 1e9;
            for(int i=0;i<N;i++)
                if(!vis[i] && mn > dis[i])
                    mn = dis[i], v = i;
            if(mn == 1e9)
                return -1;
            W += G[pre[v]][v].w;
            U += G[pre[v]][v].u;
        }
        return W / U;
    }
    double Solve(){
        double last = -1, cur = 0;
        const double EPS = 1e-9;
        while(fabs(last - cur) > EPS){
            last = cur;
            cur = Mst(last);
        }
        return cur;
    }
};
