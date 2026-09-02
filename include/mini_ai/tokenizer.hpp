#pragma once
#include <cstddef>
#include <string>
#include <vector>
namespace mini_ai {
// DOCUMENTACIÓN_AQUÍ: El tokenizador conserva cada byte (0..255), haciendo que el
// ejemplo sea reversible y no dependa de un vocabulario externo.
class ByteTokenizer {
  public:
    static constexpr size_t vocab_size = 256;
    std::vector<int> encode(const std::string &s) const {
        std::vector<int> r;
        for (unsigned char c : s)
            r.push_back(c);
        return r;
    }
    std::string decode(const std::vector<int> &x) const {
        std::string s;
        for (int c : x)
            s.push_back(char(c & 255));
        return s;
    }
};
} // namespace mini_ai
