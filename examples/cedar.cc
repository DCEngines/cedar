// cedar -- C++ implementation of Efficiently-updatable Double ARray trie
//  $Id: cedar.cc 1853 2014-06-20 15:04:03Z ynaga $
// Copyright (c) 2013-2014 Naoki Yoshinaga <ynaga@tkl.iis.u-tokyo.ac.jp>
#include <cstdio>
#include <cstdlib>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef USE_PREFIX_TRIE
#include <cedarpp.h>
#else
#include <cedar.h>
#endif
#include <gflags/gflags.h>
#include <memory>

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (argc < 2)
    { std::fprintf (stderr, "Usage: %s trie\n", argv[0]); std::exit (1); }

  typedef cedar::da <int> trie_t;
  trie_t trie;
  if (trie.open_with_mmap(argv[1]))
    { std::fprintf (stderr, "cannot open: %s\n", argv[1]); std::exit (1); }

  const size_t num_keys = trie.num_keys();
  //
  std::unique_ptr<trie_t::result_pair_type[]>   result_pair (new trie_t::result_pair_type[num_keys]);
  std::unique_ptr<trie_t::result_triple_type[]> result_triple (new trie_t::result_triple_type[num_keys]);

  trie.dump(result_pair.get(), num_keys);

  char line[8192];
  while (std::fgets (line, 8192, stdin)) {
    line[std::strlen (line) - 1] = '\0';
    std::fprintf (stdout, "commonPrefixSearch ():\n");
    if (const size_t n = trie.commonPrefixSearch (line, result_pair.get(), num_keys)) {
      std::fprintf (stdout, "%s: found, num=%ld ", line, n);
      for (size_t i = 0; i < n; ++i) {
        if (i >= num_keys) { std::fprintf (stdout, " ..truncated"); break; }
        std::fprintf (stdout, " %d:%ld", result_pair[i].value, result_pair[i].length);
      }
      std::fprintf (stdout, "\n");
    } else {
      std::fprintf (stdout, "%s: not found\n", line);
    }
    char suffix[1024];
    std::fprintf (stdout, "commonPrefixPredict ():\n");
    if (const size_t n = trie.commonPrefixPredict (line, result_triple.get(), num_keys)) {
      std::fprintf (stdout, "%s: found, num=%ld ", line, n);
      for (size_t i = 0; i < n; ++i) {
        if (i >= num_keys) { std::fprintf (stdout, " ..truncated"); break; }
        trie.suffix (suffix, result_triple[i].length, result_triple[i].id);
        std::fprintf (stdout, " %d:%ld:%ld:%s", result_triple[i].value, result_triple[i].length, result_triple[i].id, suffix);
      }
      std::fprintf (stdout, "\n");
    } else {
      std::fprintf (stdout, "%s: not found\n", line);
    }
  }
  return 0;
}
  
