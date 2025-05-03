void testString() {
    print("Hello, world!");
    print("Type your age...");
    int age;
    scan(age);
    print("Your age is: ", age, "!", "");
    return;
}

void testElif(int n) {
    if (n == 0) {
        print("if");
    }
    elif (n == 1) {
        print("elif1");
    }
    elif (n == 2) {
        print("elif2");
    }
    else {
        print("else");
    }
    return;
}

int main() {
    testString();
    return 0;
}