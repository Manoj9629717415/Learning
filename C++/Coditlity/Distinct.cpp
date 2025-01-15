// you can use includes, for example:
// #include <algorithm>
#include <set>
// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here
    set<int> unique;
    if(A.empty()) return 0;
    for(auto x : A)
    {
        unique.insert(x);
    }

    return unique.size();
}