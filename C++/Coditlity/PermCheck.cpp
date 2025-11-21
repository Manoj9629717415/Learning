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

//another solution#include <set>

int solution(vector<int> &A) {
    // Implement your solution here
    std::set<int> uniqele;
    int prevsize = 0;

    for(int &x : A)
    {
        uniqele.insert(x);
        if(prevsize == (int)uniqele.size())
        {
            return 0;
        }
        prevsize = uniqele.size();
    }

     std::set<int>::reverse_iterator rit = uniqele.rbegin();

     if(*rit != (int)uniqele.size()) return 0;


    return 1;
}