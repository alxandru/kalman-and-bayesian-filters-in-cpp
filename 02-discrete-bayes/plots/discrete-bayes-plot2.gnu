set terminal pngcairo dashed enhanced size 1080, 480
#set autoscale
set output "discrete-bayes-plot2.png"

set macros
# Placement of the labels in the graphs
POS = "at graph 0.5,1.0 font 'Arial,15"

unset key
set grid
set border 3 lw .45
set tics nomirror


set auto x
#set yrange [0:1]
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set xtic rotate by -45 scale 0
#set bmargin 10 
#plot '../data/discrete-bayes1.dat' using 1:xtic(1) ti col

set multiplot layout 1,2 rowsfirst \
              margins 0.05,0.98,0.08,0.90 \
              spacing 0.08,0.08 \
              title "Move 2 position" font "Arial,20"
set label 1 'Belief' @POS
plot '../data/discrete-bayes2.dat' index 'belief' using 2:xtic(1) lc 'black'
set label 1 'Prior' @POS
plot '../data/discrete-bayes2.dat' index 'move' using 2:xtic(1) lc 'black'
unset multiplot