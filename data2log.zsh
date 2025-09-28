#!/bin/zsh

setopt numericglobsort

for i in *.dat; do
../decoder/decode-latency $i > $i:r.log
done

