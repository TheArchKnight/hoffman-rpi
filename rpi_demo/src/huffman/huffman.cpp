#include "huffman.h"

using namespace std;
using namespace std::chrono;

// Implementación de FixedPoint
FixedPoint::FixedPoint(float decimal) {
  value = round(decimal * SCALE);
}

string FixedPoint::toBinary() const {
  return bitset<16>(value).to_string();
}

float FixedPoint::toDecimal() const {
  return static_cast<float>(value) / SCALE;
}

bool FixedPoint::operator==(const FixedPoint& other) const {
  return value == other.value;
}

// Implementación de HuffmanCompression::Node
HuffmanCompression::Node::Node(string v, int f) : value(v), frequency(f), left(nullptr), right(nullptr) {}

// Implementación de HuffmanCompression
HuffmanCompression::HuffmanCompression() : root(nullptr) {}

void HuffmanCompression::generateCodes(Node* node, string code) {
  if (!node) return;
  if (!node->left && !node->right) {
    codes[node->value] = code;
    return;
  }
  generateCodes(node->left, code + "0");
  generateCodes(node->right, code + "1");
}

string HuffmanCompression::compress(const vector<FixedPoint>& data) {
  auto start = high_resolution_clock::now();

  // Calcular frecuencias
  map<string, int> freq;
  for (const auto& num : data) {
    freq[num.toBinary()]++;
  }

  // Crear cola de prioridad
  auto comp = [](Node* a, Node* b) { return a->frequency > b->frequency; };
  priority_queue<Node*, vector<Node*>, decltype(comp)> pq(comp);

  for (const auto& pair : freq) {
    pq.push(new Node(pair.first, pair.second));
  }

  // Construir árbol
  while (pq.size() > 1) {
    Node* left = pq.top(); pq.pop();
    Node* right = pq.top(); pq.pop();
    Node* parent = new Node("", left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    pq.push(parent);
  }

  root = pq.top();
  generateCodes(root, "");

  // Comprimir datos
  string compressed;
  for (const auto& num : data) {
    compressed += codes[num.toBinary()];
  }

  auto end = high_resolution_clock::now();
  duration<double, milli> duration = end - start;
  cout << "Tiempo de compresión: " << duration.count() << "ms\n";

  return compressed;
}

vector<FixedPoint> HuffmanCompression::decompress(const string& compressed) {
  vector<FixedPoint> result;
  Node* current = root;

  for (char bit : compressed) {
    current = (bit == '0') ? current->left : current->right;

    if (!current->left && !current->right) {
      int32_t value = bitset<16>(current->value).to_ulong();
      result.push_back(FixedPoint(static_cast<float>(value) / 16384));
      current = root;
    }
  }

  return result;
}

size_t HuffmanCompression::getMemoryUsage() const {
  size_t memory = sizeof(HuffmanCompression) + codes.size() * (sizeof(string) * 2);
  
  queue<Node*> q;
  q.push(root);
  while (!q.empty()) {
    Node* node = q.front();
    q.pop();
    memory += sizeof(Node);
    if (node->left) q.push(node->left);
    if (node->right) q.push(node->right);
  }
  
  return memory;
}

