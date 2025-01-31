// you can use includes, for example:
 #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here

    if(A.empty()) return -1;

    vector<int> temp(A);

    int halfsize = (int)A.size() / 2;

    std::sort(A.begin(),A.end());

    int count = 0;
    int prev = A[0];
   // cout<<halfsize<<endl;
    for(int i = 0 ;i <(int) A.size();i++)
    {
      //  cout<<prev<<" "<<A[i]<<" "<<count<<endl;
        if(prev != A[i])
        {
            count = 1;
        }
        else
        {
            count++;
            if(count > halfsize)
            {
                auto it = std::find(temp.begin(),temp.end(),A[i]);
                int pos = it - temp.begin();
                return pos;
            }
        }
        prev = A[i];
    }

return -1;

}