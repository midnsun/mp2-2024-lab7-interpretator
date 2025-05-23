int Graph[15][4][2];
int Adjacency[15];
int Result[15];
int Path[15];
int PriorityQueue[15 * 15][2];
int Size;
int MaxInt = 2147483647;
int tmpe1;
int tmpe2;

void fillGraph() {
    Adjacency[0] = 2; Graph[0][0][0] = 5; Graph[0][0][1] = 20; Graph[0][1][0] = 1; Graph[0][1][1] = 15;
    Adjacency[1] = 3; Graph[1][0][0] = 0; Graph[1][0][1] = 15; Graph[1][1][0] = 6; Graph[1][1][1] = 0; Graph[1][2][0] = 2; Graph[1][2][1] = 25;
    Adjacency[2] = 3; Graph[2][0][0] = 1; Graph[2][0][1] = 25; Graph[2][1][0] = 3; Graph[2][1][1] = 10; Graph[2][2][0] = 7; Graph[2][2][1] = 70;
    Adjacency[3] = 2; Graph[3][0][0] = 2; Graph[3][0][1] = 10; Graph[3][1][0] = 8; Graph[3][1][1] = 50;
    Adjacency[4] = 1; Graph[4][0][0] = 9; Graph[4][0][1] = 10;
    Adjacency[5] = 2; Graph[5][0][0] = 0; Graph[5][0][1] = 20; Graph[5][1][0] = 10; Graph[5][1][1] = 30;
    Adjacency[6] = 3; Graph[6][0][0] = 1; Graph[6][0][1] = 0; Graph[6][1][0] = 7; Graph[6][1][1] = 40; Graph[6][2][0] = 10; Graph[6][2][1] = 10;
    Adjacency[7] = 4; Graph[7][0][0] = 6; Graph[7][0][1] = 40; Graph[7][1][0] = 2; Graph[7][1][1] = 70; Graph[7][2][0] = 8; Graph[7][2][1] = 30; Graph[7][3][0] = 12; Graph[7][3][1] = 60;
    Adjacency[8] = 4; Graph[8][0][0] = 7; Graph[8][0][1] = 30; Graph[8][1][0] = 12; Graph[8][1][1] = 5; Graph[8][2][0] = 13; Graph[8][2][1] = 70; Graph[8][3][0] = 3; Graph[8][3][1] = 50;
    Adjacency[9] = 2; Graph[9][0][0] = 4; Graph[9][0][1] = 10; Graph[9][1][0] = 14; Graph[9][1][1] = 25;
    Adjacency[10] = 3; Graph[10][0][0] = 5; Graph[10][0][1] = 30; Graph[10][1][0] = 6; Graph[10][1][1] = 10; Graph[10][2][0] = 11; Graph[10][2][1] = 10;
    Adjacency[11] = 2; Graph[11][0][0] = 10; Graph[11][0][1] = 10; Graph[11][1][0] = 12; Graph[11][1][1] = 35;
    Adjacency[12] = 4; Graph[12][0][0] = 11; Graph[12][0][1] = 35; Graph[12][1][0] = 7; Graph[12][1][1] = 60; Graph[12][2][0] = 8; Graph[12][2][1] = 5; Graph[12][3][0] = 13; Graph[12][3][1] = 70;
    Adjacency[13] = 2; Graph[13][0][0] = 12; Graph[13][0][1] = 70; Graph[13][1][0] = 8; Graph[13][1][1] = 70;
    Adjacency[14] = 1; Graph[14][0][0] = 9; Graph[14][0][1] = 25;
    for (int i = 0; i < 15; i += 1) {
        Result[i] = MaxInt;
        Path[i] = MaxInt;
    }
    Size = 0;
    return;
}

void swap(int ind1, int ind2) {
    int tmp1 = PriorityQueue[ind1][0];
    int tmp2 = PriorityQueue[ind1][1];
    PriorityQueue[ind1][0] = PriorityQueue[ind2][0];
    PriorityQueue[ind1][1] = PriorityQueue[ind2][1];
    PriorityQueue[ind2][0] = tmp1;
    PriorityQueue[ind2][1] = tmp2;
    return;
}

void slideDown(int ind) {
    int i = ind;
    while(i < Size) {
        if ((2*i+2) < Size) {
            if (PriorityQueue[i][1] < PriorityQueue[2*i+1][1] && PriorityQueue[i][1] < PriorityQueue[2*i+2][1]) { return; }
            if (PriorityQueue[2*i+1][1] < PriorityQueue[2*i+2][1]) {
                swap(2*i+1, i);
                i = 2*i+1;
            }
            elif (PriorityQueue[2*i+1][1] >= PriorityQueue[2*i+2][1]) {
                swap(2*i+2, i);
                i = 2*i+2;
            }
        }
        elif ((2*i+2) < Size) {
            if (PriorityQueue[1][1] < PriorityQueue[2*i+1][1]) { return; }
            swap(2*i+1, i);
            i = 2*i+1;
        }
        else { return; }
    }
    return;
}

void slideUp(int ind) {
    int i = ind;
    while(i >= 0) {
        if (((i - 1) / 2) >= 0) {
            if (PriorityQueue[(i - 1) / 2][1] > PriorityQueue[i][1]) {
                swap((i - 1) / 2, i);
            }
            else { return; }
        }
        i = (i - 1) / 2;
    }
    return;
}

int IsEmpty() {
    return Size == 0;
}

void Add(int n, int d) {
    PriorityQueue[Size][0] = n;
    PriorityQueue[Size][1] = d;
    Size += 1;
    slideUp(Size - 1);
    return;
}

void ExtractMin() {
    tmpe1 = PriorityQueue[0][0];
    tmpe2 = PriorityQueue[0][1];
    Size -= 1;
    PriorityQueue[0][0] = PriorityQueue[Size][0];
    PriorityQueue[0][1] = PriorityQueue[Size][1];
    PriorityQueue[Size][0] = tmpe1;
    PriorityQueue[Size][1] = tmpe2;
    slideDown(0);
    return;
}

void Dijkstra(int n) {
    Result[n] = 0;
    Add(n, 0);
    int i; int e1; int e2; int eit1; int eit2;
    while (IsEmpty() == 0) {
        ExtractMin();
        e1 = tmpe1;
        e2 = tmpe2;
        if (e2 == Result[e1]) {
            for (i = 0; i < Adjacency[e1]; i += 1) {
                eit1 = Graph[e1][i][0];
                eit2 = Graph[e1][i][1];
                if (Result[eit1] > Result[e1] + eit2) {
                    Result[eit1] = Result[e1] + eit2;
                    Add(eit1, Result[eit1]);
                    Path[eit1] = e1;
                }
            }
        }

        
    }

    return;
}

int main() {
    int q = 1;
    int n;
    int i;
    int tmp;
    int tmpPath[15];
    print("Welcome to the Dijkstra algorithm program!");
    fillGraph();
    print("Unfortianutly, you can't enter the graph because it was hard-coded. But you still can enter the number of a vertex you want to find path to:");
    scan(n);
    print("Executing Dijkstra algorithm");
    Dijkstra(n);
    while (q) {
        print("Enter the number of a vertex you want to find path for");
        scan(n);
        tmp = Result[n];
        print("The lenght of the shortest path is:", tmp);
        for (i = 0; n != MaxInt; n = Path[n]) {
            tmpPath[i] = n;
            i += 1;
        }
        n = i;
        print("The shortest path is: ");
        for (i = n - 1; i >= 0; i -= 1) {
            print(tmpPath[i]);
        }
        print("Type 0 to escape");
        scan(q);
    }
    print("Goodbye!");
    return 0;
}