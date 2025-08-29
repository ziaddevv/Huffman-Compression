#define ll long long 
#include <iostream>
#include <vector>
using namespace std;

class AccomulateBuffer {
public:
    unsigned char byte;
    vector<unsigned char> buffer;
    int countBits;

    AccomulateBuffer() {
        byte = 0;
        countBits = 0;
    }

    // uint8_t byte_value(string stream,int code_length)
    // {
         

    // }

    void convert(const string& line) {
        for (char c : line) {
            add(c == '1');
            if (countBits == 8) {
                buffer.push_back(byte);
                countBits = 0;
                byte = 0;
            }
        }

        if (countBits > 0) {
            byte <<= (8 - countBits);  
            buffer.push_back(byte);
        }
    }

    void clear() {
        buffer.clear();
        byte = 0;
        countBits = 0;
    }

    void add(bool d) {
        byte = (byte << 1) | (d ? 1 : 0);
        countBits++;
    }
};
