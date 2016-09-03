struct ClosestPair{
    vector<Point> points;
    void add_point(const Point& p){
        points.push_back(p);
    }
    double closest_pair(int l, int r){
        if(l == r) return INF;
        int m = (l + r) >> 1;
        double ans = min(closest_pair(l, m), closest_pair(m+1, r));
        inplace_merge(points.begin()+l, points.begin()+m+1, points.begin()+r+1, [](const Point &a, const Point &b){ return a.y < b.y; });
        vector<Point> t;
        for(int i=l;i<=r;i++)
            if(abs(points[i].x - points[m].x) <= ans)
                t.push_back(points[i]);
        for(int i=0;i<(int)t.size();i++)
            for(int j=i+1;j<(int)t.size();j++)
                if(abs(t[i].y - t[j].y) >= ans) break;
                ans = min(ans, (t[i]-t[j]).Abs());
        return ans;
    }
    double Solve(){
        sort(points.begin(), points.end());
        return closest_pair(0, points.size()-1);
    }
};
