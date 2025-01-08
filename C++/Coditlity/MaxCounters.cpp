// you can use includes, for example:
// #include <algorithm>

vector<int> solution(int N, vector<int> &A) {
    // Implement your solution here
    vector<int> counter(N,0);
    int max = 0;
    bool incremeneted = false;

    for(int i=0;i<(int)A.size();i++)
    {
        if(A[i]<=N)
        {
            counter[A[i]-1]++;
            incremeneted = true;
            if(max<counter[A[i]-1])
            {
                max = counter[A[i]-1];
            }
        }
        else
        {
            if(incremeneted || max != 0 )
            {
                counter.clear();
                counter.assign(N,max);
            }

        }
    }

    return counter;



}