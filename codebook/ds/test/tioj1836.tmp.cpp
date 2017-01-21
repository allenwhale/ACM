#include<bits/stdc++.h>
using namespace std;
#define kd 2
struct point{
    int d[kd];
    point(int x,int y){
        d[0]=x,d[1]=y;
    }
    bool operator==(const point &p){
        return d[0]==p.d[0]&&d[1]==p.d[1];
    }
};
struct node{
    node *l,*r;
    point pid,mi,ma;
    long long data,gcd;
    int s;
    node(const point &p,long long d):l(0),r(0),pid(p),mi(p),ma(p),data(d),gcd(d),s(1){}
    inline void up(){
        s=1;
        mi=ma=pid;
        gcd=data;
        if(l){
            s+=l->s;
            for(int i=0;i<kd;++i){
                mi.d[i]=min(mi.d[i],l->mi.d[i]);
                ma.d[i]=max(ma.d[i],l->ma.d[i]);
            }
            gcd=__gcd(l->gcd,gcd);
        }
        if(r){
            s+=r->s;
            for(int i=0;i<kd;++i){
                mi.d[i]=min(mi.d[i],r->mi.d[i]);
                ma.d[i]=max(ma.d[i],r->ma.d[i]);
            }
            gcd=__gcd(r->gcd,gcd);
        }
    }
}*root;
vector<node*> A;
struct __cmp{
    int sort_id;
    inline bool operator()(const node*x,const node*y)const{
        return operator()(x->pid,y->pid);
    }
    inline bool operator()(const point &x,const point &y)const{
        if(x.d[sort_id]!=y.d[sort_id])
            return x.d[sort_id]<y.d[sort_id];
        for(int i=0;i<kd;++i){
            if(x.d[i]!=y.d[i])return x.d[i]<y.d[i];
        }
        return 0;
    }
}cmp;
node* build(int k,int l,int r){
    if(l>r)return 0;
    if(k==kd)k=0;
    int mid=(l+r)/2;
    cmp.sort_id=k;
    nth_element(A.begin()+l,A.begin()+mid,A.begin()+r+1,cmp);
    node *ret=A[mid];
    ret->l=build(k+1,l,mid-1);
    ret->r=build(k+1,mid+1,r);
    ret->up();
    return ret;
}
const double alpha=0.75,loga=log2(1.0/alpha);
inline int size(node *o){
    return o?o->s:0;
}
inline bool isbad(node*o){
    return size(o->l)>alpha*o->s||size(o->r)>alpha*o->s;
}
void flatten(node *u,vector<node*>::iterator &it){
    if(!u)return;
    flatten(u->l,it);
    *it=u;
    flatten(u->r,++it);
}
bool insert(node*&u,int k,const point &x,long long data,int dep){
    if(!u){
        u=new node(x,data);
        return dep<=0;
    }
    if(u->pid==x){
        u->data=data;
        u->up();
        return 0;
    }
    cmp.sort_id=k;
    if(insert(cmp(x,u->pid)?u->l:u->r,(k+1)%kd,x,data,dep-1)){
        ++u->s;
        if(!isbad(u)){
            u->up();
            return 1;
        }
        if((int)A.size()<u->s)A.resize(u->s);
        vector<node*>::iterator it=A.begin();
        flatten(u,it);
        u=build(k,0,u->s-1);
    }
    u->up();
    return 0;
}
inline void insert(const point &x,long long data){
    insert(root,0,x,data,std::__lg(size(root))/loga);
}
inline bool range_include(node *o,const point &L,const point &R){
    for(int i=0;i<kd;++i){
        if(L.d[i]>o->ma.d[i]||R.d[i]<o->mi.d[i])return 0;
    }
    return 1;
}
inline bool range_in_range(node *o,const point &L,const point &R){
    for(int i=0;i<kd;++i){
        if(L.d[i]>o->mi.d[i]||o->ma.d[i]>R.d[i])return 0;
    }
    return 1;
}
inline bool point_in_range(node *o,const point &L,const point &R){
    for(int i=0;i<kd;++i){
        if(L.d[i]>o->pid.d[i]||R.d[i]<o->pid.d[i])return 0;
    }
    return 1;
}
long long query(node *o,const point &L,const point &R){
    if(!o)return 0;
    if(range_in_range(o,L,R))return o->gcd;
    long long ans=point_in_range(o,L,R)?o->data:0;
    if(o->l&&range_include(o->l,L,R))ans=__gcd(ans,query(o->l,L,R));
    if(o->r&&range_include(o->r,L,R))ans=__gcd(ans,query(o->r,L,R));
    return ans;
}
int n;
int main(){
    scanf("%*d%*d%d",&n);
    while(n--){
        int c,x1,y1,x2,y2;
        long long d;
        scanf("%d",&c);
        if(c==1){
            scanf("%d%d%lld",&x1,&y1,&d);
            insert(point(x1,y1),d);
        }else{
            scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
            printf("%lld\n",query(root,point(x1,y1),point(x2,y2)));
        }
    }
    return 0;
}
