// you can use includes, for example:
 #include <algorithm>
 #include <bitset>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
// BinaryGap
// Find longest sequence of zeros in binary representation of an integer.

int solution(int N) {
    // Implement your solution here
    std::string str = std::bitset<sizeof(int)*8>(N).to_string();
    int maxlength = 0;
    std::size_t one_bin = str.find("1");
    std::size_t zero_bin = str.find("0");
    if(one_bin != std::string::npos && zero_bin != std::string::npos)
    {
        str.erase(0,one_bin);
        std::string tempstring = "";
        for(char &x : str)
        {
            if(x== '1')
            {
                if(tempstring.length() > 1)
                {
                    if(((int)tempstring.length() -1) > maxlength)
                    {
                        maxlength = tempstring.length() - 1;
                    }
                }
                tempstring.clear();
                tempstring = tempstring + x;
            }
            else
            {
                if(tempstring.length() > 0)
                {
                    tempstring = tempstring + x;
                }
            }
        }
    }
    return maxlength;
}
