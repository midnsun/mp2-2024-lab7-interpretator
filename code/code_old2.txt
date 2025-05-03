double add(double a, double b) {
    return a + b;
}
double sub(double a, double b) {
    return a - b;
}
double mul(double a, double b) {
    return a * b;
}
double div(double a, double b) {
    return a / b;
}

int main() {
    double a;
    double b;
    double c;
    int op = -1;
    print(0);
    while (op != 0) {
        print(1);
        scan(op);
        print(2);
        if (op > 4 || op < 1) {
            print(3);
            return 0;
        }
        else {
            scan(a);
            scan(b);
            if (op==1) {
                c = 0;
                c = add(a, b);
            }
            if (op==2) {
                c = sub(a, b);
            }
            if (op==3) {
                c = mul(a, b);
            }
            if (op==4) {
                c = div(a, b);
            }
        }
        print(4);
        print(c);
    }
    print(5);
    return 0;
}