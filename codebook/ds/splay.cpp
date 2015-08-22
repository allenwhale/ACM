#include <bits/stdc++.h>
using namespace std;
template <class T>
class SplayTree{
public:
    class Node{
    public:
        Node *L, *R, *P;
        T val;
        int sz;
        Node(const T &rhs=T()):
            L(NULL), R(NULL), P(NULL), val(rhs), sz(1){}
        void Up(){
            sz = 1 + NodeSize(L) + NodeSize(R);
        }
    };
    static int NodeSize(Node *rhs){
        return rhs?rhs->sz:0;
    }
    Node *root;
    SplayTree(): root(NULL){}
    SplayTree(const T &rhs): root(new Node(rhs)){}
    ~SplayTree(){
    }
    void Free(){
        this->Free(this->root);
    }
    void Free(Node *rhs){
        if(!rhs) return;
        if(rhs->L)Free(rhs->L);
        if(rhs->R)Free(rhs->R);
        delete rhs;
        rhs = NULL;
    }
    int Size() const {
        return NodeSize(root);
    }
    void LeftRotate(Node *rhs){
        Node *x = rhs, *y = x->R;
        x->R = y->L;
        if(y->L)y->L->P = x;
        y->P = x->P;
        if(!x->P)root = y;
        else if(x->P->L == x)x->P->L = y;
        else x->P->R = y;
        y->L = x; x->P = y;
        x->Up(); y->Up();
    }
    void RightRotate(Node *rhs){
        Node *x = rhs, *y = x->L;
        x->L = y->R;
        if(y->R)y->R->P = x;
        y->P = x->P;
        if(!x->P)root = y;
        else if(x->P->L == x)x->P->L = y;
        else x->P->R = y;
        y->R = x; x->P = y;
        x->Up(); y->Up();
    }
    void Splay(Node *rhs){
        while(rhs->P != NULL){
            if(rhs->P->P == NULL){
                if(rhs->P->L == rhs)RightRotate(rhs->P);
                else LeftRotate(rhs->P);
            }else if(rhs->P->L == rhs && rhs->P->P->L == rhs->P){
                RightRotate(rhs->P->P);
                RightRotate(rhs->P);
            }else if(rhs->P->L == rhs && rhs->P->P->R == rhs->P){
                RightRotate(rhs->P);
                LeftRotate(rhs->P);
            }else if(rhs->P->R == rhs && rhs->P->P->R == rhs->P){
                LeftRotate(rhs->P->P);
                LeftRotate(rhs->P);
            }else{
                LeftRotate(rhs->P);
                RightRotate(rhs->P);
            }
        }
    }
    Node* FindMin() const {
        Node *tr = root;
        while(tr->L)tr = tr->L;
        return tr;
    }
    Node* FindMax() const {
        Node *tr = root;
        while(tr->R)tr = tr->R;
        return tr;
    }
    Node* Find(int k) const {
        Node *tr = root;
        while(tr){
            if(NodeSize(tr->L) >= k)
                tr = tr->L;
            else if(NodeSize(tr->L)+1 == k)
                break;
            else if(tr->R)
                k -= (NodeSize(tr->L)+1), tr = tr->R;
        }
        return tr;
    }
    void Merge(SplayTree rhs){
        if(rhs.Size() == 0)
            return;
        if(this->Size() == 0){
            *this = rhs;
            return;
        }
        this->Splay(this->FindMax());
        this->root->R = rhs.root;
        this->root->R->P = this->root;
        this->root->Up();
    }
    void Insert(const T &rhs){
        this->Merge(SplayTree(rhs));
    }
    void Split(int k, SplayTree &rhs1, SplayTree &rhs2){
        this->Splay(this->Find(k));
        rhs1.root = this->root;
        rhs2.root = this->root->R;
        rhs1.root->R = NULL;
        if(rhs2.root)rhs2.root->P = NULL;
        rhs1.root->Up();
    }
    void Delete(int k){
        this->Splay(this->Find(k));
        SplayTree a, b;
        a.root = this->root->L;
        b.root = this->root->R;
        if(a.root)a.root->P = NULL;
        if(b.root)b.root->P = NULL;
        delete this->root;
        a.Merge(b);
        this->root = a.root;
    }
    void Print() const {
        print(this->root);
        puts("");
    }
    void print(Node *rhs, int a=0) const {
        if(rhs == NULL)return;
        print(rhs->L, a+1);
        //for(int i=0;i<a;i++)cout << ' ';
        cout << rhs->val << " ";
        print(rhs->R, a+1);
    }
};
int main(){
    const int size = 10;
    const int time = 5000000;
    SplayTree<int> s[size];
    for(int i=0;i<time;i++){
        s[rand()%size].Insert(rand());
        int a,b;
        do{
            a = rand()%size;
            b = rand()%size;
        }while(a == b);
        s[a].Merge(s[b]);
        s[b].root = NULL;
    }
    for(int i=0;i<size;i++){
        printf("%d\n", i);
        //s[i].Print();
        s[i].Free();
    }
    return 0;
}

