/*
 * farthest node pair
 */
pair<double, pair<Point, Point> > Diameter(){
    if(N == 1)
        return make_pair(0, make_pair(s[0], s[0]));
    double maxd = 0;
    Point pa, pb;
    for(int i=0,j=1;i<N;i++){
        while(cmp((s[next(i)]-s[i]).Cross(s[j]-s[i])-(s[next(i)]-s[i]).Cross(s[next(j)]-s[i])) < 0)
            j = next(j);
        double d = s[i].Dist(s[j]);
        if(d > maxd)
            maxd = d, pa = s[i], pb = s[j];
        d = s[next(i)].Dist(s[next(j)]);
        if(d > maxd)
            maxd = d, pa = s[next(i)], pb = s[next(j)];
    }
    return make_pair(maxd, make_pair(pa, pb));
}  
