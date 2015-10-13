/*
 * Minimum Directed Spanning Tree
 * Solve() return answer of mdst if there exists else -1
 */
class MDST{
public:
#define v1  first.first
#define v2  first.first
#define w   second
    const static int INF = 0x3f3f3f3f;
    typedef pair<int, int> PI;
    typedef pair<PI, int> PII;
    int N;
    vector<PII> E;
    MDST(int n=0): N(n){}
    int Solve(int r){
        vector<bool> mrg(N+1, false);
        vector<int>  dis(N+1, 0);
        vector<int>  vis(N+1, 0);
        vector<int>  pre(N+1, 0);
        vector<int>  bln(N+1, 0);
        int allw = 0, tmpw = 0;
        while(true){
            tmpw = 0;
            fill(dis.begin(), dis.end(), INF);
            fill(vis.begin(), vis.end(), -1);
            fill(bln.begin(), bln.end(), -1);
            for(int i=0;i<(int)E.size();i++){
                PII e = E[i];
                if(e.v1 != e.v2 && e.v2 != r && e.w < dis[e.v2])
                    dis[e.v2] = e.w, pre[e.v2] = e.v1;
            }
            bool tf = false;
            for(int i=0;i<N;i++){
                if(mrg[i]) continue;
                if(pre[i] == -1 && i != r) return -1;
                if(pre[i] != -1) tmpw += dis[i];
                int s;
                for(s=i;s!=-1&&vis[s]==-1;s=pre[s])
                    vis[s] = i;
                if(s != -1 && vis[s] == i){
                    tf = true;
                    int j = s;
                    do{
                        bln[j] = s;
                        mrg[j] = true;
                        allw += dis[j];
                        j = pre[j];
                    }while(j != s);
                    mrg[s] = false;
                }
            }
            if(tf == false) break;
            for(int i=0;i<(int)E.size();i++){
                PII &e = E[i];
                if(bln[e.v2] != -1) e.w -= dis[e.v2];
                if(bln[e.v1] != -1) e.v1 = bln[e.v1];
                if(bln[e.v2] != -1) e.v2 = bln[e.v2];
                if(e.v1 == e.v2) {
                    e = E.back();
                    E.pop_back();
                    i--;
                }
            }
        }
        return allw + tmpw;
    }
};
