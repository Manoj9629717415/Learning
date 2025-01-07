
// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

vector<int> solution(vector<int> &A, int K) {
    // Implement your solution here
    vector<int> result;
    int rotate;
    if((int)A.size() == K || K == 0 || A.empty())
    {
        return A;
    }
    else if((int)A.size() < K)
    {
        rotate = K % A.size();
    }
    else
    {
        rotate = K;
    }

    std::vector<int>::iterator it;

    for(it=A.end()-rotate;it!=A.end();it++)
    {
        result.push_back(*it);
    }

    for(it=A.begin();it!=A.end()-rotate;it++)
    {
        result.push_back(*it);
    }

    return result;
}