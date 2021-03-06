# Gnuplot script file for comparison between brute-force and B-tree versions
# This file is bruteVSbtree.plot

set autoscale
unset log
unset label
set xtic auto
set ytic auto
set title "Comparison between different degrees (70% of creations)"
set xlabel "Operations"
set ylabel "Time (s)"
plot "N10000.result" using 1:4 title '10000 operations' with linespoints, \
     "N30000.result" using 1:4 title '30000 operations' with linespoints, \
     "N50000.result" using 1:4 title '50000 operations' with linespoints, \
     "N70000.result" using 1:4 title '70000 operations' with linespoints, \
     "N100000.result" using 1:4 title '100000 operations' with linespoints
set term png
set output "cmpLOT70.png"
replot
unset term