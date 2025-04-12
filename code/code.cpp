double norm1() {
    double res = 0.0;
    int i = 0;
    while (i < sz) {
        res+=abs(double(pMem[i]));
        i+=1;
    }
    return res;
}

double norm2() {
    double res = 0.0;
    int i = 0;
    while (i < sz) {
        res+=double(pMem[i])*double(pMem[i]);
        i+=1;
    }
    return sqrt(res);
}