
#include "RustSymbolDemangling.h"

#include <assert.h>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

using namespace std;

bool RSDParseComponents(std::string &inputString,
                        std::vector<std::string>& components);

std::string RSDDemangleComponents(std::vector<std::string>& components,
                                  bool skipHashes = false);

bool RSDIsRustHash(std::string &string);

static map<string, string> RustDemangleDollarMap = {
  { "$SP$", "@" },
  { "$BP$", "*" },
  { "$RF$", "&" },
  { "$LT$", "<" },
  { "$GT$", ">" },
  { "$LP$", "(" },
  { "$RP$", ")" },
  { "$C$" , "," },

  // in theory we can demangle any Unicode code point, but
  // for simplicity we just catch the common ones.
  { "$u7e$", "~" },
  { "$u20$", " " },
  { "$u27$", "'" },
  { "$u5b$", "[" },
  { "$u5d$", "]" },
  { "$u7b$", "{" },
  { "$u7d$", "}" },
  { "$u3b$", ";" },
  { "$u2b$", "+" },
  { "$u22$", "\""}
};

std::string RustSymbolDemangle(std::string &s, bool skipHashes) {
  vector<string> components;
  bool success = RSDParseComponents(s, components);

  if (success == false) return std::string(s);

  std::string result = RSDDemangleComponents(components, skipHashes);
  return result;
}

bool RSDIsRustHash(std::string &string) {
  if (string.at(0) != 'h') {
    return false;
  }

  return all_of(string.begin() + 1, string.end(), [] (const char &c) {
    return ishexnumber((int)c);
  });
}

bool RSDParseComponents(std::string &s, vector<string>& components) {

  bool valid = true;

  std::string inner = s;

  if (s.size() > 4 &&
      s.compare(0, strlen("_ZN"), "_ZN") == 0 &&
      *s.rbegin() == 'E') {

    inner = s.substr(3, s.size() - 1 - 3);
  } else {
    valid = false;
  }

  if (valid == false) {
    return false;
  }

  string::iterator current = inner.begin();

  while (valid) {
    int i = 0;

    // 0-9 => 48-57 in ASCII
    for (; current != inner.end(); ++current) {

      if (isdigit(*current)) {
         i = i * 10 + *current - (int)'0';
      } else {
         break;
      }
    }

    if (i == 0) {
      valid = (current == inner.end());

      current++;

      break;
    }

    if ((current + i) > inner.end()) {
      valid = false;
      break;
    }

    std::string element(current, current + i);

    current += i;

    components.push_back(element);
  }

  return true;
}

std::string RSDDemangleComponents(vector<string>& components, bool skipHashes) {
  std::string result;

  for (int index = 0; index < components.size(); ++index) {
    std::string &component = components.at(index);

    if (skipHashes &&
        index == (components.size() - 1) &&
        RSDIsRustHash(component)) {
      break;
    }

    if (index != 0) {
      result.append("::");
    }

    int componentStartIndex = 0;

    if (component.compare(0, strlen("_$"), "_$") == 0) {
      componentStartIndex = 1;
    }

    while (componentStartIndex < component.size()) {
      if (component.compare(componentStartIndex, strlen("."), ".") == 0) {
        if ((componentStartIndex + 1) < component.size() &&
            component.compare(componentStartIndex + 1, strlen("."), ".") == 0) {
          result.append("::");

          componentStartIndex += 2;
        } else {
          result.append(".");

          componentStartIndex += 1;
        }

        continue;
      }

      if (component.compare(componentStartIndex, strlen("$"), "$") == 0) {

        for (auto it = RustDemangleDollarMap.begin();
             it != RustDemangleDollarMap.end();
             ++it) {

          string key = it->first;

          if (component.compare(componentStartIndex, key.length(), key) == 0) {

            string &value = it->second;

            result.append(value);

            componentStartIndex += key.length();

            break;
          }
        }

        continue;
      }

      result.append(component.substr(componentStartIndex, 1));
      componentStartIndex++;
    }
  }

  return result;
}
