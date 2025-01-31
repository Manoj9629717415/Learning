// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here

    if (A.empty() ) return 0;

    if((int)A.size() == 1) return 0;

    int profit = 0;
    int min = A[0];

    for(int i = 1;i<(int)A.size();i++)
    {
        if(A[i] > min)
        {
            int diff = A[i] - min;
            profit = diff > profit ? diff : profit;
        }
        else if(A[i] < min)
        {
            min = A[i];
        }
    }
    return profit;


}