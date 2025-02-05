// you can use includes, for example:
// #include <algorithm>
#include <cmath>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(int N) {
    // Implement your solution here

    int count = 0;

    for(int i=1;i<=sqrt(N);i++)
    {
        if(N%i == 0)
        {
            if(i == N/i)
            {
                count++;
            }
            else
            {
                count += 2;
            }

        }
    }

    return count;
}