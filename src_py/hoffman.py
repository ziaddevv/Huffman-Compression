import struct
from heapq import heapify, heappop, heappush
from collections import Counter

class TreeNode:
    def __init__(self, character: str, frequency: int, left_child: 'TreeNode' = None, right_child: 'TreeNode' = None):
        self.character = character
        self.frequency = frequency
        self.left_child = left_child
        self.right_child = right_child
    
    def __lt__(self, other: 'TreeNode'):
        return self.frequency < other.frequency

class HuffmanEncoder:
    @staticmethod
    def decode_file(input_path: str, output_path: str):
        with open(input_path, 'rb') as file:
            lookup_table = HuffmanEncoder.read_lookup_table(file)
            binary_string = HuffmanEncoder.read_binary_data(file)
            decoded_text = HuffmanEncoder.decode_binary_data(binary_string, lookup_table)
        try:
            open(output_path, 'w').write(decoded_text)
        except:
            print(f'Can not write decoded data to file {output_path}')

    def decode_binary_data(binary_string: str, lookup_table: dict[str, str]):
        current_code, final_result = "", ""
        for bit in binary_string:
            current_code += bit
            if current_code in lookup_table:
                final_result += lookup_table[current_code]
                current_code = ""
        return final_result

    def read_binary_data(file) -> str:
        total_bits = struct.unpack('i', file.read(4))[0]
        padding_bits = struct.unpack('i', file.read(4))[0]
        byte_data = file.read((total_bits + 7) // 8)
        binary_string = ''.join(format(byte, '08b') for byte in byte_data)
        return binary_string[:-padding_bits] if padding_bits > 0 else binary_string

    @staticmethod
    def read_lookup_table(file) -> dict[str, str]:
        lookup_table: dict[str, str] = {}
        num_entries = struct.unpack('<I', file.read(4))[0]
        for _ in range(num_entries):
            symbol = file.read(1).decode('utf-8')
            code_length = struct.unpack('<I', file.read(4))[0]
            binary_code = file.read(code_length).decode('utf-8')
            lookup_table[binary_code] = symbol
        return lookup_table

    @staticmethod
    def encode_file(source_file: str, target_file: str):
        with open(source_file, 'r', encoding='utf-8-sig') as f:
            content = f.read()
        encoding_table = {}
        tree_root = HuffmanEncoder.create_huffman_tree(content)
        HuffmanEncoder.create_encoding_table(tree_root, encoding_table)
        with open(target_file, 'wb') as file:
            HuffmanEncoder.save_encoding_table(encoding_table, file)
            encoded_bits = HuffmanEncoder.encode_text(content, encoding_table)
            padding_needed = (8 - (len(encoded_bits) % 8)) % 8
            encoded_bits += padding_needed * '0'
            file.write(struct.pack('i', len(encoded_bits)))
            file.write(struct.pack('i', padding_needed))
            file.write(int(encoded_bits, 2).to_bytes(
                (len(encoded_bits) + 7) // 8, byteorder='big'))

    @staticmethod
    def save_encoding_table(encoding_table: dict[str, str], file):
        file.write(struct.pack('i', len(encoding_table)))
        for symbol in encoding_table:
            binary_code = encoding_table[symbol]
            file.write(symbol.encode())
            file.write(struct.pack('i', len(binary_code)))
            file.write(binary_code.encode())

    @staticmethod
    def write_encoded_data(encoded_string: str, file):
        binary_data = HuffmanEncoder.encode_text(encoded_string)
        file.write(struct.pack('i', len(binary_data)))
        file.write(binary_data)

    @staticmethod
    def encode_text(input_text: str, encoding_table: dict[str, str]) -> str:
        return ''.join([encoding_table[char] for char in input_text])

    @staticmethod
    def create_encoding_table(tree_root: TreeNode, encoding_table: dict[str, str] = {}, current_code: str = ""):
        if not tree_root:
            return
        if tree_root.character and tree_root.character not in encoding_table:
            encoding_table[tree_root.character] = current_code
        if tree_root.left_child:
            HuffmanEncoder.create_encoding_table(tree_root.left_child, encoding_table, current_code + '0')
        if tree_root.right_child:
            HuffmanEncoder.create_encoding_table(tree_root.right_child, encoding_table, current_code + '1')

    @staticmethod
    def create_huffman_tree(input_text: str) -> TreeNode:
        if not input_text:
            return
        frequency_map = Counter(input_text)
        node_heap = [TreeNode(symbol, count) for symbol, count in frequency_map.items()]
        heapify(node_heap)
        while len(node_heap) > 1:
            left_node, right_node = heappop(node_heap), heappop(node_heap)
            combined_freq = left_node.frequency + right_node.frequency
            heappush(node_heap, TreeNode(None, combined_freq, left_node, right_node))
        return heappop(node_heap)