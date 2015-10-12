/*
 * gen phi from 1~MAXN
 * store answer in phi
 */
#define MAXN 100
int mindiv[MAXN], phi[MAXN];
void genphi(){
    for(int i=1;i<MAXN;i++)
        mindiv[i] = i;
    for(int i=2;i*i<MAXN;i++)
        if(mindiv[i] == i)
            for(int j=i*i;j<MAXN;j+=i)
                mindiv[j] = i;
    phi[1] = 1;
    for(int i=2;i<MAXN;i++){
        phi[i] = phi[i/mindiv[i]];
        if((i/mindiv[i])%mindiv[i] == 0)
            phi[i] *= mindiv[i];
        else phi[i] *= (mindiv[i]-1);
    }
}
