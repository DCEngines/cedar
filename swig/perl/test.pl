#!/usr/bin/env perl
use strict;
use cedar;
use Time::HiRes;

my $trie  = new cedar::trie ();

my $keys = my $queries = $#ARGV >= 0 ? $ARGV[0] : "/usr/share/dict/words";

my $t = Time::HiRes::clock ();
my $n = 0;
open (IN, $keys);
while (<IN>) {
    $n++;
    chop;
    $trie->insert ($_, $n);
}
close (IN);
printf (STDERR "insert %f sec.\n", (Time::HiRes::clock () - $t));

$t = Time::HiRes::clock ();
open (IN, $queries);
while (<IN>) {
    chop;
    $n = $trie->lookup ($_);
}
close (IN);
printf (STDERR "lookup %f sec.\n", (Time::HiRes::clock () - $t));

my $t = Time::HiRes::clock ();
foreach my $r (@{$trie->prefix ("cedar")}) {
    printf (STDERR "%d %s\n", $r->value, $r->key);
}
printf (STDERR "prefix %f sec.\n", (Time::HiRes::clock () - $t));

my $t = Time::HiRes::clock ();
my $r = $trie->longest_prefix ("cedarwoo");
printf (STDERR "%d %s\n", $r->value, $r->key);
printf (STDERR "longest prefix %f sec.\n", (Time::HiRes::clock () - $t));

my $t = Time::HiRes::clock ();
my $it = $trie->predict ("cedar");
while (my $r = $it->next ()) {
    printf (STDERR "%d %s\n", $r->value, $r->key);
}
printf (STDERR "predict %f sec.\n", (Time::HiRes::clock () - $t));
