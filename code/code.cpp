int fibonacci(int n) {
    if (n <= 2) {
        print(n);
        return 1;
    }
    else {
        print(fibonacci(n - 1) + fibonacci(n - 2));
    }
}

int main() {
    int n;
    scan(n);
    fibonacci(n);
}