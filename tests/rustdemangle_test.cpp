
#include "RustSymbolDemangling.h"

#include "gtest/gtest.h"

TEST(rustdemangle, basic_one_component) {
  std::string input = "_ZN4testE";

  Demangle output = rustdemangle(input);

  ASSERT_EQ(output.original, input);
  ASSERT_EQ(output.valid, true);
  ASSERT_EQ(output.inner, "4test");

  std::vector<std::string> expectedElements({ "test" });

  ASSERT_EQ(output.elements, expectedElements);
}

TEST(rustdemangle, basic_many_components) {
  std::string input = "_ZN4test1a2bcE";

  Demangle output = rustdemangle(input);

  ASSERT_EQ(output.original, input);
  ASSERT_EQ(output.valid, true);
  ASSERT_EQ(output.inner, "4test1a2bc");

  std::vector<std::string> expectedElements({ "test", "a", "bc" });

  ASSERT_EQ(output.elements, expectedElements);
}


TEST(rustdemangleDisplay, greenWIP2) {
  std::string original = "_ZN4test1a2bcE";
  std::string inner = "4test1a2bc";
  std::vector<std::string> elements({ "test", "a", "bc" });

  Demangle demangle = {
    .original = original,
    .valid = true,
    .elements = elements,
    .inner = inner
  };

  std::string output = rustdemangleDisplay(demangle);

  ASSERT_EQ(output, "test::a::bc");
}

TEST(rustdemangle, basic) {
  std::string original = "_ZN4test1a2bcE";
  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "test::a::bc");
}

TEST(rustdemangle, dollar_basic1) {
  std::string original = "_ZN4$RP$E";
  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, ")");
}

TEST(rustdemangle, dollar_basic2) {
  std::string original = "_ZN8$RF$testE";
  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "&test");
}

TEST(rustdemangle, dollar_basic3) {
  std::string original = "_ZN35Bar$LT$$u5b$u32$u3b$$u20$4$u5d$$GT$E";

  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "Bar<[u32; 4]>");
}

TEST(rustdemangle, dollar_many) {
  std::string original = "_ZN12test$BP$test4foobE";

  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "test*test::foob");
}

TEST(rustdemangle, trait_impls) {
  std::string original = "_ZN71_$LT$Test$u20$$u2b$$u20$$u27$static$u20$as$u20$foo..Bar$LT$Test$GT$$GT$3barE";

  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "<Test + 'static as foo::Bar<Test>>::bar");
}

TEST(rustdemangle, RustSymbolIsHash) {
  std::string wrongHash1 = "foo";
  ASSERT_EQ(RustSymbolIsHash(wrongHash1), false);

  std::string wrongHash2 = "hoo";
  ASSERT_EQ(RustSymbolIsHash(wrongHash2), false);

  std::string wrongHash3 = "h05af22Ze174051e9E";
  ASSERT_EQ(RustSymbolIsHash(wrongHash3), false);

  std::string correctHash1 = "h05af221e174051e9E";
  ASSERT_EQ(RustSymbolIsHash(correctHash1), true);
}

TEST(rustdemangle, demangle_rustHashes) {
  std::string original = "_ZN3foo17h05af221e174051e9E";

  std::string output = RustSymbolDemangle(original);
  std::string outputWithoutHash = RustSymbolDemangle(original, true);

  ASSERT_EQ(output, "foo::h05af221e174051e9");
  ASSERT_EQ(outputWithoutHash, "foo");
}
