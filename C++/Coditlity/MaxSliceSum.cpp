// you can use includes, for example:
#include <algorithm>
#include <numeric>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

// you can also do with Kadane's Algorithm

int solution(vector<int> &A) {
    // Implement your solution here

    if((int)A.size() == 1) return A[0];

    int max_Prefixsum = A[0];
   // int max_Postfixsum = sum - A[0];
    int maxsum = A[0];


    for(int i = 1;i<(int)A.size();i++)
    {
        max_Prefixsum = max_Prefixsum + A[i];

         if(A[i] > max_Prefixsum)
        {
            max_Prefixsum = A[i];
        }

        if(max_Prefixsum > maxsum)
        {
            maxsum = max_Prefixsum;
        }     
    }
    return maxsum;
}