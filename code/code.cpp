void testString() {
    print("Hello, world!");
    print("Type your age...");
    int age;
    scan(age);
    print("Your age is: ", age, "!", "");
    return;
}

void testFor(int f) {
    if (f) {
        print("for");
    }
    else {
        print("not");
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

int main() {
    testElif(0);
    testElif(2);
    testElif(1);
    testElif(3);
    print(-1);
    testElse(0);
    testElse(2);
    testElse(1);
    testElse(3);
    return 0;
}