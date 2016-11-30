#include <bits/stdc++.h>
using namespace std;
unsigned Rand(){
	static unsigned x=20150118;
	return x = x*0xdefaced+1;
}
class Treap{
	public:
		Treap *L, *R;
		int val, sum, sz;
		int cov, rev;
		unsigned pri;
		int tmx, lmx, rmx;
		Treap(int rhs=0): L(NULL), R(NULL), val(rhs), sum(rhs), sz(1), cov(0), rev(0), pri(Rand()), tmx(rhs), lmx(rhs), rmx(rhs){}
		void Down(){
			if(rev){
				rev = 0;
				swap(L, R);
				if(L){
					L->rev ^= 1;
					swap(L->lmx, L->rmx);
				}
				if(R){
					R->rev ^= 1;
					swap(R->lmx, R->rmx);
				}
			}
			if(cov){
				cov = 0;
				if(L){
					L->val = val; L->sum = val*L->sz;
					if(val >= 0)
						L->tmx = L->lmx = L->rmx = L->sum;
					else L->tmx = L->lmx = L->rmx = val;
					L->cov = 1;
				}
				if(R){
					R->val = val; R->sum = val*R->sz;
					if(val >= 0)
						R->tmx = R->lmx = R->rmx = R->sum;
					else R->tmx = R->lmx = R->rmx = val;
					R->cov = 1;
				}
			}
		}
		void Up(){
			tmx = lmx = rmx = sum = val;
			sz = 1;
			if(L)sum += L->sum, sz += L->sz;
			if(R)sum += R->sum, sz += R->sz;
			if(L){
				lmx = L->lmx;
				lmx = max(lmx, L->sum+val);
				if(R) lmx = max(lmx, L->sum+val+R->lmx);
				tmx = max(tmx, L->tmx);
				tmx = max(tmx, L->rmx+val);
			}else if(R){
				lmx = max(lmx, val+R->lmx);
			}
			if(R){
				rmx = R->rmx;
				rmx = max(rmx, R->sum+val);
				if(L) rmx = max(rmx, R->sum+val+L->rmx);
				tmx = max(tmx, R->tmx);
				tmx = max(tmx, R->lmx+val);
			}else if(L){
				rmx = max(rmx, L->rmx+val);
			}
			if(L && R){
				tmx = max(tmx, L->rmx+val+R->lmx);
			}
		}
};
int size(Treap *rhs){
	return rhs?rhs->sz:0;
}
Treap* Merge(Treap *a,Treap *b)
{
	if(!a || !b) return a?a:b;
	if(a->pri > b->pri){
		a->Down();
		a->R = Merge(a->R,b);
		a->Up();
		return a;
	}else{
		b->Down();
		b->L = Merge(a,b->L);
		b->Up();
		return b;
	}
}
void Split(Treap *tr,Treap *&a,Treap *&b,int k){
	if(k == 0) a = NULL, b = tr;
	else if(k == size(tr)) a = tr, b = NULL;
	else{
		tr->Down();
		if(size(tr->L) >= k){
			b = tr;
			Split(tr->L, a, b->L, k);
			b->Up();
		}else{
			a = tr;
			Split(tr->R, a->R, b, k-size(tr->L)-1);
			a->Up();
		}
	}
}
void Change(Treap *&tr, int pos, int k, int l){
	Treap *a, *b, *c, *d;
	Split(tr, a, b, pos-1);
	Split(b, c, d, k);
	c->cov = 1;
	c->val = l;
	c->rev = 0;
	c->sum = l*size(c);
	if(l >= 0) c->tmx = c->rmx = c->lmx = c->sum;
	else c->tmx = c->rmx = c->lmx = l;
	tr = Merge(a, Merge(c, d));
}
void Reverse(Treap *&tr, int pos, int k){
	Treap *a, *b, *c, *d;
	Split(tr, a, b, pos-1);
	Split(b, c, d, k);
	c->rev ^= 1;
	swap(c->lmx, c->rmx);
	tr = Merge(a, Merge(c, d));
}
void print(Treap *tr){
	if(!tr)return;
	tr->Down();
	if(tr->L)print(tr->L);
	printf("%d ", tr->val);
	if(tr->R)print(tr->R);
}
int main(){
	Treap *root = NULL, *a, *b ,*c, *d;
	for(int i=0;i<10;i++){
		root = Merge(root, new Treap(i));
	}
	print(root); puts(""); Split(root, a, b, 5); print(a); puts(""); print(b); puts(""); root = Merge(a, b); Reverse(root, 5, 3); print(root); puts(""); Change(root, 3, 4, 10); print(root); puts("");

	return 0;
}
