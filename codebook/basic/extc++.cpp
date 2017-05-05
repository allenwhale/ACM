#include <bits/extc++.h>
#include <bits/stdc++.h>
using namespace std;
using namespace __gnu_pbds;
typedef __gnu_pbds::tree<int, null_type, 
		less<int>, rb_tree_tag, 
		tree_order_statistics_node_update> set_t;
typedef __gnu_pbds::priority_queue<int> heap_t;
cc_hash_table<int, int> umap;
int main(){
	set_t s;
	s.insert(10); s.insert(20);
	assert(*s.find_by_order(0) == 10);
	assert(*s.find_by_order(1) == 20);
	assert(s.order_of_key(10) == 0);
	assert(s.order_of_key(20) == 1);
	for(int x:s) printf("%d\n", x); // 10, 20
	s.erase(10);
	assert(*s.find_by_order(0) == 20);
	heap_t ha, hb;
	ha.clear(); hb.clear();
	ha.push(10); ha.push(20);
	hb.push(30); hb.push(40);
	assert(ha.top() == 20 && hb.top() == 40);
	ha.join(hb);
	assert(ha.top() == 40);
	ha.pop();
	for(int x:ha) printf("%d\n", x); //10, 20, 30
}
