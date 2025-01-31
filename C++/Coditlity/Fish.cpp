// you can use includes, for example:
// #include <algorithm>
#include <stack>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A, vector<int> &B) {
    // Implement your solution here
    stack<int> upstream;
    stack<int> downstream;

    int count = 0;

    for(int i = 0;i<(int)B.size();i++)
    {
        if(B[i] == 1)
        {
            count = count + (int)upstream.size();
            //upstream.clear();
            stack<int>().swap(upstream);
            downstream.push(A[i]);
        }
        else
        {
            upstream.push(A[i]);
            while(!downstream.empty())
            {
                if(downstream.top() < A[i])
                {
                    downstream.pop();
                }
                else
                {
                    upstream.pop();
                    break;
                }
            }
        }
    }

    count = count + (int)downstream.size() + (int)upstream.size();
    return count;

}