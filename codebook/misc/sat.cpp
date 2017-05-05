/*
	IPJ_SAT_solver version 2
	two_lit 1UIP
*/
class SAT_solver{
	public:
	SAT_solver();
	SAT_solver(int _lit,int _cl);
	void init(int _lit,int _cl);
	void set_cl(int number,std::vector< int > &CL);
	bool solve();
	std::vector< int > get_ans();
	private:
	int lit,cl;
	std::vector< std::vector< int > > cls, watch_lit, dcp;
	std::vector< int > ans, ass, ass_by, ass_time;
	std::vector< std::pair< int , int > > watch;
	std::queue< std::pair< int , int > > unit_cl;
	std::vector< int > make_dicision_list();
	bool check(int);
	void set_two_lit();
	int find_new_lit(int);
	int assign_lit(int);
	void add_cls(int);
	std::vector<int> first_uip(int , int);
};
SAT_solver::SAT_solver(){
	lit=0;cl=0;
	cls.clear();
	ans.clear();
}
SAT_solver::SAT_solver(int _lit,int _cl){
	lit=_lit;cl=_cl;
	cls.clear();
	ans.clear();
	for(int i = 0 ; i < cl ; ++i){
		cls.push_back(std::vector<int>(0));
	}	
}
void SAT_solver::init(int _lit,int _cl){
	lit=_lit;cl=_cl;
	cls.clear();
	ans.clear();
	for(int i = 0 ; i < cl ; ++i){
		cls.push_back(std::vector< int >(0));
	}	
}
void SAT_solver::set_cl(int number,std::vector< int > &CL){
	if(number < cl){
		cls[number]=CL;
	}else{
		printf("CL_number error\n");
	}
}
std::vector< int > SAT_solver::get_ans(){
	return ans;
}

// make dicision list each literal in clause gain 2 ^ -|cl|,Jeroslaw-Wang Score
std::vector<int> SAT_solver::make_dicision_list(){
	std::vector< int > ret;
	std::vector< std::pair< double , int> > lit_weight;
	for(int i = 0 ; i < lit ; ++i){
		lit_weight.push_back(std::pair< double , int >(0 , i + 1));
	}
	for(int i = 0 ; i < cl ; ++i){
		double score = 1.0 ;
		for(int j = 0 ; j < int(cls[i].size()) ; ++j)score /= 2.0;
		for(int j = 0 ; j < int(cls[i].size()) ; ++j){
			lit_weight[std::abs(cls[i][j]) - 1].first += score;
		}
	}
	std::sort(lit_weight.begin(),lit_weight.end());
	for(int i = 0; i < lit ; ++i)
		ret.push_back(lit_weight[lit - 1 - i].second);	
	return ret;
}
// check literal == assign to false
bool SAT_solver::check(int _lit){
	if(!_lit)return 0;
	int now = std::abs(_lit) , pn = _lit > 0 ? 1 : 2; 
	if( ass[now] == pn || !ass[now])return 1;
	return 0;
}
// find not assign to false , not watch literal in clause 
int SAT_solver::find_new_lit(int _cl){
	for(int i = 0 ; i < int(cls[_cl].size()) ; ++i){
		if(cls[_cl][i] == watch[_cl].first || cls[_cl][i] == watch[_cl].second)continue;
		if(check(cls[_cl][i]))return cls[_cl][i];
	}
	return 0;
}
// init two lit  keep watch list  lit + |lit|
void SAT_solver::set_two_lit(){
	for(int i = 0 ; i <= 2 * lit ; ++i){
		watch_lit.push_back(std::vector< int >(0));
	}
	watch.clear();
	for(int i = 0 ; i < cl ; ++i){
		add_cls(i);
	}
}
// add new clause
void SAT_solver::add_cls(int _cl){
	watch.push_back({0,0});
	watch[_cl].first = find_new_lit(_cl);
	if(watch[_cl].first)watch_lit[watch[_cl].first + lit].push_back(_cl);
	watch[_cl].second = find_new_lit(_cl);
	if(watch[_cl].second == 0)unit_cl.push({watch[_cl].first , _cl});
	else watch_lit[watch[_cl].second + lit].push_back(_cl);
}
// assign lit = true return -1 mean ok ,others are conflict clause
int SAT_solver::assign_lit(int _lit){
	int now = std::abs(_lit) , pn = _lit > 0 ? 1 : 2; 
	if(ass[now] == pn)return -1;
	else if(ass[now] == 0){
		ass[now] = pn;
		// move -_lit watching
		int r_lit = lit - _lit;
		for(int i = 0 ; i < int(watch_lit[r_lit].size()) ;++i){
			int _cl = watch_lit[r_lit][i];
			int new_w = find_new_lit(_cl);	
			if(!new_w && !check(watch[_cl].first) && !check(watch[_cl].second)){
				//no new watching literal and all false , conflict
				return _cl;
			}else if (new_w){
				//has new watching literal , move to back and pop
				if(watch[_cl].first == - _lit)watch[_cl].first = new_w;
				if(watch[_cl].second == - _lit)watch[_cl].second = new_w;
				watch_lit[new_w + lit].push_back(_cl);
				watch_lit[r_lit][i] = watch_lit[r_lit][watch_lit[r_lit].size() - 1];
				watch_lit[r_lit].pop_back();
				i--;
			}else{	
				//no new watching literal , unit clause add 
				if(watch[_cl].first == - _lit){
					if(!ass[std::abs(watch[_cl].second)])unit_cl.push({watch[_cl].second , _cl});
				}else{
					if(!ass[std::abs(watch[_cl].first)])unit_cl.push({watch[_cl].first , _cl});
				}
			}
		}
		return -1;
	}else{
		return ass_by[std::abs(_lit)];
	}
}
// get new clause by 1UIP
std::vector<int> SAT_solver::first_uip(int _cl , int dcps){
	std::vector< int > ret , check_list(lit + 1 , 0);
	std::queue< int > dcps_level;
	// get list from conflict clause and only one now-dicision-literal
	for(int i = 0 ; i < int(cls[_cl].size()) ; ++i){
		check_list[abs(cls[_cl][i])] = true;
		if(ass_time[std::abs(cls[_cl][i])] == -1)continue;
		if(ass_time[std::abs(cls[_cl][i])] != dcps)ret.push_back(cls[_cl][i]);
		else dcps_level.push(cls[_cl][i]);
	}
	//trans now dicision level to another
	while(dcps_level.size() > 1){
		int now = std::abs( dcps_level.front());
		// ass_by[now] == -1 mean this is dicision literal it can't find another literal to instead of itself
		if(ass_by[now] == -1 && ass_time[now]==dcps){
			dcps_level.push(dcps_level.front());
			dcps_level.pop();
			continue;
		}
		dcps_level.pop();
		for(int i = 0 ; i < int(cls[ass_by[now]].size()) ; ++i){
			if(check_list[abs(cls[ass_by[now]][i])] == true)continue;
			check_list[abs(cls[ass_by[now]][i])] = true;
			if(ass_time[std::abs(cls[ass_by[now]][i])] == -1)continue;
			if(ass_time[std::abs(cls[ass_by[now]][i])] != dcps)ret.push_back(cls[ass_by[now]][i]);
			else dcps_level.push(cls[ass_by[now]][i]);
		}
	}
	//and only one now-dicision-literal to new clause
	ret.push_back(dcps_level.front());
	return ret;
}
bool SAT_solver::solve(){
	ans.clear();
	std::vector< int > dicision = make_dicision_list();
	std::vector< int > undi(lit);
	ass_by.clear();
	ass_time.clear();
	ass_by = std::vector< int > (lit + 1 ,-2);
	ass_time = std::vector< int > (lit + 1 ,-2);
	ass = std::vector< int > (lit + 1 , 0);
	for(int i = 0 ; i < lit ; ++i) undi[dicision[i]] = i;
	/*for(int i = 0 ; i <= lit ; ++i){
		ass_by.push_back(-2);
		ass_time.push_back(-2);
	}*/
	for(int i = 0 ; i < cl ; ++i)
		if(!cls[i].size())return 0;
	set_two_lit();
	// ass_time = -1 mean it correct value
	while(!unit_cl.empty()){
		std::pair< int , int > now = unit_cl.front();
		unit_cl.pop();
		int now_p = std::abs(now.first);
		ass_by[now_p] = now.second;
		ass_time[now_p] = -1;
		//conflict
		if(assign_lit(now.first))return false;
	}

	int dic = 0, dcp_cnt = 0;
	while(dic >= 0 && dic < lit){
		//if it be assigned before  , skip
		if(ass[dicision[dic]]){
			dic++;
			continue;
		}
		//assign it to true create new dcp list
		unit_cl.push({dicision[dic],-1});
		dcp.push_back(std::vector< int >(0));

		while(!unit_cl.empty()){
			std::pair< int , int > uni = unit_cl.front();
			unit_cl.pop();
			// -1 mean correct value 
			int _lit = uni.first, _cl = uni.second;
			if(dcp_cnt != -1)dcp[dcp_cnt].push_back(_lit);
			ass_time[std::abs(_lit)] = dcp_cnt;
			ass_by[std::abs(_lit)] = _cl;
			int cf_cl = assign_lit(_lit);
			
			//conflict backtrack
			if(cf_cl != -1){
				// can't backtrack anymore
				if(dic == -1)return false;
				while(!unit_cl.empty())unit_cl.pop();
				
				std::vector<int> new_cl = first_uip(cf_cl , dcp_cnt);
				//find backtrack dicision , last one is now-dicision-literal
				int	back_to = -1;
				for(int i = 0 ; i < int(new_cl.size()) - 1 ; ++i){
					back_to = std::max(back_to , ass_time[std::abs(new_cl[i])]);
				}
				//back
				for( ; dcp_cnt > back_to ; --dcp_cnt){
					for(int i = 0 ; i < int(dcp[dcp_cnt].size()) ; ++i){
						int tmp = std::abs(dcp[dcp_cnt][i]);
						ass[tmp] = 0;
						ass_by[tmp] = -2;
						ass_time[tmp] = -2;
					}
					dcp[dcp_cnt].clear();
					dcp.pop_back();
				}
				if(new_cl.size() == 0)return false;
				cl++;
				cls.push_back(new_cl);
				add_cls(cl - 1);
				if(dcp_cnt != -1)dic = undi[dcp[dcp_cnt][0]];
				else dic = -1;
				//continue doing unit clause becase add this new clause we will get more unit clause in back_to dicision
			}
		}
		dcp_cnt++;
		dic++;
	}
	if(dic == lit){
		for(int i = 0 ; i < lit ; ++i){
			ans.push_back(ass[i + 1]==1 ? 1 : 0);
		}
		return true;
	}
	return false;	
}


/*
	This main function for kcw project
*/
int main(int argc,char ** args){
	if(argc != 2){
		printf("command error\n");
		return 0;
	}
	char in[256],out[256];
	sscanf(args[1],"%s",in);
	for(int i = 0 ; in[i] ; ++i){
		out[i]=in[i];
		if(in[i] == '.'){
			if(strcmp("cnf",in+i+1)){
				printf("FILE ERROR\n");
				return 0;
			}
			out[i+1]='s';
			out[i+2]='a';
			out[i+3]='t';
			out[i+4]='\0';
			break;
		}
	}
	freopen(in,"r",stdin);
	//freopen(out,"w",stdout);
	char p[8],cnf[8];
	int n,m;
	scanf("%s %s %d %d",p,cnf,&n,&m); 
	SAT_solver IPJ(n,m);
	for(int i = 0 ; i < m ; ++i){
		std::vector< int > tmp(0);
		int x;
		while(scanf("%d",&x) , x)tmp.push_back(x);	
		IPJ.set_cl(i,tmp);
	}
	if(IPJ.solve()){
		printf("s SATISFIABLE\n");
		std::vector< int > ans = IPJ.get_ans();
		printf("v");
		for(int i = 1 ; i <= n ; ++i){
			printf(" ");
			if(!ans[i-1])printf("-");
			printf("%d",i);
		}
		printf(" 0\n");
	}else{
		printf("s UNSATISFIABLE\n");
	}
}
