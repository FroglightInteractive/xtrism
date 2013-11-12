#!/usr/bin/perl

while (<>) {
  if (/\/(\d)(\d)(\d)/) {
    $r = ($1+.5)/4;
    $g = ($2+.5)/4;
    $b = ($3+.5)/4;
    $R = sprintf('%.3f',$r);
    $G = sprintf('%.3f',$g);
    $B = sprintf('%.3f',$b);
    s/\d\d\d/$B,$G,$R/;
  }
  print $_;
}
