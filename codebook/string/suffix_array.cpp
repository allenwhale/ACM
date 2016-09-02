vector<int> SuffixArray(const string& s){
	int len = s.size();
	int alpha = 256;
	vector<int> cnt(alpha), rank(len), sa(len), tsa(len), tp[2];
	tp[0] = tp[1] = vector<int>(0, len);
	for(int i=0;i<len;i++)cnt[s[i] + 1]++;
	for(int i=1;i<alpha;i++)cnt[i] += cnt[i - 1];
	for(int i=0;i<len;i++)rank[i] = cnt[s[i]];
	for(int i=1;i<len;i<<=1){
		for(int j=0;j<len;j++){
			if(i + j >= len)tp[1][j] = 0;
			else tp[1][j] = rank[i + j]+1;
			tp[0][j] = rank[j];
		}
		fill(cnt.begin(), cnt.end(), 0);
		for(int j=0;j<len;j++)cnt[tp[1][j] + 1]++;
		for(int j=1;j<alpha;j++)cnt[j] += cnt[j - 1];
		for(int j=0;j<len;j++)tsa[cnt[tp[1][j]]++] = j;
		fill(cnt.begin(), cnt.end(), 0);
		for(int j=0;j<len;j++)cnt[tp[0][j] + 1]++;
		for(int j=1;j<alpha;j++)cnt[j] += cnt[j - 1];
		for(int j=0;j<len;j++)sa[cnt[tp[0][tsa[j]]]++] = tsa[j];
		rank[sa[0]] = 0;
		for(int j=1;j<len;j++){
			if(tp[0][sa[j]] == tp[0][sa[j - 1]] && tp[1][sa[j]] == tp[1][sa[j - 1]]){
				rank[sa[j]] = rank[sa[j - 1]];
			}else rank[sa[j]] = j;
		}
	}
	return sa;
}
