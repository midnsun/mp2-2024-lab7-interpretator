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

norm1();


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
    double a
    double b;
    double c;
    int op = -1;
    print("This is calculator!");
    while (op != 0) {
        print("Type 0 for quit, 1 for add, 2 for sub, 3 for mul 4 for div");
        scan(op);
        print("Enter two operands with enter after each");
        scan(a);
        scan(b);
        if (op > 4 && op < 0) {
            print("Invalid operation input");
            return 0;
        }
        else {
            if (op==1) {
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
        print("Your result is: ");
        print(c);
    }
    print("Calculator ended its work");
    return 0;
}