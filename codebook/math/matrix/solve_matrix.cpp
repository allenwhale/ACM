/*
 * Ax = b
 * it will return the answer(x)
 * if row != column or there is any free variable, it will return an empty vector
 */
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
