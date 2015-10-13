/*
 * the kernel of the polygon
 */
Polygon Kernel(const Polygon &rhs){
    HalfPlaneSet hlps;
    for(int i=0;i<rhs.N;i++)
        hlps.add(HalfPlane(rhs.s[i], rhs.s[(i+1)%rhs.N]));
    return hlps.Solve();
}
