void testString() {
    print("Hello, world!");
    print("Type your age...");
    int age;
    scan(age);
    print("Your age is: ", age, "!", "");
    return;
}

void testFor(int a) {
    if (a == 0) {
    for (int i = 0; i < 10; i+=1) {
        print(i * i);
    }
    }
    else {
        int j = 0;
        while (j < 10) {
            print(j + j);
            j += 1;
        }
    }
    return;
}

void testWhile(int a) {
    int i = 0;
    while (i < 10) {
        print(i);
        i += 1;
    }
    return;
}

void testElif(int n) {
    print(n);
    if (n == 0) {
        print(10);
    }
    elif (n == 1) {
        print(15);
    }
    elif (n == 2) {
        print(20);
    }
    else {
        print(30);
    }
    return;
}

void testElse(int n) {
    print(n);
    if (n == 0) {
        print(10);
    }
    else {
        print(30);
    }
    return;
}

int fibonacci(int n) {
    if (n <= 2) {
        return 1;
    }
    else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

int main() {
    print("block 0");
    for (int i = 1; i < 10; i += 1) {
        print(fibonacci(i));
    }
    print("block 1")
    testElif(0);
    testElif(2);
    testElif(1);
    testElif(3);
    print("block 2");
    testElse(0);
    testElse(2);
    testElse(1);
    testElse(3);
    print("block 4");
    testFor(1);
    testFor(0);
    return 0;
}