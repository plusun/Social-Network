# Gnuplot script file for comparison between brute-force and B-tree versions
# This file is bruteVSbtree.plot

set autoscale
unset log
unset label
set xtic auto
set ytic auto
set title "Comparison between different degrees (10% of creations)"
set xlabel "Operations"
set ylabel "Time (s)"
plot "btreeN5Lot10.result" using 1:2 title '1-degree' with linespoints, \
     "btree2N5Lot10.result" using 1:2 title '2-degree' with linespoints, \
     "btree3N5Lot10.result" using 1:2 title '3-degree' with linespoints, \
     "btree4N5Lot10.result" using 1:2 title '4-degree' with linespoints, \
     "btree5N5Lot10.result" using 1:2 title '5-degree' with linespoints, \
     "btree6N5Lot10.result" using 1:2 title '6-degree' with linespoints
set term png
set output "cmp10.png"
replot
unset term