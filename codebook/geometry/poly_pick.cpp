int PointsOnedge(){
	int res = 0;
	for(int i=0;i<N;i++)
		res += __gcd(abs(int(s[(i+1)%N].x-s[i].x)), abs(int(s[(i+1)%N].y-s[i].y)));
	return res;
}
int PointsInside(){
	return int(Area()) + 1 - PointsOnedge()/2;
}
