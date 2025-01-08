#include <set>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here
    set<int> unique(A.begin(),A.end());
    int i = 1;

    for(auto x : unique)
    {
        if(x > 0)
        {
            if( x != i)
            {
                break;
            }
            i++;
        }
    }
    return i;
}