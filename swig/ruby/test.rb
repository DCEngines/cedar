#!/usr/bin/env ruby
require 'cedar'

trie = Cedar::Trie.new

keys = queries = ARGV.length > 0 ? ARGV[0] : "/usr/share/dict/words"

t = Time.now
f = open(keys)
n = 0
while f.gets do
  $_.chop!
  n += 1
  trie[$_] = n
#  trie.insert($_, n)
end
f.close
STDERR.printf("insert %f sec.\n", (Time.now - t))

t = Time.now
f = open(queries)
while f.gets do
  $_.chop!
  trie[$_]
#  trie.lookup($_)
end
f.close
STDERR.printf("lookup %f sec.\n", (Time.now - t))

t = Time.now
trie.prefix("cedar").each do |r|
  STDERR.printf("%d %s\n", r.value, r.key)
end
STDERR.printf("prefix %f sec.\n", (Time.now - t))

t = Time.now
r = trie.longest_prefix("cedarwoo")
STDERR.printf("%d %s\n", r.value, r.key)
STDERR.printf("longest prefix %f sec.\n", (Time.now - t))

t = Time.now
trie.predict("cedar").each do |r|
  STDERR.printf("%d %s\n", r.value, r.key)
end
STDERR.printf("predict %f sec.\n", (Time.now - t))
