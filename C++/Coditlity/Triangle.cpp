// you can use includes, for example:
 #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here

    if(A.size() < 3) return 0;

    std::sort(A.begin(),A.end());

    for(int i = 2;i<(int)A.size();i++)
    {
        const long long sum = (long)A[i-2] + (long)A[i-1];
        if(sum>A[i]) return 1;    
    }

    return 0;


}