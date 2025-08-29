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
#include <cstdint>
using namespace std;
/*
  Header file 
  magic number ---> to make sure this file follow the format that is used in this tool
  file extension
  Number of codes --> 2 bytes --> just to maintain the Hofman tree that has been built before 
  [code table] consists of
   1- char -->  1 byte
   2- code --> string


  original length --> of chars  [4 bytes]
  padding --> [1 bytes] --> used only to handle the last byte written in file (if it is less that 8 bits )
  bitstream --> take 1 bytes each time

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
void encodeToFile(AccomulateBuffer ab,string filename,string outputfile)
{
    
    fstream file(filename,ios::in);
    string line;
    string stream;
    int originalLength=0;
    char ch;
    while (file.get(ch)) {
        stream += encoded[ch]; 
        originalLength++;
    }

    ofstream out(outputfile,ios::binary);
    ab.convert(stream);

    int magicNum=1;
    out.write((char*)&magicNum,sizeof(magicNum));

    string ext = "";
    size_t pos = filename.find_last_of(".");
    if (pos != string::npos) {
        ext = filename.substr(pos);  
    }

    uint8_t extLen = ext.size();
    out.write((char*)&extLen, 1);
    out.write(ext.c_str(), extLen);



    uint16_t NumberOfCodes=encoded.size();
    out.write(reinterpret_cast<char*>(&NumberOfCodes), sizeof(uint16_t));
    
    for(const auto&pair:encoded)
    {
        char character=pair.first;
        string code=pair.second;
        uint8_t code_lenght=code.length();

        out.write(&character, 1);

        out.write(reinterpret_cast<char*>(&code_lenght), 1);

        uint8_t packed_bytes = (code_lenght + 7) / 8;  
        for (int i = 0; i < packed_bytes; i++) {
            uint8_t byte_value = 0;
            for (int j = 0; j < 8 && (i * 8 + j) < code_lenght; j++) {
                if (code[i * 8 + j] == '1') {
                    byte_value |= (1 << (7 - j));  
                }
            }
            out.write(reinterpret_cast<char*>(&byte_value), 1);
        }
    }

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
void decodeToFile( string binFile, string outFile) {
    ifstream in(binFile, ios::binary);
    if (!in) {
        cerr << "Error opening compressed file!\n";
        return;
    }

        //     //--------
    //  4 bytes --> magic number used to make sure that this bin file is hoff[4 bytes]
    //   file extension --> string
    //   Number of codes --> 2 bytes
    //   [code table]--> char : code
    //   original length[4 bytes]
    //   padding bits  [1 byte]
    //   bitstream --> 
    int magicNum;
    in.read((char*)&magicNum,sizeof(magicNum));


   uint8_t extLen;
    in.read((char*)&extLen, 1);
    string ext(extLen, '\0');
    in.read(&ext[0], extLen);

    if (outFile.find_last_of(".") == string::npos && !ext.empty()) {
        outFile += ext;
    }



    uint16_t numOfCodes;
    in.read((char*)&numOfCodes,sizeof(numOfCodes));
    

    unordered_map<char,string> encodeTable;

    for (int i = 0; i < numOfCodes; i++) {
        char ch;
        in.read(&ch, 1);

        uint8_t code_length;
        in.read(reinterpret_cast<char*>(&code_length), 1);

        uint8_t packed_bytes = (code_length + 7) / 8;
        string code = "";

        for (int j = 0; j < packed_bytes; j++) {
            uint8_t byte_value;
            in.read(reinterpret_cast<char*>(&byte_value), 1);

            for (int k = 7; k >= 0 && (j * 8 + (7 - k)) < code_length; k--) {
                code += ((byte_value >> k) & 1) ? '1' : '0';
            }
        }

        encodeTable[ch] = code;
    }

    Tree*t = new Tree();
    t->rebuildTree(encodeTable);

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
    
    string decoded = t->DecodeStream(bitstream);
    

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
int main(int argc, char** argv)
{
  if (argc < 4) {
        cerr << "Usage:\n"
             << "  " << argv[0] << " -c <input_file> <output_file>\n"
             << "  " << argv[0] << " -d <input_file> <output_file>\n"
             << "Examples:\n"
             << "  " << argv[0] << " -c input.txt compressed.bin\n"
             << "  " << argv[0] << " -d compressed.bin output.txt\n";
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    if (mode == "-c") {
        cout << "compressing " << inputFile << " -> " << outputFile << "\n";

        readFile(inputFile);

        if (frequencyOfChar.empty()) {
            cerr << "Error??: Input file is empty or could not be read  \n";
            return 1;
        }

        Tree t(frequencyOfChar);
        t.init();
        t.BuildHofmanTree();
        t.encode();
        encoded = t.encoded;

        AccomulateBuffer buffer;
        encodeToFile(buffer, inputFile, outputFile);

        cout << "compression completed successfully!\n";
    }
    else if (mode == "-d") {
        cout << "Decompressing " << inputFile << " -> " << outputFile << "\n";
        decodeToFile(inputFile, outputFile);
        cout << "Decompression completed successfully!\n";
    }
    else {
        cerr << "unknown mode: " << mode << "\n";
        cerr << "Use -c for compression or -d for decompression\n";
        return 1;
    }

    return 0;
}