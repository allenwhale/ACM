Point MassCenter(){
	if(cmp(Area()) == 0)return nilPoint;
	Point res;
	for(int i=0;i<N;i++)
		res = res + (s[i] + s[(i+1)%N]) * s[i].Cross(s[(i+1)%N]);
	return res / Area() / 6.0;
}
