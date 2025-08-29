# Huffman-Compression

A simple file compression and decompression tool based on the **Huffman coding algorithm**.

## Features
- Compresses text and other files into a smaller binary format.
- Decompresses files back to their exact original form.
- Lossless compression using Huffman coding.

## Usage

```bash
# Compile
g++ src/main.cpp src/BTree.cpp src/Buffer.cpp -o main

# Compress
./main -c <input_file> <output_file>
# Output: compression completed successfully!

# Decompress
./main -d <input_file> <output_file>
# Output: decompression completed successfully!
