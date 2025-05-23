int GLOB = -1;

void testIntOperation() {
    print("Testing int operators...");
    
    
    int a = 5;
    if (a != 5) {
        print("faild testIntOperation a != 5");
    }
    
    
    int b = a + 3;
    if (b != 8) {
        print("faild testIntOperation b != 8");
    }
    int c = b - 2;
    if (c != 6) {
        print("faild testIntOperation c != 6");
    }
    
    
    int d = c * 2;
    if (d != 12) {
        print("faild testIntOperation d != 12");
    }
    int e = d / 3;
    if (e != 4) {
        print("faild testIntOperation e != 4");
    }
    int f = d % 5;
    if (f != 2) {
        print("faild testIntOperation f != 2");
    }
    
    
    f += 3;
    if (f != 5) {
        print("faild testIntOperation f != 5");
    }
    f -= 2;
    if (f != 3) {
        print("faild testIntOperation f != 3");
    }
    f *= 4;
    if (f != 12) {
        print("faild testIntOperation f != 12");
    }
    f /= 3;
    if (f != 4) {
        print("faild testIntOperation f != 4");
    }
    f %= 3;
    if (f != 1) {
        print("faild testIntOperation f != 1");
    }
    
    
    int g = (a == 5) && (b == 8);
    if (g == 0) {
        print("faild testIntOperation g == 0");
    }
    int h = (a == 0) || (b == 8);
    if (h == 0) {
        print("faild testIntOperation h == 0");
    }
    
    
    if (a != 5) {
        print("faild testIntOperation a != 5");
    }
    if (a == 6) {
        print("faild testIntOperation a == 6");
    }
    if (a >= 6) {
        print("faild testIntOperation a >= 6");
    }
    if (a > 6) {
        print("faild testIntOperation a > 6");
    }
    if (b <= 7) {
        print("faild testIntOperation b <= 7");
    }
    if (b < 8) {
        print("faild testIntOperation b < 8");
    }
    return;
}

void testDoubleOperation() {
    print("Testing double operators...");
    
    
    double a = 5.5;
    if (a != 5.5) {
        print("faild testIntOperation a != 5.5");
    }
    
    
    double b = a + 3.2;
    if (b != 8.7) {
        print("faild testIntOperation b != 8.7");
    }
    double c = b - 2.1;
    if (c != 6.6) {
        print("faild testIntOperation c != 6.6");
    }
    
    
    double d = c * 2.0;
    if (d != 13.2) {
        print("faild testIntOperation d != 13.2");
    }
    double e = d / 3.0;
    if (e != 4.4) {
        print("faild testIntOperation e != 4.4");
    }
    
    
    e += 1.1;
    if (e != 5.5) {
        print("faild testIntOperation e != 5.5");
    }
    e -= 0.5;
    if (e != 5.0) {
        print("faild testIntOperation e != 5.0");
    }
    e *= 2.0;
    if (e != 10.0) {
        print("faild testIntOperation e != 10.0");
    }
    e /= 4.0;
    if (e != 2.5) {
        print("faild testIntOperation e != 10.0");
    }
    
    
    int g = (a == 5.5) && (b == 8.7);
    if (g == 0) {
        print("faild testIntOperation g == 0");
    }
    int h = (a == 0.0) || (b == 8.7);
   
    if (h == 0) {
        print("faild testIntOperation h == 0");
    }
    
    if (a != 5.5) {
        print("faild testIntOperation a != 5.5");
    }
    if (a == 5.6) {
        print("faild testIntOperation a == 5.6");
    }
    if (a >= 6.0) {
        print("faild testIntOperation a >= 6.0");
    }
    if (a > 5.5) {
        print("faild testIntOperation a > 5.5");
    }
    if (b <= 8.6) {
        print("faild testIntOperation b <= 8.6");
    }
    if (b < 8.7) {
        print("faild testIntOperation b < 8.7");
    }
    
    return;
}

void testStringOperation() {
    print("Testing string operators...");
    
    
    string a = "Hello";
    if (a != "Hello") {
        print("faild testStringOperation a != Hello")
    }
    
    
    string b = a + " World";
    if (b != "Hello World") {
        print(b)
        print("faild testStringOperation b != Hello World")
    }
    
    
    b += "!";
    if (b != "Hello World!") {
        print("faild testStringOperation b != Hello World!")
    }
    
    if (a != "Hello") {
        print("faild testStringOperation a != Hello");
    }
    if (a == "hello") {
        print("faild testStringOperation a != hello");
    }
    
    if ("aaa" > "bbb") {
         print("faild testStringOperation aaa > bbb");
    }
    if ("abb" >= "bbb") {
         print("faild testStringOperation abb >= bbb");
    }
    if ("bbb" < "aaa") {
         print("faild testStringOperation bbb < aaa");
    }
    if ("bbb" <= "abb") {
         print("faild testStringOperation bbb <= abb");
    }
    
   
    int c = (a == "Hello") && (b == "Hello World!");
    if (c == 0) {
        print("faild testStringOperation c == 0");
    }
    int d = (a == "hello") || (b == "Hello World!");
    if (d == 0) {
        print("faild testStringOperation d == 0");
    }
    return;
}

int testElse(int n) {
    if (n == 0) {
        return 10;
    }
    else {
        return 30;
    }
    return;
}

int testElif(int n) {
    if (n == 0) {
        return 10;
    }
    elif (n == 1) {
        return 15;
    }
    elif (n == 2) {
        return 20;
    }
    else {
        return 30;
    }
    return;
}

int testWhile(int a) {
    int i = 0;
    while (i < a) {
        i += 1;
    }
    return i;
}

int testFor(int a) {
    int cnt = 0;
    for (int i = 0; i < a; i+=1) {
        cnt += 1;
    }
    return cnt;
}

void testOperators() {


    print("Testing elif operator...");
    if (testElif(0) != 10) {
        print("faild testElif(0) != 10");
    }
    if (testElif(2) != 20) {
        print("faild testElif(2) != 20");
    }
    if (testElif(1) != 15) {
        print("faild testElif(1) != 15");
    }
    if (testElif(3) != 30) {
        print("faild testElif(3) != 30");
    }


    print("Testing else operator...");
    if (testElse(0) != 10) {
        print("faild testElse(0) != 10");
    }
    if (testElse(2) != 30) {
        print("faild testElse(2) != 30");
    }
    if (testElse(1) != 30) {
        print("faild testElse(1) != 30");
    }
    if (testElse(3) != 30) {
        print("faild testElse(3) != 30");
    }


    print("Testing for operator...");
    if (testFor(10) != 10)
    {
        print("faild testFor(10) != 9");
    }
    if (testFor(-5) != 0)
    {
        print("faild testFor(-5) != 0");
    }


    print("Testing while operator...");
    if (testWhile(10) != 10)
    {
        print("faild testWhile(10) != 9");
    }
    if (testWhile(-5) != 0)
    {
        print("faild testWhile(-5) != 0");
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

void testRecursion() {

    print("Testing recursion...");

    if (fibonacci(-1) != 1) {
        print("faild fibonacci(-1) != 1");
    }
    if (fibonacci(1) != 1) {
        print("faild fibonacci(1) != 1");
    }
    if (fibonacci(8) != 21) {
        print("faild fibonacci(8) != 21");
    }
    if (fibonacci(3) != 2) {
        print("faild fibonacci(3) != 2");
    }
    if (fibonacci(9) != 34) {
        print("faild fibonacci(9) != 34");
    }
    if (fibonacci(2) != 1) {
        print("faild fibonacci(2) != 1");
    }
    if (fibonacci(5) != 5) {
        print("faild fibonacci(5) != 5");
    }


    return;
}

void testArrays() {
    print("Testing arrays1...");
    int n1 = 4;
    int n2 = 5; 
    int n3 = 2;
    int n4 = 6;
    int n5 = 3;
    int i;
    int j;
    int k;
    int l;
    int p;
    int A[n1][n2][n3][n4][n5];
    for (i = 0; i < n1; i += 1) {
        for (j = 0; j < n2; j += 1) {
            for (k = 0; k < n3; k += 1) {
                for (l = 0; l < n4; l += 1) {
                    for (p = 0; p < n5; p += 1) {
                        A[i][j][k][l][p] = i * j * k * l * p;
    } } } } }
    for (i = 0; i < n1; i += 1) {
        for (j = 0; j < n2; j += 1) {
            for (k = 0; k < n3; k += 1) {
                for (l = 0; l < n4; l += 1) {
                    for (p = 0; p < n5; p += 1) {
                        if (i * j * k * l * p != A[i][j][k][l][p]) {
                            print("error", "A[", i, "]", "[", j, "]", "[", l, "]", "[", k, "]", "[", p, "] != ", i * j * k * l * p, A[i][j][k][l][p]);
    } } } } } }
    return;
}

void testArrays2()
{
    print("Testing arrays2...");
    int n1 = 4;
    int n2 = 5; 
    int i;
    int j;
    double A[n1][n2];
    for (i = 0; i < n1; i += 1) {
        for (j = 0; j < n2; j += 1) {
            A[i][j] = sqrt(i * j);
    } }
    for (i = 0; i < n1; i += 1) {
        for (j = 0; j < n1; j += 1) {
            if (sqrt(i * j) != A[i][j]) {
                print("error", "A[", i, "]", "[", j, "] != ", sqrt(i * j), A[i][j]);
    } } }
    return;
}

void testArrays3() {
    print("Testing arrays3...");
    int n1 = 5;
    int n2 = 7;
    int i;
    int j;
    int A[n1][n2];
    for (i = 0; i < n1; i += 1) {
        for (j = 0; j < n2; j += 1) {
            A[i][j] = i * j;
    } }
    double B[A[4][4]][A[3][6]];
    int n3 = 16;
    int n4 = 18;
    for (i = 0; i < n3; i += 1) {
        for (j = 0; j < n4; j += 1) {
            B[i][j] = sqrt(i * j);
    } }
    for (i = 0; i < n3; i += 1) {
        for (j = 0; j < n4; j += 1) {
            if (sqrt(i * j) != B[i][j]) {
                print("error", "B[", i, "]", "[", j, "] != ", sqrt(i * j), B[i][j]);
    } } }
    return;
}

void testArrays4() {
    print("Testing arrays4...");
    int n1 = 5;
    int n2 = 7;
    int i;
    int j;
    string A[n1][n2];
    for (i = 0; i < n1; i += 1) {
        for (j = 0; j < n2; j += 1) {
            if (i > 0 && j > 0) { 
                A[i][j] = "a" + A[i-1][j] + A[i][j-1];
            } else {
                A[i][j] = "";
            } 
    } } 
    for (i = 0; i < n1; i += 1) {
        for (j = 0; j < n2; j += 1) {
            if (i > 0 && j > 0) { 
                if (A[i][j] != "a" + A[i-1][j] + A[i][j-1]) {
                    print("error", "A[", i, "]", "[", j, "] != ", "expected...", A[i][j]);
                }
            } else {
                if (A[i][j] != "") {
                    print("error", "A[", i, "]", "[", j, "] != ", "empty", A[i][j]);
                }
            }
    } }
    return;
}

void testArrays5() {
    print("Testing arrays5...");
    int n1 = 5;
    int i;
    int A[n1];
    int B[n1];
    for (i = 0; i < n1; i += 1) {
        A[i] = i;
    }
    for (i = 0; i < n1; i += 1) {
        B[A[n1 - A[i] - 1]] = i;
    }
    for (i = 0; i < n1; i += 1) {
        if (n1 - i - 1 != B[i]) {
            print("error", "B[", i, "]", " != ", i, B[i]);
        }
    }
    return;
}

void processGlobal() {
    print("Testing processGlobal...");
    if (GLOB != (-1)) { print("error: global variable isn't valid", GLOB); }
    GLOB = GLOB + 1;
    return;
}

void testGlobal() {
    print("Testing testGlobal...");
    if (GLOB != 0) { print("error: global variable isn't valid", GLOB); }
    return;
}

void testBrackets() {
    print("Testing testBrackets...");
    int a = 2;
    int b = (((a + 2) / 2) == 2);
    if (b != 1) { print("error", "b != 1", b); }
    return;
}

int main() {
    testIntOperation();
    testDoubleOperation();
    testStringOperation();
    testOperators();
    testRecursion();
    testArrays();
    testArrays2();
    testArrays3();
    testArrays4();
    testArrays5();
    processGlobal();
    testGlobal();
    testBrackets();
    return 0;
}