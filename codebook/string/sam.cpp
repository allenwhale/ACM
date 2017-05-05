#include <bits/stdc++.h>
using namespace std;
struct SAM{
    struct State{
        int val, fail;
        array<int, 26> ch;
        State(int v=0): val(v), fail(0){
            ch.fill(0);
        }
    };
    vector<State> states;
    int root, tail;
    void build(const string &s){
        states.clear();
        states.push_back(State());
        states.push_back(State());
        root = tail = 1;
        for(char c : s) expand(c - 'a');
    }
    void expand(int c){
        int p = tail, newp = states.size();
        states.push_back(State(states[p].val + 1));
        for(;p&&!states[p].ch[c];p=states[p].fail)
            states[p].ch[c] = newp;
        if(p == 0){
            states[newp].fail = root;
        }else{
            if(states[states[p].ch[c]].val == states[p].val + 1){
                states[newp].fail = states[p].ch[c];
            }else{
                int q = states[p].ch[c], dupp = states.size();
                states.push_back(states[q]);
                states[dupp].val = states[p].val + 1;
                states[p].fail = states[newp].fail = dupp;
                for(;p&&states[p].ch[c]==q;p=states[p].fail)
                    states[p].ch[c] = dupp;
            }
        }
        tail = newp;
    }
    bool find(const string &s){
        int p = root;
        for(char c : s){
            if(states[p].ch[c - 'a'])
                p = states[p].ch[c - 'a'];
            else return false;
        }
        return true;
    }
};

int main(){
    char s[100];
    scanf("%s", s);
    SAM sam;
    sam.build(s);
    while(~scanf("%s", s)){
        if(sam.find(s))puts("Y");
        else puts("N");
    }
    return 0;
}
