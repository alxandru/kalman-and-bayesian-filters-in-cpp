set terminal pngcairo dashed enhanced size 1080, 480
#set autoscale
set output "discrete-bayes-plot1.png"

set macros
# Placement of the labels in the graphs
POS = "at graph 0.5,-0.1 font 'Arial,15"

unset key
set grid
set border 3 lw .45
set tics nomirror


set auto x
set yrange [0:0.3]
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set xtic rotate by -45 scale 0
#set bmargin 10 
plot '../data/discrete-bayes1.dat' using 2:xtic(1) lc 'black'