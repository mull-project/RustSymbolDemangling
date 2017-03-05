
#include "RustSymbolDemangle.h"

#include <assert.h>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

using namespace std;

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
  Demangle demangle = rustdemangle(s);

  std::string result = rustdemangleDisplay(demangle, skipHashes);

  return result;
}

Demangle rustdemangle(std::string &s) {

  bool valid = true;

  std::string inner = s;

  if (s.size() > 4 &&
      s.compare(0, strlen("_ZN"), "_ZN") == 0 &&
      *s.rbegin() == 'E') {

    inner = s.substr(3, s.size() - 1 - 3);
  } else {
    valid = false;
  }

  std::vector<std::string> elements;

  if (valid == false) {
    Demangle result = {
      .original = s,
      .valid = false,
      .elements = {},
      .inner = inner
    };

    return result;
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

    elements.push_back(element);
  }

  Demangle result = {
    .original = s,
    .valid = valid,
    .elements = elements,
    .inner = inner
  };

  return result;
}

bool RustSymbolIsHash(std::string &string) {
  if (string.at(0) != 'h') {
    return false;
  }

  return all_of(string.begin() + 1, string.end(), [] (const char &c) {
    return ishexnumber((int)c);
  });
}

std::string rustdemangleDisplay(Demangle demangle, bool skipHashes) {
  assert(demangle.valid);

  std::string result;

  for (int index = 0; index < demangle.elements.size(); ++index) {
    std::string &component = demangle.elements.at(index);

    if (skipHashes &&
        index == (demangle.elements.size() - 1) &&
        RustSymbolIsHash(component)) {
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
