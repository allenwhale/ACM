#include <bits/stdc++.h>
using namespace std;
#define MAXN 5010
#define DEGREE 2
#define SQR(x) ((x)*(x))
namespace KD {
    const double alpha = 0.75;
    typedef long long T;
    struct Point {
        vector<T> d;
        int index;
        Point() {}
        Point(const vector<T> &_d, int _i): d(_d), index(_i) {}
        T distance(const Point &rhs) const {
            T res = 0;
            for(int i=0;i<DEGREE;i++)
                res += SQR(d[i] - rhs.d[i]);
            return res;
        }
        bool operator == (const Point &rhs) const {
            return index == rhs.index;
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
    inline std::function<bool (const Point&, const Point&)>  cmp(int idx) {
        return [&](const Point &a, const Point &b){
            for(int i=0;i<DEGREE;i++)
                if(a.d[(i + idx) % DEGREE] != b.d[(i + idx) % DEGREE])
                    return a.d[(i + idx) % DEGREE] < b.d[(i + idx) % DEGREE];
            return a.index < b.index;
        };
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
    Node *Build(vector<Point>::iterator p, int l, int r, int d) {
        if(l == r) return nil;
        int mid = (l + r) >> 1;
        nth_element(p + l, p + mid, p + r, cmp(d));
        Node *n = New(p[mid], d);
        n->L = Build(p, l, mid, (d + 1) % DEGREE);
        n->R = Build(p, mid + 1, r, (d + 1) % DEGREE);
        n->pull();
        return n;
    }
    void GetTree(Node *tr, vector<Point> &v) {
        if(tr == nil) return;
        GetTree(tr->L, v);
        v.push_back(tr->p);
        recycle[rc_cnt++] = tr;
        GetTree(tr->R, v);
    }
    Node *Rebuild(Node *tr) {
        vector<Point> v;
        GetTree(tr, v);
        return Build(v.begin(), 0, v.size(), 0);
    }
    Node **Insert(Node *&tr, const Point &p, int d, Node **&bad) {
        if(tr == nil) {
            tr = New(p, d);
            return &nil;
        }
        Node **res;
        if(cmp(d)(p, tr->p)) res = Insert(tr->L, p, (d + 1) % DEGREE, bad);
        else res = Insert(tr->R, p, (d + 1) % DEGREE, bad);
        tr->pull();
        if(tr->isbad())res = &tr;
        return res;
    }
    void Insert(Node *&tr, const Point &p) {
        Node **bad = Insert(tr, p, 0, bad);
        if(bad != &nil) *bad = Rebuild(*bad);
    }
    Node **GetMin(Node *&tr, int d) {
        if(tr->d == d){
            if(tr->L != nil) return GetMin(tr->L, d);
            else return &tr;
        } else {
            Node **res = &tr, **tmp;
            if(tr->L != nil) {
                tmp = GetMin(tr->L, d);
                if(cmp(d)((*tmp)->p, (*res)->p))
                    res = tmp;
            }
            if(tr->R != nil) {
                tmp = GetMin(tr->R, d);
                if(cmp(d)((*tmp)->p, (*res)->p))
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
                Node **mn = GetMin(tr->R, tr->d);
                Point p = (*mn)->p;
                Delete(tr->R, (*mn)->p);
                tr->p = p;
            } else if(tr->L != nil) {
                Node **mn = GetMin(tr->L, tr->d);
                Point p = (*mn)->p;
                Delete(tr->L, (*mn)->p);
                tr->p = p;
                tr->R = tr->L;
                tr->L = nil;
            } else {
                recycle[rc_cnt++] = tr;
                tr = nil;
            }
        } else if(cmp(tr->d)(p, tr->p)) Delete(tr->L, p);
        else Delete(tr->R, p);
    }
    priority_queue<qNode> pq;
    void Search(Node *tr, const Point &p, int d, int m) {
        if(tr == nil) return;
        if(cmp(d)(p, tr->p)) {
            Search(tr->L, p, (d + 1) % DEGREE, m);
            if((int)pq.size() < m) {
                pq.push(qNode(tr->p, p.distance(tr->p)));
                Search(tr->R, p, (d + 1) % DEGREE, m);
            } else {
                if(p.distance(tr->p) <= pq.top().dis) {
                    pq.push(qNode(tr->p, p.distance(tr->p)));
                    pq.pop();
                }
                if(SQR(tr->p.d[d] - p.d[d]) <= pq.top().dis)
                    Search(tr->R, p, (d + 1) % DEGREE, m);
            }
        } else {
            Search(tr->R, p, (d + 1) % DEGREE, m);
            if((int)pq.size() < m) {
                pq.push(qNode(tr->p, p.distance(tr->p)));
                Search(tr->L, p, (d + 1) % DEGREE, m);
            } else {
                if(p.distance(tr->p) <= pq.top().dis) {
                    pq.push(qNode(tr->p, p.distance(tr->p)));
                    pq.pop();
                }
                if(SQR(tr->p.d[d] - p.d[d]) <= pq.top().dis)
                    Search(tr->L, p, (d + 1) % DEGREE, m);
            }
        }
    }
    void Search(Node *tr, const Point &p, int m) {
        while(pq.size()) pq.pop();
        Search(tr, p, 0, m);
    }
    void GetRange(Node *tr, vector<Point> &v, int x1, int x2, int y1, int y2) {
        if(tr == nil) return;
        if(x1 <= tr->p.d[0] && tr->p.d[0] <= x2 && y1 <= tr->p.d[1] && tr->p.d[1] <= y2)
            v.push_back(tr->p);
        if(tr->d ? (y1 <= tr->p.d[1]) : (x1 <= tr->p.d[0]))
            GetRange(tr->L, v, x1, x2, y1, y2);
        if(tr->d ? (y2 >= tr->p.d[1]) : (x2 >= tr->p.d[0]))
            GetRange(tr->R, v, x1, x2, y1, y2);
    }
};

int main(){
    srand(time(0));
    KD::Init();
    vector<KD::Point> v;
    for(int i=0;i<10;i++)
        v.push_back({{rand()%100, rand()%100}, i});
    puts("---origin---");
    for(KD::Point p: v)
        cout << p << endl;
    KD::Node *root = KD::Build(v.begin(), 0, v.size(), 0);
    vector<KD::Point> u;
    KD::GetRange(root, u, 0, 100, 0, 100);
    puts("---range---");
    for(KD::Point p: u)
        cout << p << endl;
    //KD::Print(root);
    return 0;
}

