// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(vector<int> &A) {
    // Implement your solution here

    double minavg = 10000.0;
    int minstartpos = 0;

    for(int i=0;i<(int)A.size()-1;i++)
    {
        double sum = A[i];
        for(int j=i+1,count=2;j<(int)A.size();j++,count++)
        {
            sum = sum + (double)A[j];           
            double avg = sum/count;
            //cout<<A[i]<<" "<<A[j]<<" "<<sum<<" "<<count<<" "<<avg<<" "<<minavg<<endl;
            if(avg < minavg)
            {
                minavg = avg;
                minstartpos = i;
            }
        }
    }

return minstartpos;
}