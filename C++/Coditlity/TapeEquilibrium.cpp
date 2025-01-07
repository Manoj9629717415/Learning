#include <algorithm>
#include <numeric>
#include <cstdlib>


int solution(vector<int> &A) {
    // Implement your solution here

    if(A.size() == 1) return 0;
    if(A.size() == 2) return abs(A[0] - A[1]);
    //if(A.size() == 3) return abs(A[0]-(A[1]+A[2]))<abs((A[0]+A[1])-A[2])?abs(A[0]-(A[1]+A[2])):abs((A[0]+A[1])-A[2]);


    int sum = std::accumulate(A.begin(),A.end(),0);

    int sumfirstpart = 0;
    int minvalu = 1000;

    for(int x=0;x<(int)A.size()-1;x++)
    {
        sumfirstpart = sumfirstpart + A[x];
        int secondpart = sum - sumfirstpart;
        int diff = abs(sumfirstpart - secondpart);
        //cout<<sum<<" "<<sumfirstpart<<" "<<secondpart<<" "<<diff<<" "<<minvalu<<endl;
        if(diff < minvalu)
        {
            minvalu = diff;
        }
    }

    return minvalu;
}