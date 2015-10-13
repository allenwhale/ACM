/*
 * return guass eliminated matrix 
 * r will be chenged to the number of the non-free variables
 * l[i] will be set to true if i-th variable is not free
 * ignore flag
 */
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
