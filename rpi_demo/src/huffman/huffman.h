#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <bitset>
#include <cmath>
#include <chrono>

class FixedPoint {
private:
  static const int SCALE = 16384;    
  static const int FRACTION_BITS = 14;
  int32_t value;

public:
  FixedPoint(float decimal = 0);
  std::string toBinary() const;
  float toDecimal() const;
  bool operator==(const FixedPoint& other) const;
};

class HuffmanCompression {
private:
  struct Node {
    std::string value;
    int frequency;
    Node* left;
    Node* right;
    Node(std::string v, int f);
  };

  Node* root;
  std::map<std::string, std::string> codes;
  void generateCodes(Node* node, std::string code);

public:
  HuffmanCompression();
  std::string compress(const std::vector<FixedPoint>& data);
  std::vector<FixedPoint> decompress(const std::string& compressed);
  size_t getMemoryUsage() const;
};

#endif // HUFFMAN_H
