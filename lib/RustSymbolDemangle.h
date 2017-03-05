
#include <string>
#include <vector>

typedef struct {
  std::string original;
  std::string inner;
  bool valid;
  std::vector<std::string> elements;
} Demangle;

std::string RustSymbolDemangle(std::string &s, bool skipHashes = false);

extern Demangle rustdemangle(std::string &inputString);
std::string rustdemangleDisplay(Demangle demangle, bool skipHashes = false);
bool RustSymbolIsHash(std::string &string);
