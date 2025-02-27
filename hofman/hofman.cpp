#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <chrono>
#include <bitset>
#include <cmath>
using namespace std;
using namespace std::chrono;

// Implementación de punto fijo optimizada para sistemas embebidos
class FixedPoint {
private:
  static const int SCALE = 16384;    // 2^14
  static const int FRACTION_BITS = 14;
  int32_t value;

public:
  FixedPoint(float decimal = 0) {
    value = round(decimal * SCALE);
  }

  string toBinary() const {
    return bitset<16>(value).to_string();
  }

  float toDecimal() const {
    return static_cast<float>(value) / SCALE;
  }

  bool operator==(const FixedPoint& other) const {
    return value == other.value;
  }
};

// Sistema de compresión Huffman optimizado
class HuffmanCompression {
private:
  struct Node {
    string value;
    int frequency;
    Node* left;
    Node* right;

    Node(string v, int f) : value(v), frequency(f), left(nullptr), right(nullptr) {}
  };

  Node* root;
  map<string, string> codes;

  void generateCodes(Node* node, string code) {
    if (!node) return;
    if (!node->left && !node->right) {
      codes[node->value] = code;
      return;
    }
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
  }

public:
  HuffmanCompression() : root(nullptr) {}

  // Método principal de compresión
  string compress(const vector<FixedPoint>& data) {
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

  // Método de descompresión
  vector<FixedPoint> decompress(const string& compressed) {
    vector<FixedPoint> result;
    Node* current = root;

    for (char bit : compressed) {
      if (bit == '0') {
        current = current->left;
      } else {
        current = current->right;
      }

      if (!current->left && !current->right) {
        int32_t value = bitset<16>(current->value).to_ulong();
        result.push_back(FixedPoint(static_cast<float>(value) / 16384));
        current = root;
      }
    }

    return result;
  }

  // Método para medir uso de memoria
  size_t getMemoryUsage() const {
    size_t memory = 0;
    memory += sizeof(HuffmanCompression);
    memory += codes.size() * (sizeof(string) * 2);
    // Calcular memoria del árbol
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
};

// Función principal de pruebas
int main() {
  // Generar datos de prueba
  vector<FixedPoint> testData;
  for (float i = 0; i < 1000; i++) {
    testData.push_back(FixedPoint(sin(i * 0.01)));
  }

  // Comprimir usando punto fijo
  HuffmanCompression compressor;
  string compressed = compressor.compress(testData);
  vector<FixedPoint> descompressed  = compressor.decompress(compressed);
  // Calcular tasa de compresión
  float originalSize = testData.size() * sizeof(FixedPoint);
  float compressedSize = compressed.length() / 8.0;
  float compressionRate = (1 - compressedSize / originalSize) * 100;
  
  // Mostrar resultados
  //
  cout << compressed << endl;
  cout << "Tamaño original: " << originalSize << " bytes\n";
  cout << "Tamaño comprimido: " << compressedSize << " bytes\n";
  cout << "Tasa de compresión: " << compressionRate << "%\n";
  cout << "Uso de memoria: " << compressor.getMemoryUsage() << " bytes\n";
  
  for (FixedPoint data: descompressed) {
    cout << data.toDecimal() << endl;
  
  }

  return 0;
}
