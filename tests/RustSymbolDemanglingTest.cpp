
#include "RustSymbolDemangling.h"

#include "gtest/gtest.h"

#pragma mark - Test: RSDParseComponents()

TEST(RSDParseComponents, basic_one_component) {
  std::string input = "_ZN4testE";

  std::vector<std::string> components;
  bool success = RSDParseComponents(input, components);

  ASSERT_TRUE(success);

  std::vector<std::string> expectedComponents({ "test" });
  ASSERT_EQ(components, expectedComponents);
}

TEST(RSDParseComponents, basic_many_components) {
  std::string input = "_ZN4test1a2bcE";

  std::vector<std::string> components;
  bool success = RSDParseComponents(input, components);

  ASSERT_TRUE(success);

  std::vector<std::string> expectedComponents({ "test", "a", "bc" });

  ASSERT_EQ(components, expectedComponents);
}

#pragma mark - Test: RSDDemangleComponents()

TEST(RSDDemangleComponents, many_components) {
  std::string original = "_ZN4test1a2bcE";
  std::string inner = "4test1a2bc";
  std::vector<std::string> components({ "test", "a", "bc" });

  std::string output = RSDDemangleComponents(components);

  ASSERT_EQ(output, "test::a::bc");
}

#pragma mark - Test: RustSymbolDemangle()

TEST(RustSymbolDemangle, basic) {
  std::string original = "_ZN4test1a2bcE";
  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "test::a::bc");
}

TEST(RustSymbolDemangle, dollar_basic1) {
  std::string original = "_ZN4$RP$E";
  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, ")");
}

TEST(RustSymbolDemangle, dollar_basic2) {
  std::string original = "_ZN8$RF$testE";
  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "&test");
}

TEST(RustSymbolDemangle, dollar_basic3) {
  std::string original = "_ZN35Bar$LT$$u5b$u32$u3b$$u20$4$u5d$$GT$E";

  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "Bar<[u32; 4]>");
}

TEST(RustSymbolDemangle, dollar_many) {
  std::string original = "_ZN12test$BP$test4foobE";

  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "test*test::foob");
}

TEST(RustSymbolDemangle, trait_impls) {
  std::string original = "_ZN71_$LT$Test$u20$$u2b$$u20$$u27$static$u20$as$u20$foo..Bar$LT$Test$GT$$GT$3barE";

  std::string output = RustSymbolDemangle(original);

  ASSERT_EQ(output, "<Test + 'static as foo::Bar<Test>>::bar");
}

TEST(RustSymbolDemangle, demangle_rustHashes) {
  std::string original = "_ZN3foo17h05af221e174051e9E";

  std::string output = RustSymbolDemangle(original);
  std::string outputWithoutHash = RustSymbolDemangle(original, true);

  ASSERT_EQ(output, "foo::h05af221e174051e9");
  ASSERT_EQ(outputWithoutHash, "foo");
}

#pragma mark - Test: RSDIsRustHash()

TEST(RSDIsRustHash, isRustHash) {
  std::string wrongHash1 = "foo";
  ASSERT_EQ(RSDIsRustHash(wrongHash1), false);

  std::string wrongHash2 = "hoo";
  ASSERT_EQ(RSDIsRustHash(wrongHash2), false);

  std::string wrongHash3 = "h05af22Ze174051e9E";
  ASSERT_EQ(RSDIsRustHash(wrongHash3), false);

  std::string correctHash1 = "h05af221e174051e9E";
  ASSERT_EQ(RSDIsRustHash(correctHash1), true);
}
