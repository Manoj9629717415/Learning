 #include <algorithm>
 #include <set>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(int X, vector<int> &A) {
    // Implement your solution here

    set<int> unique;
    int mintime = -1;

    for(int i=0;i<(int)A.size();i++)
    {
        unique.insert(A[i]);
        if((int)unique.size() == X)
        {
            mintime = i;
            break;
        }
    }
    return mintime;
}