#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

class Huffman {
private:
  struct HuffmanNode {
    int16_t value;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(int16_t v, int f) : value(v), freq(f), left(nullptr), right(nullptr) {}
  };

  struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
      return a->freq > b->freq;  // Min-heap based on frequency
    }
  };

  HuffmanNode* root;
  std::unordered_map<int16_t, std::string> huffmanCodes;

  void buildCodes(HuffmanNode* node, const std::string& code);
  void deleteTree(HuffmanNode* node);

public:
  Huffman();
  ~Huffman();

  // Build the Huffman tree from a flattened vector of int16_t values.
  void buildTree(const std::vector<int16_t>& data);

  // Encode the flattened data into a vector of uint16_t (packed bits)
  std::vector<uint16_t> encode(const std::vector<int16_t>& flattenedData);

  // Decode the compressed data back into a flattened vector of int16_t.
  // 'bitSize' is the total number of valid bits in the encoded bit stream.
  std::vector<int16_t> decode(const std::vector<uint16_t>& encodedData, int bitSize);
};

#endif // HUFFMAN_H

