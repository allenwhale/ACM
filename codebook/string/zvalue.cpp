vector<int> z_value(string s){
	int len = s.size();
	vector<int> z(0, len);
	int l=0, r=0;
	z[0] = len;
	for(int i=1,j;i<len;z[i]=j,i++){
		j = max(min(z[i-l], r-i), 0);
		while(i+j<len&&s[i+j]==s[j])j++;
		if(i+z[i]>r)r=(l=i)+z[i];
	}
	return z;
}
