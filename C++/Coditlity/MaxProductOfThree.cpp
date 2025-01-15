// you can use includes, for example:
 #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here

    std::sort(A.begin(),A.end());

    int plusmax = A[A.size()-1] * A[A.size()-2] * A[A.size()-3];

    int minmax =  0;

    if(A[0] < 0 && A[1] < 0)
    {
        minmax = A[0] * A[1] * A[A.size()-1];
    }

    return plusmax > minmax ? plusmax : minmax;


}