/*
 * class of polynomial function
 * coef is the coefficient
 * f(x) = sigma(c[i]*x^i)
 */
class Function {
public:
    vector<double> coef;
    Function(const vector<double> c=vector<double>()): coef(c){}
    double operator () (const double &rhs) const {
        double res = 0.0;
        double e = 1.0;
        for(int i=0;i<(int)coef.size();i++,e*=rhs)
            res += e * coef[i];
        return res;
    }
    Function derivative() const {
        vector<double> dc((int)this->coef.size()-1);
        for(int i=0;i<(int)dc.size();i++)
            dc[i] = coef[i+1] * (i+1);
        return Function(dc);
    }
    int degree() const {
        return (int)coef.size()-1;
    }
};
/*
 * calculate the integration of f(x) from a to b
 * divided into n piece
 * the bigger the n is, the more accurate the answer is
 */
template<class T>
double simpson(const T &f, double a, double b){
    double c = (a+b) / 2.0;
    return (f(a)+4.0*f(c)+f(b)) * (b-a) / 6.0;
}
template<class T>
double simpson(const T &f, double a, double b, double eps, double A){
    double c = (a+b) / 2.0;    
    double L = simpson(f, a, c), R = simpson(f, c, b);    
    if(fabs(A-L-R) <= 15.0*eps) return L + R + (A-L-R) / 15.0;    
    return simpson(f, a, c, eps/2, L) + simpson(f, c, b, eps/2, R);    
}
template<class T>
double simpson(const T &f, double a, double b, double eps){
    return simpson(f, a, b, eps, simpson(f, a, b));
}