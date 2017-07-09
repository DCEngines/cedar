#!/usr/bin/env lua
require ("cedar")

trie = cedar.trie ()

keys    = "/usr/share/dict/words"
queries = "/usr/share/dict/words"
if table.maxn (arg) > 0 then
   keys    = arg[1]
   queries = arg[1]
end

t = os.clock ()
local n = 0
for line in io.lines (keys) do
   n = n + 1
   trie:insert (line, n)
end
io.stderr:write (string.format ("insert %f sec.\n", (os.clock () - t)))

t = os.clock ()
for line in io.lines (queries) do
   n = trie:lookup (line)
end
io.stderr:write (string.format ("lookup %f sec.\n", (os.clock () - t)))

t = os.clock ()
ret = trie:prefix ("cedar")
for i = 0, ret:size () - 1  do
   io.stderr:write (string.format ("%d %s\n", ret[i]:value (), ret[i]:key ()))
end
io.stderr:write (string.format ("prefix %f sec.\n", (os.clock () - t)))

t = os.clock ()
r = trie:longest_prefix ("cedarwoo")
io.stderr:write (string.format ("%d %s\n", r:value (), r:key ()))
io.stderr:write (string.format ("longest prefix %f sec.\n", (os.clock () - t)))

t = os.clock ()
it = trie:predict ("cedar")
r = it:next ()
while r do
   io.stderr:write (string.format ("%d %s\n", r:value (), r:key ()))
   r = it:next ()
end
io.stderr:write (string.format ("predict %f sec.\n", (os.clock () - t)))
