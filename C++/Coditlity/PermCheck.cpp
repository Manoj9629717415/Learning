// you can use includes, for example:
 #include <algorithm>
 #include <set>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here
    set<int> indexseq;
    set<int> value;
    int sumindex = 0;
    int sumvalue = 0;

    for(int i=0,j=1;i<(int)A.size();i++,j++)
    {
        indexseq.insert(j);
        sumindex = sumindex + j;
        value.insert(A[i]);
        sumvalue = sumvalue + A[i];
    }

    if(indexseq.size()==value.size())
    {
        if(sumindex == sumvalue)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }


}