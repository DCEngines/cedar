The `enron_benchmark` benchmarks cedar trie. This benchmark is written to compute memory requirements of cedar for
given dataset. The dataset is split in words and each word is added in cedar one after the other. Once the complete
dataset is added, memory required to store the dataset is computed and information is printed on the terminal.

The dataset may be stored in single file or may be spread across multiple files. The only input to `enron_benchmark` is a
file name. The file should contain list of files in which dataset is spread.

For example

1. Prepare list of files 

```
root@ubuntu16:~/workspace/dce/cedar/build# find /dev/shm/maildir/ -type f > files-list.txt 

root@ubuntu16:~/workspace/dce/cedar/build# head files-list.txt 
/dev/shm/maildir/blair-l/acctg___invoicing_info/1.
/dev/shm/maildir/blair-l/customer___midam/1.
/dev/shm/maildir/blair-l/customer___midam/2.
/dev/shm/maildir/blair-l/customer___midam/5.
/dev/shm/maildir/blair-l/customer___midam/4.
/dev/shm/maildir/blair-l/customer___midam/3.
/dev/shm/maildir/blair-l/customer___reliant_retail_energy_minneapolis/1.
/dev/shm/maildir/blair-l/customer___reliant_retail_energy_minneapolis/2.
/dev/shm/maildir/blair-l/customer___reliant_retail_energy_minneapolis/7.
/dev/shm/maildir/blair-l/customer___reliant_retail_energy_minneapolis/4.

root@ubuntu16:~/workspace/dce/cedar/build# wc -l files-list.txt 
36536 files-list.txt
```

2. Run benchmark
```
root@ubuntu16:~/workspace/dce/cedar/build# benchmark/enron_benchmark files-list.txt 
Trie size 40734720
Total number of nodes (used + unused) in trie 4036352
Total number of used nodes 4036150
Total number of words added 11354129
Total number of characters added 78251917
Total number of unique words 380295
Total number of characters in unique words 6964330
```

`Trie size` -- is amount of memory consumed by cedar to store complete dataset


`nodes` -- word in trie is added character by character. Each character causes state transition in trie.
Node represents a state.
