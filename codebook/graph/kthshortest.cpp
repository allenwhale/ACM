/*
 * ed[0] is original graph, ed[1] is reversed graph
 */
#define w first
#define to second
#define MAXN 10010
vector<PI> ed[2][MAXN];
int N, dis[MAXN];
void AllDijkstra(int s, int g){
    memset(dis, 0x3f, sizeof(dis));
    dis[s] = 0;
    priority_queue<PI, vector<PI>, greater<PI> > pq;
    pq.push({dis[s], s});
    while(!pq.empty()){
        PI now = pq.top(); pq.pop();
        if(now.w > dis[now.to]) continue;
        for(int i=0;i<(int)ed[g][now.to].size();i++){
            PI e = ed[g][now.to][i];
            if(dis[e.to] > dis[now.to] + e.w){
                dis[e.to] = dis[now.to] + e.w;
                pq.push({dis[e.to], e.to});
            }
        }
    }
}
int KthShortestPath(int s, int t, int k){
    for(int i=0;i<N;i++){
        for(int j=0;j<(int)ed[0][i].size();j++){
            PI e = ed[0][i][j];
            ed[1][e.to].push_back({e.w, i});
        }
    }
    AllDijkstra(t, 1);
    priority_queue<PI> pq;
    pq.push({-dis[s], s});
    while(!pq.empty()){
        PI now = pq.top(); pq.pop();
        int real = -now.w - dis[now.to];
        if(now.to == t && (!(--k)))
            return real;
        
		for(int i=0;i<(int)ed[0][now.to].size();i++){
			PI e = ed[0][now.to][i];
			pq.push({-(real+e.w+dis[e.to]), e.to});
		}
    }
    return -1;
}
