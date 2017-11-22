/*
 * find the geometric median
 * fail if input is a line
 */
Point GeometricMedian(const vector<Point> &points){
    Point ans;
    for(auto &p : points)
        ans = ans + p;
    ans = ans / (double)points.size();
    Point delta;
    do{
        Point nxt_ans;
        double norm = 0;
        for(auto &p : points){
            double len = (p - ans).Abs();
            if(cmp(len) == 0){
                norm = 1;
                nxt_ans = p;
                break;
            }
            norm += 1.0 / len;
            nxt_ans = nxt_ans + p / len;
        } 
        nxt_ans = nxt_ans / norm;
        delta = ans - nxt_ans;
        ans = nxt_ans;
    }while(delta.Abs() > EPS);
    return ans;
}
