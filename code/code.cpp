int fibonacci(int n) {
    if (n <= 2) {
        return 1;
    }
    else {
        int r1 = fibonacci(n - 1);
        print(r1);
        int r2 = fibonacci(n - 2);
        print(r2);
        return r1 + r2;
    }
    return 0;
}

int main() {
    int n;
    scan(n);
    n = fibonacci(n);
    print(n);
    return 0;
}