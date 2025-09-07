# File Compression Tool
A lightweight file compression and decompression utility implementing the **Huffman encoding algorithm** for efficient data compression.

## Features
- Compresses text and binary files into optimized compressed format
- Decompresses files back to their exact original state
- Lossless compression using adaptive Huffman encoding
- Cross-platform compatibility
- Multiple implementation options (C++ and Python)

## Implementation Options

### Python Version
**Requirements:** Python 3.6 or higher

```bash
# Compress a file
python3 script.py <source_file> <compressed_file>
# Example: python3 script.py document.txt document.huff
# Output: Original file size: 1024
#         compressed file size: 512
#         Compression ratio: 50.0

# Decompress a file
python3 script.py -d <compressed_file> <output_file>
# Example: python3 script.py -d document.huff restored_document.txt
```

### C++ Version 
**Requirements:** GCC compiler or equivalent

```bash
# Compile the source code
g++ src/main.cpp src/BTree.cpp src/Buffer.cpp -o compression_tool

# Compress a file
./compression_tool -c <input_file> <output_file>
# Output: compression completed successfully!

# Decompress a file
./compression_tool -d <input_file> <output_file>
# Output: decompression completed successfully!
```

## File Structure
```
├── src_py/
│   ├── script.py          # Main entry point
│   └── hoffman.py         # Huffman encoding implementation
├── src/                   # C++ implementation (legacy)
│   ├── main.cpp
│   ├── BTree.cpp
│   └── Buffer.cpp
└── README.md
```

## Algorithm Details
The tool uses Huffman coding, which:
- Analyzes character frequency in the input file
- Builds an optimal binary tree for encoding
- Assigns shorter codes to more frequent characters
- Achieves better compression ratios for text with repetitive patterns

## Supported File Types
- Text files (.txt, .md, .log, etc.)
- Source code files
- Any UTF-8 encoded content
- Binary files 
