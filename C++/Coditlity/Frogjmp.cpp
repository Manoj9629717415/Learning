/ you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(int X, int Y, int D) {
    // Implement your solution here
    int minjump = 0;
    int distance = Y -X;

    if(distance != D && distance !=0 )
    {
        minjump = distance / D;
        if(distance % D != 0)
        {
            minjump++;
        }
    }
    return minjump;
}