#define ll long long 
#include <iostream>
#include <vector>
#include <algorithm>
#include<unordered_map>
#include <map>
#include <queue>
#include <climits>
#include<set>
#include<fstream>
#include "BTree.cpp"
#include "Buffer.cpp"
using namespace std;
/*
  it works 
  1- change the output.txt to output.bin by accomulating bit bufferes because currently it stores it as asci 1s and 0s
  2- learn how to read bin files 
  3- rewrite the code in structured way following oop
  4- complete the challenge in codingchallenges


  it works 
  1- store the prefix table or the frequency at the beginning of the file 
  2- rewrite the program in structured way

*/
unordered_map<char,int>frequencyOfChar;
unordered_map<char,string>encoded;
void readFile(string filename)
{
    fstream file(filename,ios::in);
    if(!file)
    {
        cout<<"there is no file called " <<filename<<"\n";
        return;
    }
     char ch;
    while (file.get(ch)) {
        frequencyOfChar[ch]++;    
    }
}
void encodeToFile(AccomulateBuffer ab,string filename)
{
    //  out.write(reinterpret_cast<char*>(&ab.buffer),sizeof(ab.buffer));
    //  out.close();
    
    fstream file(filename,ios::in);
    if(!file)
    {
        cout<<"there is no file called " <<filename<<"\n";
        return;
    }
    string line;
    string stream;
    int originalLength=0;
    char ch;
    while (file.get(ch)) {
        stream += encoded[ch]; 
        originalLength++;
    }
    // while(getline(file,line))
    // {
    //     int n=line.size();
    //     originalLength+=n;
    //     string newLine="";
    //     for(int i=0;i<n;i++)
    //     {
    //         newLine+=encoded[line[i]];
    //     }
    //     stream+=newLine;
    //     stream += encoded['\n'];
    // }



    ofstream out("data.bin",ios::binary);
    ab.convert(stream);
    //header format  
    // [ int ][ 1 byte for how many leftover bits at the last 8 bits][the encoded ]
    //encoded stream
    //[8 bits][8 bits] [8 bits]
    // out<<stream.length()<<ab.countBits;
    out.write((char*)&originalLength,sizeof(originalLength));
    unsigned char padding = (ab.countBits == 0) ? 0 : (8 - ab.countBits);
    out.write((char*)&padding,sizeof(padding));
    
    for(auto byte : ab.buffer)
    {
     out.write((char*)&byte,1);
    }

    ab.clear();
    out.close();
}
void decodeToFile(Tree* t, string binFile, string outFile) {
    ifstream in(binFile, ios::binary);
    if (!in) {
        cerr << "Error opening compressed file!\n";
        return;
    }

    int originalLength;
    unsigned char padding;
    in.read((char*)&originalLength, sizeof(originalLength));
    in.read((char*)&padding, sizeof(padding));

    vector<unsigned char> bytes(
        (istreambuf_iterator<char>(in)),
        (istreambuf_iterator<char>())
    );
    in.close();

    string bitstream;
    for (unsigned char b : bytes) {
        for (int i = 7; i >= 0; --i) {
            bitstream += ((b >> i) & 1) ? '1' : '0';
        }
    }
    if (padding > 0) {
        bitstream.erase(bitstream.end() - padding, bitstream.end());
    }

    string decoded = t->getLine(bitstream);

    if ((int)decoded.size() > originalLength) {
        decoded = decoded.substr(0, originalLength);
    }

    ofstream out(outFile);
    if (!out) {
        cerr << "Error opening output file!\n";
        return;
    }
    out << decoded;
    out.close();

    cout << "Decoded output written to: " << outFile << "\n";
}
// void testBin(string filename) {
//     ifstream in(filename, ios::binary);
//     if (!in) {
//         cout << "File not found!\n";
//         return;
//     }

//     // Read header
//     int originalLength;
//     unsigned char padding;
//     in.read((char*)&originalLength, sizeof(originalLength));
//     in.read((char*)&padding, sizeof(padding));

//     cout << "Original length: " << originalLength << "\n";
//     cout << "Padding bits: " << (int)padding << "\n";

//     // Read encoded data
//     vector<unsigned char> bytes(
//         (istreambuf_iterator<char>(in)),
//         (istreambuf_iterator<char>())
//     );

//     cout << "Encoded bytes: " << bytes.size() << "\n";

//     // Rebuild bitstring
//     string bitstring;
//     for (int i = 0; i < bytes.size(); i++) {
//         for (int bit = 7; bit >= 0; --bit) {
//             bitstring += ((bytes[i] >> bit) & 1) ? '1' : '0';
//         }
//     }

//     // Remove padding bits at the end
//     if (padding > 0 && !bitstring.empty()) {
//         bitstring.erase(bitstring.end() - padding, bitstring.end());
//     }

//     cout << "Bitstring length: " << bitstring.size() << "\n";
//     cout << "Bitstring: " << bitstring << "\n";
// }
int main(int argc,char ** argv)
{
    int8_t a;
    cout<<sizeof(a)<<"\n";
    string filename;
    if(sizeof(argv)/sizeof(char)>1){
    filename = argv[1];
    readFile(filename);
    }
    // for(auto ch : frequencyOfChar)
    // {
    //     cout<<ch.first<<" "<<ch.second<<"\n";
    // }
    Tree * t=new Tree(frequencyOfChar);
    t->init();
    t->BuildHofmanTree();
    t->encode();
    encoded=t->encoded;
    AccomulateBuffer buffer;
    encodeToFile(buffer,filename);
    decodeToFile(t,"data.bin","Retrieve.txt");
    // testBin("data.bin");
    return 0;
}