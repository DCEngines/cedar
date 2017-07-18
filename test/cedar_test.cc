
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

	EXPECT_EQ(trie.num_keys(), kMaxStrings);

	for (size_t i = 0; i < kMaxStrings; i++) {
		trie_int_t::result_triple_type result;
		result = trie.exactMatchSearch<decltype(result)>(strings[i], strlen(strings[i]));
		EXPECT_EQ(result.value, i);
		EXPECT_EQ(result.length, strlen(strings[i]));
	}

	for (size_t i = 0; i < kMaxStrings; i++) {
		trie_int_t::result_pair_type prefixResult[kMaxStrings];
		size_t num_res = trie.commonPrefixSearch(strings[i], prefixResult, kMaxStrings);
		EXPECT_EQ(num_res, i + 1);
		for (size_t j = 0; j < num_res; j++) {
			EXPECT_EQ(prefixResult[j].value, j);
		}
	}

	for (size_t i = 0; i < kMaxStrings; i++) {
		trie_int_t::result_triple_type prefixResult[kMaxStrings];
		size_t num_res = trie.commonPrefixPredict(strings[i], prefixResult, kMaxStrings);
		EXPECT_EQ(num_res, kMaxStrings - i);
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
	{
		// no inserted strings are prefix of "ab"
		trie_int_t::result_pair_type prefixResult[kMaxStrings];
		size_t num_res = trie.commonPrefixSearch("ab", prefixResult, kMaxStrings);
		EXPECT_EQ(num_res, 0);
	}
	{
		// all inserted strings are prefix of "abcdef"
		trie_int_t::result_pair_type prefixResult[kMaxStrings];
		size_t num_res = trie.commonPrefixSearch("abcdef", prefixResult, kMaxStrings);
		EXPECT_EQ(num_res, kMaxStrings);
	}
	{
		// all inserted strings are completions of "ab"
		trie_int_t::result_triple_type prefixResult[kMaxStrings];
		size_t num_res = trie.commonPrefixPredict("ab", prefixResult, kMaxStrings);
		EXPECT_EQ(num_res, kMaxStrings);
	}
	{
		// no inserted strings are completions of "abcdef"
		trie_int_t::result_triple_type prefixResult[kMaxStrings];
		size_t num_res = trie.commonPrefixPredict("abcdef", prefixResult, kMaxStrings);
		EXPECT_EQ(num_res, 0);
	}
}

int main(int argc, char **argv) {

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
