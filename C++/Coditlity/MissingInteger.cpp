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

//Another solution#include <set>
int solution(vector<int> &A) {
    // Implement your solution here

    if(((int)A.size() == 1) && A[0] != 1) return 1;
    if(((int)A.size() == 1) && A[0] == 1) return 2;
    std::set<int> uniqele;

    for(int i = 0;i < (int)A.size();i++)
    {
        if(A[i] > 0)
        {
            uniqele.insert(A[i]);
        }
    }

    if(uniqele.empty()) return 1;

    int min = 1;
    for(int x : uniqele)
    {
        if(x == min)
        {
            min++;
        }
        else {
            return min;
        }
    }

    return min;
}