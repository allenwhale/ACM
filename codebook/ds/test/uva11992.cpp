#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;
#define MAXN 1000010
#define SQR(x) ((x) * (x))
namespace KD {
    int degree = 2;
	const double alpha = 0.75;
	typedef long long T;
	struct Point {
		vector<T> d;
		int index;
		Point() {}
		Point(const vector<T> &_d, int _i=-1): d(_d), index(_i) {}
		T distance(const Point &rhs) const {
			T res = 0;
			for(int i=0;i<degree;i++)
				res += SQR(d[i] - rhs.d[i]);
			return res;
		}
		bool operator == (const Point &rhs) const {
			return index == rhs.index;
		}
		bool operator != (const Point &rhs) const {
			return !(index == rhs.index);
		}
        bool operator <= (const Point &rhs) const {
            bool res = true;
            for(int i=0;i<degree;i++)
                res &= d[i] <= rhs.d[i];
            return res;
        }
	};
	ostream& operator << (ostream &o, const Point &rhs) {
		o << "<" << rhs.index << ", (";
		for(int i=0;i<(int)rhs.d.size();i++){
			if(i) o << ", ";
			o << rhs.d[i];
		}
		o << ")>";
		return o;
	}
	bool cmp(int idx, const Point &a, const Point &b) {
        for(int i=0;i<degree;i++){
            if(a.d[(i + idx) % degree] != b.d[(i + idx) % degree])
                return a.d[(i + idx) % degree] < b.d[(i + idx) % degree];
        }
        return a.index < b.index;
    }
	struct qNode {
		Point p;
		T dis;
		qNode(const Point &_p=Point(), T _d=0):p(_p), dis(_d) {}
		bool operator < (const qNode &rhs) const {
			if(dis != rhs.dis) return dis < rhs.dis;
			return p.index < rhs.p.index;
		}
	};
    struct Node;
    Node *nil;
	struct Node {
		Point p, minNode, maxNode;
		Node *L, *R;
		int size, d;
        int data, minData, maxData, sumData;
        int lazySet, lazyAdd;
		void pull(){
            push();
            if(L != nil)L->push();
            if(R != nil)R->push();
            size = L->size + R->size + 1;
            minNode = maxNode = p;
            minData = maxData = sumData = data;
            if(L != nil){
                for(int i=0;i<KD::degree;i++){
                    minNode.d[i] = min(minNode.d[i], L->minNode.d[i]);
                    maxNode.d[i] = max(maxNode.d[i], L->maxNode.d[i]);
                }
                minData = min(minData, L->minData);
                maxData = max(maxData, L->maxData);
                sumData += L->sumData;
            }
            if(R != nil){
                for(int i=0;i<KD::degree;i++){
                    minNode.d[i] = min(minNode.d[i], R->minNode.d[i]);
                    maxNode.d[i] = max(maxNode.d[i], R->maxNode.d[i]);
                }
                minData = min(minData, R->minData);
                maxData = max(maxData, R->maxData);
                sumData += R->sumData;
            }
        }
        void push(){
            if(lazySet){
                data = minData = maxData = lazySet;
                sumData = lazySet * size;
                if(L != nil){
                    L->lazySet = lazySet;
                    L->lazyAdd = 0;
                }
                if(R != nil){
                    R->lazySet = lazySet;
                    R->lazyAdd = 0;
                }
                lazySet = 0;
            }
            if(lazyAdd){
                data += lazyAdd;
                minData += lazyAdd;
                maxData += lazyAdd;
                sumData += lazyAdd * size;
                if(L != nil){
                    L->lazyAdd += lazyAdd;
                }
                if(R != nil){
                    R->lazyAdd += lazyAdd;
                }
                lazyAdd = 0;
            }
        }
		bool isbad(){
			return max(L->size , R->size) > alpha * size;
		}
        void Print(int d=0) {
            if(this == nil) return;
            push();
            R->Print(d + 1);
            for(int i=0;i<d;i++) printf(" ");
            cout << this->p << " " << this->minData << " " << this->maxData << " " << this->sumData << " " << this->data << endl;
            L->Print(d + 1);
        }
        bool range_include(const Point &L, const Point &R) {
            for(int i=0;i<degree;i++){
                if(!(L.d[i] <= maxNode.d[i] && minNode.d[i] <= R.d[i]))
                    return false;
            }
            return true;
        }
        bool range_in_range(const Point &L, const Point &R) {
            for(int i=0;i<degree;i++){
                if(!(L.d[i] <= minNode.d[i] && maxNode.d[i] <= R.d[i]))
                    return false;
            }
            return true;
        }
        bool point_in_range(const Point &L, const Point &R) {
            for(int i=0;i<degree;i++){
                if(!(L.d[i] <= p.d[i] && p.d[i] <= R.d[i]))
                    return false;
            }
            return true;
        }
	}pool[MAXN], *tail, *recycle[MAXN];
	int rc_cnt;
	void Init() {
		tail = pool;
		nil = tail++;
		nil->L = nil->R = nil;
		nil->size = 0;
		rc_cnt = 0;
	}
	Node *New(const Point &p, int d, int data=0) {
		Node *n;
		if(rc_cnt) n = recycle[--rc_cnt];
		else n = tail++;
		n->L = n->R = nil;
		n->size = 1;
		n->p = p;
		n->d = d;
        n->data = n->minData = n->maxData = data;
        n->lazyAdd = n->lazySet = 0;
		return n;
	}
    void Free(Node *tr){ recycle[rc_cnt++] = tr; }
	Node *Build(vector<Point>::iterator p, int l, int r, int d) {
		if(l == r) return nil;
		int mid = (l + r) >> 1;
		nth_element(p + l, p + mid, p + r, bind(cmp, d, _1, _2));
		Node *n = New(p[mid], d);
		n->L = Build(p, l, mid, (d + 1) % degree);
		n->R = Build(p, mid + 1, r, (d + 1) % degree);
		n->pull();
		return n;
	}
	void GetTree(Node *tr, vector<Point> &v) {
		if(tr == nil) return;
		GetTree(tr->L, v);
		v.push_back(tr->p);
        Free(tr);
		GetTree(tr->R, v);
	}
	Node *Rebuild(Node *tr) {
		vector<Point> v;
		GetTree(tr, v);
		return Build(v.begin(), 0, v.size(), 0);
	}
	Node **Insert(Node *&tr, const Point &p, int d) {
		if(tr == nil) {
			tr = New(p, d);
			return &nil;
		}
		Node **res;
		if(cmp(d, p, tr->p)) res = Insert(tr->L, p, (d + 1) % degree);
		else res = Insert(tr->R, p, (d + 1) % degree);
		tr->pull();
		if(tr->isbad())res = &tr;
		return res;
	}
	void Insert(Node *&tr, const Point &p) {
		Node **bad = Insert(tr, p, 0);
		if(bad != &nil) *bad = Rebuild(*bad);
	}
	Node *GetMin(Node *tr, int d) {
		if(tr->d == d){
			if(tr->L != nil) return GetMin(tr->L, d);
			else return tr;
		} else {
			Node *res = tr, *tmp;
			if(tr->L != nil) {
				tmp = GetMin(tr->L, d);
				if(cmp(d, tmp->p, res->p))
					res = tmp;
			}
			if(tr->R != nil) {
				tmp = GetMin(tr->R, d);
				if(cmp(d, tmp->p, res->p))
					res = tmp;
			}
			return res;
		}
	}
	void Delete(Node *&tr, const Point &p) {
		if(tr == nil) return;
		tr->size--;
		if(tr->p == p) {
			if(tr->R != nil) {
				Node *mn = GetMin(tr->R, tr->d);
				Point p = mn->p;
				Delete(tr->R, mn->p);
				tr->p = p;
			} else if(tr->L != nil) {
				Node *mn = GetMin(tr->L, tr->d);
				Point p = mn->p;
				Delete(tr->L, mn->p);
				tr->p = p;
				tr->R = tr->L;
				tr->L = nil;
			} else {
                Free(tr);
				tr = nil;
			}
		} else if(cmp(tr->d, p, tr->p)) Delete(tr->L, p);
		else Delete(tr->R, p);
	}
	priority_queue<qNode> pq;
	void Search(Node *tr, const Point &p, int d, int m) {
		if(tr == nil) return;
		if(cmp(d, p, tr->p)) {
			Search(tr->L, p, (d + 1) % degree, m);
			if((int)pq.size() < m) {
                if(p != tr->p)
                    pq.push(qNode(tr->p, p.distance(tr->p)));
				Search(tr->R, p, (d + 1) % degree, m);
			} else {
				if(pq.size() && p.distance(tr->p) <= pq.top().dis && p != tr->p) {
					pq.push(qNode(tr->p, p.distance(tr->p)));
					pq.pop();
				}
				if(pq.size() && SQR(tr->p.d[d] - p.d[d]) <= pq.top().dis)
					Search(tr->R, p, (d + 1) % degree, m);
			}
		} else {
			Search(tr->R, p, (d + 1) % degree, m);
			if((int)pq.size() < m) {
                if(p != tr->p)
                    pq.push(qNode(tr->p, p.distance(tr->p)));
				Search(tr->L, p, (d + 1) % degree, m);
			} else {
				if(pq.size() && p.distance(tr->p) <= pq.top().dis && p != tr->p) {
					pq.push(qNode(tr->p, p.distance(tr->p)));
					pq.pop();
				}
				if(pq.size() && SQR(tr->p.d[d] - p.d[d]) <= pq.top().dis)
					Search(tr->L, p, (d + 1) % degree, m);
			}
		}
	}
	void Search(Node *tr, const Point &p, int m) {
		while(pq.size()) pq.pop();
		Search(tr, p, 0, m);
	}
    void Update(Node *tr, const Point &p, int d, int data) {
        tr->push();
        if(tr->p == p){
            tr->data = data;
            tr->pull();
            return;
        }
        if(cmp(d, p, tr->p))
            Update(tr->L, p, (d + 1) % degree, data);
        else
            Update(tr->R, p, (d + 1) % degree, data);
        tr->pull();
    }
    void UpdateRange(Node *tr, const Point &L, const Point &R, int op, int data) {
        tr->push();
        if(tr->range_in_range(L, R)){
            if(op == 0){ // set
                tr->lazySet = data;
            }else if(op == 1){ //add
                tr->lazyAdd += data;
            }
            return;
        }
        if(tr->point_in_range(L, R)){
            if(op == 0){ // set
                tr->data = data;
            }else if(op == 1){ //add
                tr->data += data;
            }
        }
        if(tr->L != nil && tr->L->range_include(L, R))
            UpdateRange(tr->L, L, R, op, data);
        if(tr->R != nil && tr->R->range_include(L, R))
            UpdateRange(tr->R, L, R, op, data);
        tr->pull();
    }
    pair<int, int> Query(Node *tr, const Point &L, const Point &R){
        tr->push();
        if(tr->range_in_range(L, R)){
            return {tr->minData, tr->maxData};
        }
        pair<int, int> res = {INT_MAX, INT_MIN};
        if(tr->point_in_range(L, R)){
            res.first = min(res.first, tr->data);
            res.second = max(res.second, tr->data);
        }
        if(tr->L != nil && tr->L->range_include(L, R)){
            auto childRes = Query(tr->L, L, R);
            res.first = min(res.first, childRes.first);
            res.second = max(res.second, childRes.second);
        }
        if(tr->R != nil && tr->R->range_include(L, R)){
            auto childRes = Query(tr->R, L, R);
            res.first = min(res.first, childRes.first);
            res.second = max(res.second, childRes.second);
        }
        return res;
    }
    tuple<int, int, int> QueryRange(Node *tr, const Point &L, const Point &R) {
        tr->push();
        if(tr->range_in_range(L, R)){
            return make_tuple(tr->minData, tr->maxData, tr->sumData);
        }
        tuple<int, int, int> res = make_tuple(INT_MAX, INT_MIN, 0);
        if(tr->point_in_range(L, R)){
            get<0>(res) = min(get<0>(res), tr->data);
            get<1>(res) = max(get<1>(res), tr->data);
            get<2>(res) += tr->data;
        }
        if(tr->L != nil && tr->L->range_include(L, R)){
            auto childRes = QueryRange(tr->L, L, R);
            get<0>(res) = min(get<0>(res), get<0>(childRes));
            get<1>(res) = max(get<1>(res), get<1>(childRes));
            get<2>(res) += get<2>(childRes);
        }
        if(tr->R != nil && tr->R->range_include(L, R)){
            auto childRes = QueryRange(tr->R, L, R);
            get<0>(res) = min(get<0>(res), get<0>(childRes));
            get<1>(res) = max(get<1>(res), get<1>(childRes));
            get<2>(res) += get<2>(childRes);
        }
        return res;
    }
	void GetRange(Node *tr, vector<Point> &v, const Point &p1, const Point &p2) {
		if(tr == nil) return;
        if(p1 <= tr->p && tr->p <= p2) v.push_back(tr->p);
        if(p1.d[tr->d] <= tr->p.d[tr->d]) GetRange(tr->L, v, p1, p2);
        if(tr->p.d[tr->d] <= p2.d[tr->d]) GetRange(tr->R, v, p1, p2);
	}
};

int main(){
    KD::degree = 2;
    KD::Init();
    int R, C, M;
    scanf("%d%d%d", &R, &C, &M);
    vector<KD::Point> p(R * C);
    for(int i=0;i<R;i++){
        for(int j=0;j<C;j++){
            p[i * C + j] = KD::Point({i, j}, i * C + j);
        }
    }
    KD::Node *root = KD::Build(p.begin(), 0, p.size(), 0);
    while(M--){
        int op;
        KD::T x1, y1, x2, y2;
        scanf("%d%lld%lld%lld%lld", &op, &x1, &y1, &x2, &y2);
        x1--, x2--, y1--, y2--;
        if(op == 1){ //add
            int v;
            scanf("%d", &v);
            KD::UpdateRange(root, KD::Point({x1, y1}), KD::Point({x2, y2}), 1, v);
        }else if(op == 2){ //set
            int v;
            scanf("%d", &v);
            KD::UpdateRange(root, KD::Point({x1, y1}), KD::Point({x2, y2}), 0, v);
        }else if(op == 3){ //query
            auto ans = KD::QueryRange(root, KD::Point({x1, y1}), KD::Point({x2, y2}));
            printf("%d %d %d\n", get<2>(ans), get<0>(ans), get<1>(ans));
        }
        //root->Print();
    }
}


