
#include <cstdio>
#include <cstdlib>
#include <cedar_config.h>

#ifdef USE_PREFIX_TRIE
#include <cedarpp.h>
#else
#include <cedar.h>
#endif

#include <gtest/gtest.h>

typedef cedar::da <int> trie_int_t;

TEST(cedar, exactMatch) {
	trie_int_t trie;

	static size_t constexpr kMaxStrings = 3;
	static const char* strings[kMaxStrings] = {
		"abc",
		"abcd",
		"abcde",
	};

	for (size_t i = 0; i < kMaxStrings; i++) {
		trie.update(strings[i], strlen(strings[i]), i);
	}

	for (size_t i = 0; i < kMaxStrings; i++) {
		trie_int_t::result_triple_type result;
		result = trie.exactMatchSearch<decltype(result)>(strings[i], strlen(strings[i]));
		EXPECT_EQ(result.value, i);
		EXPECT_EQ(result.length, strlen(strings[i]));
	}

	{
		trie_int_t::result_triple_type result;
		result = trie.exactMatchSearch<decltype(result)>("ab");
		EXPECT_EQ(result.value, trie_int_t::CEDAR_NO_VALUE);
	}
	{
		trie_int_t::result_triple_type result;
		result = trie.exactMatchSearch<decltype(result)>("abcdef");
		EXPECT_EQ(result.value, trie_int_t::CEDAR_NO_VALUE);
	}
}

int main(int argc, char **argv) {

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
