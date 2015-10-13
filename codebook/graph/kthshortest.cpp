int KthShortestPath(int s, int t, int k){
	Graph RG(N);
	for(int i=0;i<N;i++)
		for(int j=0;j<(int)vc[i].size();j++){
			Edge e = vc[i][j];
			RG.add_edge(e.to, Edge(i, e.w));
		}
	RG.AllDijkstra(t);
	dis = RG.dis;
	priority_queue<PI> pq;
	pq.push(PI(-dis[s], s));
	while(!pq.empty()){
		PI v = pq.top();
		pq.pop();
		int real = -v.FF - dis[v.SS];
		if(v.SS == t && (!(--k)))
			return real;
		for(int i=0;i<(int)vc[v.SS].size();i++){
			Edge e = vc[v.SS][i];
			pq.push(PI(-(real+e.w+dis[e.to]), e.to));
		}
	}
	return -1;
}
