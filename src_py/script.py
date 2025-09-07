import sys
from os.path import getsize
from hoffman import HuffmanEncoder

def run_compression():
    command_args = sys.argv
    if len(command_args) < 3 or (len(command_args) > 4 and command_args[1] == '-d') or (len(command_args) > 3 and command_args[1] != '-d'):
        print("Invalid arguments")
        return
    
    if command_args[1] == '-d':
        HuffmanEncoder.decode_file(command_args[2], command_args[3])
    else:
        HuffmanEncoder.encode_file(command_args[1], command_args[2])
        original_size, encoded_size = getsize(command_args[1]), getsize(command_args[2])
        print("Original file size:", original_size)
        print("compressed file size:", encoded_size)

if __name__ == "__main__":
    run_compression()