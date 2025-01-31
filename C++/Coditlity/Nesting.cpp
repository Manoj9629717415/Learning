// you can use includes, for example:
// #include <algorithm>
#include <stack>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(string &S) {
    // Implement your solution here

    if (S.empty()) return 1;

    stack<char> nested;

    for(auto &ch : S)
    {
        if(ch == ')')
        {
            if(nested.top() == '(')
            {
                nested.pop();
            }
            else
            {
                return 0;
            }
        }
        else
        {
            nested.push(ch);
        }
    }

if(nested.empty())  return 1;
else return 0;



}