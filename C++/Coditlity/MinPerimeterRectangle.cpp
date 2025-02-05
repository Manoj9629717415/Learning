// you can use includes, for example:
 #include <algorithm>
 #include <cmath>
 #include <limits.h>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(int N) {
    // Implement your solution here

    int minperimeter = INT_MAX;

    for(int i = 1;i <= sqrt(N);i++)
    {
        if( N % i == 0)
        {
            int perimeter = 2 * (i + (N/i));
            if(perimeter < minperimeter)
            {
                minperimeter = perimeter;
            }
        }
    }

    return minperimeter;

}