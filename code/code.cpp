double norm1() {
    double res = 0.0;
    int i = 0;
    while (i < sz) {
        res+=abs(pMem[i]);
        i+=1;
    }
    if (res > 0) {
        i = 0;
    }
    return res;
}

double norm2() {
    double res = 0.0;
    int i = 0;
    while (i < sz) {
        res+=pMem[i]*pMem[i];
        i+=1;
    }
    if (res > 0) {
        i = 0;
    }
    else {
        i = 1;
    } 
    return sqrt(res);
}

int main() {
    norm1();
    return 0;
}
