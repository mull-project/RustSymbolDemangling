
#include <string>
#include <vector>

typedef struct {
  std::string original;
  std::string inner;
  bool valid;
  std::vector<std::string> elements;
} Demangle;

std::string RustSymbolDemangle(std::string &s, bool skipHashes = false);

// These are exposed for unit testing.
Demangle RSDParseComponents(std::string &inputString);
std::string RSDDemangleComponents(Demangle demangle, bool skipHashes = false);
bool RSDIsRustHash(std::string &string);
