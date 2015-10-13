class Circle{
public:
    Point O;
    double R;
    Circle(const Point &o, const double &r): O(o), R(r) {}
    double Area() const {
        return PI * R * R;
    }
    double Perimeter() const {
        return 2.0 * PI * R;
    }
    /*
     * default not includes on the edge
     */
    bool InCircle(const Point &rhs) const {
        return cmp(O.Dist(rhs) - R) < 0;
    }
    /*
     * default is segment
     * if want to change it to line, remove the if which judge t
     */
    vector<Point> Intersection(const Line &rhs){
        vector<Point> res;
        Point d1 = rhs.b - rhs.a, d2 = rhs.a - O; 
        double A = d1.x*d1.x + d1.y*d1.y;
        double B = 2.0 * d1.Dot(rhs.a-O);
        double C = d2.x*d2.x + d2.y*d2.y - R*R;
        double D = B*B -4*A*C;
        if(cmp(D) >= 0){
            double t1 = (-B - sqrt(max(0.0, D))) / (2.0*A);
            double t2 = (-B + sqrt(max(0.0, D))) / (2.0*A);
            if(cmp(t1-1) <= 0 && cmp(t1) >= 0)
                res.push_back(rhs.a + d1*t1);
            if(cmp(t1-t2) != 0 && cmp(t2-1) <= 0 && cmp(t2) >= 0)
                res.push_back(rhs.a + d1*t2);
        }
        return res;
    }
    /*
     * the intersections of two circle
     */
    pair<Point, Point> Intersection(const Circle &rhs) const {
        double d = (O-rhs.O).Abs();
        double cost = (R*R+d*d-rhs.R*rhs.R) / (2.0*R*d);
        double sint = sqrt(1.0 - cost*cost);
        Point rfn = (rhs.O-O) / d * R;
        return make_pair(O+rfn.Rotate(cost, sint), O+rfn.Rotate(cost, -sint));
    }
    friend ostream& operator << (ostream& out, const Circle &rhs){
        out << "C{" << rhs.O << ", " << rhs.R << "}";
        return out;
    }
    bool operator < (const Circle &rhs) const {
        if(cmp(R-rhs.R) != 0) return cmp(R-rhs.R) < 0;
        return O < rhs.O;
    }
    bool operator == (const Circle &rhs) const {
        return cmp(R-rhs.R) == 0 && O == rhs.O;
    }
};

