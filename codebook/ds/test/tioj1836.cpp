#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;
#define MAXN 27000
#define SQR(x) ((x) * (x))
namespace KD {
    const int degree = 2;
	const double alpha = 0.75;
	typedef long long T;
	struct Point {
        T d[2];
		long long index, data, gcdData;
		Point(): index(-1) {}
		Point(T x, T y, long long _i=-1): index(_i) {
            d[0] = x, d[1] = y;
        }
		T distance(const Point &rhs) const {
			T res = 0;
			for(int i=0;i<degree;i++)
				res += SQR(d[i] - rhs.d[i]);
			return res;
		}
		bool operator == (const Point &rhs) const {
			return index == rhs.index && rhs.d[0] == d[0] && rhs.d[1] == d[1];
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
		for(int i=0;i<degree;i++){
			if(i) o << ", ";
			o << rhs.d[i];
		}
		o << ")>";
		return o;
	}
	bool cmp(int idx, const Point &a, const Point &b) {
        if(a.d[idx] != b.d[idx])
            return a.d[idx] < b.d[idx];
        for(int i=0;i<degree;i++){
            if(a.d[i] != b.d[i]){
                return a.d[i] < b.d[i];
            }
        }
        return a.index < b.index;
    }
	struct qNode {
		Point p;
		T dis;
		qNode(const Point &_p=Point(), T _d=0):p(_p), dis(_d) {}
		bool operator < (const qNode &rhs) const {
			if(dis != rhs.dis) return dis < rhs.dis;
			return p.index <= rhs.p.index;
		}
	};
    struct Node;
    Node *nil;
	struct Node {
		Point p, minNode, maxNode;
		Node *L, *R;
		int size, d;
		void pull(){
            size = L->size + R->size + 1;
            minNode = maxNode = p;
            p.gcdData = p.data;
            if(L != nil){
                for(int i=0;i<KD::degree;i++){
                    minNode.d[i] = min(minNode.d[i], L->minNode.d[i]);
                    maxNode.d[i] = max(maxNode.d[i], L->maxNode.d[i]);
                }
                p.gcdData = __gcd(p.gcdData, L->p.gcdData);
            }
            if(R != nil){
                for(int i=0;i<KD::degree;i++){
                    minNode.d[i] = min(minNode.d[i], R->minNode.d[i]);
                    maxNode.d[i] = max(maxNode.d[i], R->maxNode.d[i]);
                }
                p.gcdData = __gcd(p.gcdData, R->p.gcdData);
            }
        }
		bool isbad(){
			return max(L->size , R->size) > alpha * size;
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
	void Print(Node *tr, int d=0) {
		if(tr == nil) return;
		Print(tr->R, d + 1);
		for(int i=0;i<d;i++) printf(" ");
		cout << tr->p << " " << tr->p.gcdData << " " << tr->p.data << endl;
		Print(tr->L, d + 1);
	}
	Node *New(const Point &p, int d) {
		Node *n;
		if(rc_cnt) n = recycle[--rc_cnt];
		else n = tail++;
		n->L = n->R = nil;
		n->size = 1;
		n->p = p;
		n->d = d;
		return n;
	}
    void Free(Node *tr){ recycle[rc_cnt++] = tr; }
	Node *Build(vector<Point>::iterator p, int l, int r, int d) {
		if(l == r) return nil;
		int mid = (l + r) >> 1;
		nth_element(p + l, p + mid, p + r, bind(cmp, d, _1, _2));
		Node *n = New(p[mid], d);
		n->L = Build(p, l, mid, (d ^ 1));
		n->R = Build(p, mid + 1, r, (d ^ 1));
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
        int d = tr->d;
		vector<Point> v;
        v.reserve(tr->size);
		GetTree(tr, v);
		return Build(v.begin(), 0, v.size(), d);
	}
	Node **Insert(Node *&tr, const Point &p, int d) {
		if(tr == nil) {
			tr = New(p, d);
            tr->pull();
			return &nil;
		}
		Node **res;
		if(cmp(d, p, tr->p)) res = Insert(tr->L, p, (d ^ 1));
		else res = Insert(tr->R, p, (d ^ 1));
		tr->pull();
		if(tr->isbad())res = &tr;
		return res;
	}
	void Insert(Node *&tr, const Point &p) {
		Node **bad = Insert(tr, p, 0);
		if(bad != &nil){
            *bad = Rebuild(*bad);
        }
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

    bool Update(Node *&tr, const Point &p, int d, long long data) {
        if(tr == nil){
            tr = New(p, d);
            tr->p.data = data;
            tr->pull();
            return 1;
        }
        if(tr->p == p){
            tr->p.data = data;
            tr->pull();
            return 0;
        }
        if(Update(cmp(d, p, tr->p) ? tr->L : tr->R, p, (d ^ 1), data)){
            if(!tr->isbad()){
                tr->pull();
                return 1;
            }
            tr = Rebuild(tr);
        }
        tr->pull();
        return 0;
    }
    long long Query(Node *tr, const Point &L, const Point &R){
        if(tr == nil){
            return 0;
        }
        if(tr->range_in_range(L, R)){
            return tr->p.gcdData;
        }
        long long res = 0;
        if(tr->point_in_range(L, R)){
            res = tr->p.data;
        }
        if(tr->L != nil && tr->L->range_include(L, R)){
            res = __gcd(res, Query(tr->L, L, R));
        }
        if(tr->R != nil && tr->R->range_include(L, R)){
            res = __gcd(res, Query(tr->R, L, R));
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
    long long R, C;
    int N;
    scanf("%lld%lld%d", &R, &C, &N);;
    KD::Init();
    KD::Node *root = KD::nil;
    unordered_set<KD::T> p;
    while(N--){
        int op;
        scanf("%d", &op);
        if(op == 1){
            long long x, y, v;
            scanf("%lld%lld%lld", &x, &y, &v);
            KD::Update(root, KD::Point(x, y, x * C + y), 0, v);
        }else if(op == 2){
            long long x1, y1, x2, y2;
            scanf("%lld%lld%lld%lld", &x1, &y1, &x2, &y2);
            printf("%lld\n", KD::Query(root, KD::Point({x1, y1}), KD::Point({x2, y2})));
        }
    }
	return 0;
}


