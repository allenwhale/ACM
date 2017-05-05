#include <bits/stdc++.h>
using namespace std;
// M variables
// N constraints
#define EPS 1e-6
pair<double, vector<double>> simplex(const vector<vector<double>> &A, const vector<double> &B, const vector<double> &C){
    int M = C.size() + 1, N = B.size(), R = N, S = M - 1;
    vector<vector<double>> D(N + 2, vector<double>(M + 1, 0));
    vector<int> idx(N + M);
    for(int i=0;i<N+M;i++) idx[i] = i;
    for(int i=0;i<N;i++){
        for(int j=0;j<M-1;j++) D[i][j] = -A[i][j];
        D[i][M - 1] = 1;
        D[i][M] = B[i];
        if(D[R][M] > D[i][M]) R = i;
    }
    for(int i=0;i<M-1;i++) D[N][i] = C[i];
    D[N + 1][M - 1] = -1;
    while(1){
        if(R < N){
            swap(idx[S], idx[R + M]);
            D[R][S] = 1.0 / D[R][S];
            for(int j=0;j<=M;j++) 
                if(j != S) D[R][j] *= -D[R][S];
            for(int i=0;i<=N+1;i++) if(i != R){
                for(int j=0;j<=M;j++) if(j != S)
                    D[i][j] += D[R][j] * D[i][S];
                D[i][S] *= D[R][S];
            }
        }
        R = S = -1;
        for(int j=0;j<M;j++)
            if(S < 0 || idx[S] > idx[j])
                if(D[N + 1][j] > EPS || (D[N + 1][j] > -EPS && D[N][j] > EPS))
                    S = j;
        if(S < 0)break;
        for (int i=0;i<N;i++) 
            if(D[i][S] < -EPS) {
                double tmp;
                if (R < 0 || ((tmp=D[R][M] / D[R][S] - D[i][M] / D[i][S]) < -EPS || (tmp < EPS && idx[R + M] > idx[i + M])))
                    R = i;
            }
        if (R < 0) return {-1, vector<double>()};
    }
    if(D[N + 1][M] < -EPS) return {-1, vector<double>()};
    double ans = 0;
    vector<double> X(M-1);
    for(int i=M;i<N+M;i++)
        if(idx[i] < M - 1)
            ans += (X[idx[i]] = D[i - M][M]) * C[idx[i]];
    return {ans, X};
}

int main(){
    int N, M;
    while(~scanf("%d%d", &N, &M)){
        vector<vector<double>> A(M, vector<double>(N, 0));
        vector<double> B(M);
        vector<double> C(N);
        for(int i=0;i<N;i++)
            scanf("%lf", &C[i]);
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++){
                scanf("%lf", &A[i][j]);
            }
            scanf("%lf", &B[i]);
        }
        auto ans = simplex(A, B, C);
        printf("Nasa can spend %d taka.\n", (int)ceil(ans.first * M));
    }
    return 0;
}

