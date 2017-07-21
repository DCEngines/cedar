
#include <cstdlib>
#include <map>
#include <set>
#include <random>
#include <unistd.h>

namespace {

// Slot map contains slot id of leaf in trie array and length of string
// e.g. if string was "abcd", then slot_map key is slot id of last char "d".
// and you get slot_map[slot_id_of_d] = 4
std::map<int, size_t> slot_map;

// list of strings which were inserted
std::set<std::string> string_set;

// Everytime a char in the trie is relocated to a new slot, 
// this callback is called.  Update the slot_map
// The slot_map is used during calls to suffix to retrieve
// the originally inserted strings
struct MoveSlotCallback {
	void operator() (const int from, const int to) {
		auto iter = slot_map.find(from);
		if (iter != slot_map.end()) {
		  slot_map.insert({to, iter->second});
		  slot_map.erase(iter);
		}
	}
}move_slot_callback;

}

/** 
 * This test verifies that it is possible to retrieve an 
 * already inserted string in the trie using the suffix() method
 */
TEST(cedar, suffix) {

  const int pid = getpid();
  std::mt19937 generator(pid);

  constexpr size_t kMaxLineSize = 400;
  // random generator of characters
  // caution : do not generate char = 0 as trie uses XOR
  std::uniform_int_distribution<int> charGen(97, 122);
  // random generator of string length
  std::uniform_int_distribution<int> lengthGen(5, kMaxLineSize);

  cedar::da <int> trie;
  char line[kMaxLineSize + 1];
  const int num_strings = 100000;
  for (int i = 0; i < num_strings; i++) {
	size_t length = lengthGen(generator);
    for (size_t pos = 0; pos < length; pos ++) {
		line[pos] = charGen(generator);
	}
	size_t from{0};
	size_t pos{0};
	// Insert each string into trie
  	trie.update(line, from, pos, length - 1, 0, move_slot_callback);
	EXPECT_EQ(pos, length - 1);
	// record the <leaf slot id, length> for newly inserted string 
	slot_map.insert({from, length - 1});
	// record string in a set for later processing
	std::string inserted_string(line, pos);
	string_set.insert(inserted_string);
  }

  char result[kMaxLineSize + 1];
  for (auto& slot_iter : slot_map) {
	bzero(result, kMaxLineSize);
	// Suffix() function allows you to retrieve the string from the trie
	// using the leaf slot number and length
	trie.suffix(result, slot_iter.second, slot_iter.first);
	std::string found_string(result, slot_iter.second);
	auto set_iter = string_set.find(found_string);
	EXPECT_NE(set_iter, string_set.end());
	// erase every string which was found
	string_set.erase(set_iter);
  }

  // number of keys in trie is same as number inserted
  EXPECT_EQ(trie.num_keys(), num_strings);
  // all strings must have been erased by now
  EXPECT_TRUE(string_set.empty());
}
