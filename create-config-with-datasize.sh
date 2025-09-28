#!/bin/sh

for i in {0..12}; do
    data_size=$((2**${i}))
    sed -e s"|%data_size%|$data_size|" source-sink.xml.in > source-sink-${data_size}kB.xml
done
