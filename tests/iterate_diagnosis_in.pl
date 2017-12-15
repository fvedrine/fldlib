#!/usr/bin/perl
use strict;
use warnings;

use IO::File;
use File::ReadBackwards;

use autodie; # die if problem reading or writing a file

my $fh_in = IO::File->new($ARGV[0] . "_out", "r") or die;
my $condition = "";

my $line_number = 0;
while ( my $line = $fh_in->getline() ) {
  $line_number = $line_number + 1;
  if (length($line) > 0)
     { $condition = $line; }
}

if (($condition ne "path condition: 0\n") and ($condition ne "path condition: 0 0\n")) {
  $fh_in->seek(0, 0);

  my $fh_out = IO::File->new($ARGV[0] . "_in", "w");
  $fh_out->print($condition);
  while ( ($line_number > 1) and my $line = $fh_in->getline() ) {
    $line_number = $line_number - 1;
    $fh_out->print($line);
  }
  undef $fh_out;
}
else {
  if (-e ($ARGV[0] . "_in"))
    { unlink ($ARGV[0] . "_in"); }
}

undef $fh_in;

