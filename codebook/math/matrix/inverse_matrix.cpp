/*
 * return an inverse matrix
 * if row != column or the inverse matrix doesn't exist, it will return an empty matrix
 */
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
