#!/usr/bin/env python
import sys, time, cedar

trie = cedar.trie ()

keys = queries = len (sys.argv) > 1 and sys.argv[1] or "/usr/share/dict/words"

t = time.clock ()
n = 0
for line in open (keys):
    n += 1
    trie[line[:-1]] = n
#    trie.insert (line[:-1], n)
print >> sys.stderr, "insert %f sec." % (time.clock () - t)

t = time.clock ()
for line in open (queries):
    trie[line[:-1]]
#    trie.lookup (line[:-1])
print >> sys.stderr, "lookup %f sec." % (time.clock () - t)

t = time.clock ()
for r in trie.prefix ("cedar"):
    print >> sys.stderr, r.value (), r.key ()
print >> sys.stderr, "prefix %f sec." % (time.clock () - t)

t = time.clock ()
r = trie.longest_prefix ("cedarwoo")
print >> sys.stderr, r.value (), r.key ()
print >> sys.stderr, "longest prefix %f sec." % (time.clock () - t)

t = time.clock ()
for r in trie.predict ("cedar"):
    print >> sys.stderr, r.value (), r.key ()
print >> sys.stderr, "predict %f sec." % (time.clock () - t)
