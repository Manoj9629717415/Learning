// you can use includes, for example:
 #include <algorithm>
 #include <stack>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(string &S) {
    // Implement your solution here
    stack<char> nested;

    if(S.length() == 0) return 1;
    if(S[0] == ')' || S[0] == '}' || S[0] == ']') return 0;

    for(auto &ch : S)
    {
        if(ch == '(' || ch ==  '{' || ch == '[' )
        {
            //cout<<"push "<<ch<<endl;
            nested.push(ch);
        }
        else if(ch == ')' && nested.size()> 0 && nested.top() == '(')
        {
           // cout<<"pop "<<ch<<endl;
           nested.pop();
        }
        else if(ch == '}' && nested.size()> 0 && nested.top() == '{')
        {
            //cout<<"pop "<<ch<<endl;
           nested.pop();
        }
        else if(ch == ']' && nested.size()> 0 && nested.top() == '[')
        {
            //cout<<"pop "<<ch<<endl;
           nested.pop();
        }
        else
        {
             return 0;
        }
 
    }

if(nested.size() > 0)
{
    return 0;
}
else
{
    return 1;
}

}