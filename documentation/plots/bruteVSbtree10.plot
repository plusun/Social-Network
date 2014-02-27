# Gnuplot script file for comparison between brute-force and B-tree versions
# This file is bruteVSbtree.plot

set autoscale
unset log
unset label
set xtic auto
set ytic auto
set title "Brute-force VS B-tree (10% of creations)"
set xlabel "Operations"
set ylabel "Time (s)"
plot "N5Lot10.result" using 1:2 title 'Brute-force' with linespoints, \
     "btreeN5Lot10.result" using 1:2 title 'B-tree(1-degree)' with linespoints
set term png
set output "bruteVSbtree10.png"
replot
unset term