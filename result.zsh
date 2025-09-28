#!/bin/zsh

setopt numericglobsort

for i in *.log; do
echo -n "$i:r "; cavr -s 2 $i
done | awk '{print $1, $2*1000000, $4}'  > result
