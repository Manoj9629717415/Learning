// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

int solution(int A, int B, int K) {
    // Implement your solution here

    if(K==1) return (B-A)+1;
	if(A == B && A%K==0) return 1;
    if(A == B && A%K!=0) return 0;
    if((B-A)<K) return 0;

    int numofdivisible = 0;

    if(A%K == 0)
    {
        int diff = (B - A);
        numofdivisible = diff/K;
		//increment one since first value is divisible
		numofdivisible++;

        if(B%K==0 && diff%K !=0)
        {
            numofdivisible++;
        }
    }
    else
    {
        int firstdivisblevalue = A + (K - (A%K));
        int diff = (B - firstdivisblevalue);
        numofdivisible = diff/K;
		//increment one since first value is divisible
		numofdivisible++;
        if(B%K==0 && diff%K !=0)
        {
            numofdivisible++;
        }
    }
	
	return numofdivisible;
}