#include <bits/stdc++.h>
using namespace std;
using namespace std::placeholders;
#define MAXN 100010
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
	struct Node {
		Point p;
		Node *L, *R;
		int size, d;
		void pull(){ size = L->size + R->size + 1; }
		bool isbad(){
			return max(L->size , R->size) > alpha * size;
		}
	}*nil, pool[MAXN], *tail, *recycle[MAXN];
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
		cout << tr->p << endl;
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
	void GetRange(Node *tr, vector<Point> &v, const Point &p1, const Point &p2) {
		if(tr == nil) return;
        if(p1 <= tr->p && tr->p <= p2) v.push_back(tr->p);
        if(p1.d[tr->d] <= tr->p.d[tr->d]) GetRange(tr->L, v, p1, p2);
        if(tr->p.d[tr->d] <= p2.d[tr->d]) GetRange(tr->R, v, p1, p2);
	}
};

void Solve(){
    int N;
    scanf("%d", &N);
    KD::degree = 2;
    KD::Init();
    vector<KD::Point> p(N);
    for(int i=0;i<N;i++){
        KD::T x, y;
        scanf("%lld%lld", &x, &y);
        p[i] = KD::Point({x, y}, i);
    }
    KD::Node *root = KD::Build(p.begin(), 0, p.size(), 0);
    vector<KD::T> ans(N);
    for(auto x : p){
        KD::Search(root, x, 1);
        ans[x.index] = KD::pq.top().dis;
    }
    for(auto x : ans)
        printf("%lld\n", x);
}

int main(){
    int T;
    scanf("%d", &T);
    while(T--)Solve();
	return 0;
}


