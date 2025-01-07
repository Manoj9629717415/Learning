// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here
    if(A.empty()) return 1;
    if(A.size() == 1 && A[0] == 1) return 2;
    if(A.size() == 1 && A[0] != 1) return 1;
    int sumarray = 0;
    int sumnumber = 0;
    int i = 1;
    for(std::vector<int>::iterator it= A.begin();it!=A.end();it++,i++)
    {
        sumarray = sumarray + *it;
        sumnumber = sumnumber + i;
    }

    int diff = sumarray - sumnumber;

    return A.size()- (diff -1);


}