#include <vector>
#include <string>
#include <functional>
#include <set>

typedef cedar::da <int> trie_int_t;

template <class S>
auto powerset(const S& s)
{
    std::set<S> ret;
    ret.emplace();
    for (auto&& e: s) {
        std::set<S> rs;
        for (auto x: ret) {
            x.insert(e);
            rs.insert(x);
        }
        ret.insert(begin(rs), end(rs));
    }
    return ret;
}

void permute(std::string& str, std::function<void(const std::string&)> callback,
        size_t first = 0) {
    if (first == str.size()) {
        if (callback) {
            callback(str);
        }
        return;
    }

    for (auto current = first; current < str.size(); ++current) {
        std::swap(str[first], str[current]);
        permute(str, callback, first + 1);
        /* restore original value */
        std::swap(str[first], str[current]);
    }
}

TEST(cderapp, insert_all_permutations) {
    /*
     * Given 2 characters in string a and b -- the test adds
     * a, ab, ba, b in trie
     */
    std::set<char> characters{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
    std::vector<std::string> perm;

    for (auto&& subset : powerset(characters)) {
        if (subset.empty()) {
            continue;
        }

        std::string start;
        for (auto&& e : subset) {
            start += e;
        }

        permute(start, [&perm] (const std::string& str) {
                perm.emplace_back(str);
        });
    }

    auto nelements = perm.size();

    {
	    trie_int_t trie;
	    for (size_t i = 0; i < nelements; ++i) {
	    	/* add a string to trie with value as location in vector */
	    	trie.update(perm[i].c_str(), perm[i].length(), i);
	    }
	    EXPECT_EQ(trie.num_keys(), nelements);

		for (size_t i = 0; i < nelements; ++i) {
			trie_int_t::result_triple_type r;
			r = trie.exactMatchSearch<decltype(r)>(perm[i].c_str(), perm[i].length());
			/* make sure value matches */
			EXPECT_EQ(r.value, i);
			EXPECT_EQ(r.length, perm[i].length());
		}
	}

    {
    	/* now reverse the vector and rerun the test */
    	std::reverse(perm.begin(), perm.end());

	    trie_int_t trie;
	    for (size_t i = 0; i < nelements; ++i) {
	    	trie.update(perm[i].c_str(), perm[i].length(), i);
	    }
	    EXPECT_EQ(trie.num_keys(), nelements);

		for (size_t i = 0; i < nelements; ++i) {
			trie_int_t::result_triple_type r;
			r = trie.exactMatchSearch<decltype(r)>(perm[i].c_str(), perm[i].length());
			EXPECT_EQ(r.value, i);
			EXPECT_EQ(r.length, perm[i].length());
		}
	}
}

/**
 */
TEST(cedarpp, match_and_predict) {
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
