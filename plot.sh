#!/bin/sh

gnuplot <<EOF
set title 'CPU: Core i7 CPU @ 2.80GHz, 2010'
set term pngcairo size 800,600
set output 'latency-colors.png'
set log x
set log y
set xlabel 'kB'
set ylabel 'us'
set grid xtics
set grid ytics
set xrange [0.8:5000]
set xtics (1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096)
plot \
'0/result' u 1:2 w lp pt 18 lc 1 title 'profile = balanced (default)', \
'1/result' u 1:2 w lp pt 9  lc 3 title 'profile = throughput-performance'
EOF

