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

int main() {
    testString();
    testElif(0);
    testElif(2);
    testElif(1);
    testElif(3);
    return 0;
}