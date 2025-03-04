#include "huffman.h"
#include <bitset>
#include <queue>

Huffman::Huffman() : root(nullptr) {}

Huffman::~Huffman() {
  deleteTree(root);
}

void Huffman::deleteTree(HuffmanNode* node) {
  if (node) {
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }
}

void Huffman::buildCodes(HuffmanNode* node, const std::string& code) {
  if (!node) return;
  // Leaf node
  if (!node->left && !node->right) {
    huffmanCodes[node->value] = code;
  }
  buildCodes(node->left, code + "0");
  buildCodes(node->right, code + "1");
}

void Huffman::buildTree(const std::vector<int16_t>& data) {
  // Create frequency map
  std::unordered_map<int16_t, int> freqMap;
  for (int16_t num : data) {
    freqMap[num]++;
  }

  // Build min-heap of Huffman nodes
  std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;
  for (const auto& pair : freqMap) {
    pq.push(new HuffmanNode(pair.first, pair.second));
  }

  // Combine nodes until one remains
  while (pq.size() > 1) {
    HuffmanNode* left = pq.top();
    pq.pop();
    HuffmanNode* right = pq.top();
    pq.pop();

    HuffmanNode* parent = new HuffmanNode(-1, left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    pq.push(parent);
  }
  root = pq.top();
  pq.pop();

  // Build the Huffman codes from the tree
  huffmanCodes.clear();
  buildCodes(root, "");
}

std::vector<uint16_t> Huffman::encode(const std::vector<int16_t>& flattenedData) {
  std::string bitString;
  // Convert each value to its Huffman code
  for (int16_t num : flattenedData) {
    bitString += huffmanCodes[num];
  }

  std::vector<uint16_t> encodedData;
  // Pack bits into 16-bit words
  for (size_t i = 0; i < bitString.size(); i += 16) {
    std::string chunk = bitString.substr(i, 16);
    // Pad with zeros if chunk is less than 16 bits
    while (chunk.size() < 16) {
      chunk.push_back('0');
    }
    uint16_t value = static_cast<uint16_t>(std::bitset<16>(chunk).to_ulong());
    encodedData.push_back(value);
  }
  return encodedData;
}

std::vector<int16_t> Huffman::decode(const std::vector<uint16_t>& encodedData, int bitSize) {
  // Reconstruct the full bit string from the encoded data
  std::string bitString;
  for (uint16_t word : encodedData) {
    bitString += std::bitset<16>(word).to_string();
  }
  // Use only the valid number of bits (remove padding)
  if (bitString.size() > static_cast<size_t>(bitSize)) {
    bitString = bitString.substr(0, bitSize);
  }

  std::vector<int16_t> decodedData;
  HuffmanNode* current = root;
  for (char bit : bitString) {
    current = (bit == '0') ? current->left : current->right;
    // If a leaf node is reached, output the value and restart from the root
    if (!current->left && !current->right) {
      decodedData.push_back(current->value);
      current = root;
    }
  }
  return decodedData;
}

