// you can use includes, for example:
 #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here
    sort(A.begin(),A.end());

    for(std::vector<int>::iterator it = A.begin();it!=A.end();it=it+2)
    {
        if(it == A.end()-1)
        {
            return *it;
        }
        if(*it != *(it+1))
        {
            return *it;
        }
    }
    return 0;
}