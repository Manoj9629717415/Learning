// you can use includes, for example:
#include <algorithm>
#include <map>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here
    int numofeast = std::count(A.begin(),A.end(),0);
    int numofwest = (int)A.size() - numofeast;

    if(numofeast == 0 || numofwest == 0) return 0;

    int count = 0;
    for(int i=0;i<(int)A.size();i++)
    {
        if(A[i] == 0)
        {
            count = count + numofwest;
            numofeast--;
            if(numofeast == 0) break;
        }
        else
        {
            numofwest--;
        }
    }

    if(count > 1000000000 || count < 0)
    {
        return -1;
    }
    else{
        return count;
    }

}