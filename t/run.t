#!/usr/bin/perl
# run.t
# Copyright (c) 2006 Jonathan Rockway <jrockway@cpan.org>

use FindBin '$Bin';
use Test::More;

my @templates = glob "$Bin/templates/*";
my $executable = "$Bin/../template";

plan tests => 2*scalar @templates;

foreach my $template (@templates){
    $output = `$executable < $template`;
    ok($output);
    ok($? == 0, 'no errors');
}
