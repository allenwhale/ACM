#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define EPS 1e-9
template<class T>
class Matrix{
public:
	vector<vector<T> > D;
	int R, C;
	Matrix(): R(0), C(0) {}

	Matrix(int r, int c): R(r), C(c) {
		for(int i=0;i<R;i++)
			D.push_back(vector<T>(C));
	}

	Matrix(const Matrix &rhs): D(rhs.D), R(rhs.R), C(rhs.C) {}

	Matrix& operator = (const Matrix &rhs) {
		R = rhs.R;
		C = rhs.C;
		D = rhs.D;
	}

    T& at(const int &rhs1, const int &rhs2) {
        return D[rhs1][rhs2];
    }

    const T& at(const int &rhs1, const int &rhs2) const {
        return D[rhs1][rhs2];
    }

	Matrix operator + (const Matrix &rhs) const {
		if(R != rhs.R || C != rhs.C) return Matrix();
		Matrix res(R, C);
		for(int i=0;i<R;i++)
			for(int j=0;j<C;j++)
                res.at(i, j) = at(i, j) + rhs.at(i, j);
        return res;
	}

	Matrix operator - (const Matrix &rhs) const {
		if(R != rhs.R || C != rhs.C) return Matrix();
		Matrix res(R, C);
		for(int i=0;i<R;i++)
			for(int j=0;j<C;j++)
                res.at(i, j) = at(i, j) - rhs.at(i, j);
        return res;
	}

    Matrix operator * (const Matrix &rhs) const {
        if(C != rhs.R) return Matrix();
        Matrix res(R, rhs.C);
        for(int i=0;i<R;i++)
            for(int j=0;j<rhs.C;j++)
                for(int k=0;k<C;k++)
                    res.at(i, j) += at(i, k) * rhs.at(k ,j);
        return res;
    }

    void I() {
        for(int i=0;i<R;i++)
            at(i, i) = 1;
    } 

    Matrix pow(int rhs) const {
        if(R != C) return Matrix();
        Matrix res(R, R), p(*this);
        res.I();
        while(rhs){
            if(rhs&1)res = res * p;
            p = p * p;
            rhs >>= 1;
        }
        return res;
    }

    Matrix GuassElimination(int &r, vector<bool> &l, int flag=0) {
        l = vector<bool>(C);
        r = 0;
        Matrix res(*this);
        for(int i=0;i<res.C-flag;i++){
            for(int j=r;j<res.R;j++){
                if(fabs(res.at(j, i)) > EPS){
                    swap(res.D[r], res.D[j]);
                    break;
                }
            }
            if(fabs(res.at(r, i)) < EPS){
                continue;
            }
            for(int j=0;j<res.R;j++){
                if(j != r && fabs(res.at(j, i)) > EPS){
                    double tmp = (double)res.at(j, i) / (double)res.at(r, i);
                    for(int k=0;k<res.C;k++){
                        res.at(j, k) -= tmp * res.at(r, k);
                    }
                }
            }
            r++;
            l[i] = true;
        }
        return res;
    }

    vector<double> Solve(vector<double> a) {
        if(R != C) return vector<double>();
        vector<double> res(R);
        Matrix t(R, C+1);
        for(int i=0;i<R;i++){
            for(int j=0;j<C;j++)
                t.at(i, j) = at(i, j);
            t.at(i, C) = a[i];
        }
        int r = 0;
        vector<bool> l;
        t = t.GuassElimination(r, l, 1);
        if(r != R) return vector<double>();
        for(int i=0;i<C;i++){
            if(l[i])for(int j=0;j<R;j++){
                if(fabs(t.at(j, i)) > EPS)
                    res[i] = t.at(j, C) / t.at(j, i);
            }
        }
        return res;
    }

    Matrix Inverse() {
        if(R != C) return Matrix();
        Matrix t(R, R*2);
        for(int i=0;i<R;i++){
            for(int j=0;j<C;j++)
                t.at(i, j) = at(i, j);
            t.at(i, i+R) = 1;
        }
        int r = 0;
        vector<bool> l;
        t = t.GuassElimination(r, l, R);
        if(r != R)return Matrix();
        for(int i=0;i<C;i++){
            if(l[i])for(int j=0;j<R;j++){
                if(fabs(t.at(j, i)) > EPS){
                    for(int k=0;k<C;k++)
                        t.at(j, C+k) /= t.at(j, i);
                }
            }
        }
        Matrix res(R, C);
        for(int i=0;i<R;i++)
            for(int j=0;j<C;j++)
                res.at(i, j) = t.at(i, j+C);
        return res;
    }
    
    friend ostream& operator << (ostream &out, const Matrix<T> &rhs) {
        for(int i=0;i<rhs.R;i++)
            for(int j=0;j<rhs.C;j++)
                out << rhs.at(i, j) << (j==rhs.C-1?'\n':' ');
        return out;
    }
};

int main(){
}
