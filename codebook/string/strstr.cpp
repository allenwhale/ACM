#include <bits/stdc++.h>
using namespace std;
char s1[1000000], s2[10000];
int main(){
    memset(s1, 'a', sizeof(s1));
    memset(s2, 'a', sizeof(s2));
    char *ptr = strstr(s1, s2);
    while(ptr){
        printf("%ld\n", ptr-s1);
        ptr = strstr(ptr+1, s2);
    }
}
