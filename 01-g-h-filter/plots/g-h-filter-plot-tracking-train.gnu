set terminal pngcairo dashed enhanced size 960, 1460
#set autoscale
set output "g-h-filter-plot-tracking-train.png"

set macros
# Placement of the labels in the graphs
POS = "at graph 0.5, 1.1 font 'Arial,15"

set key box font "Arial,10" #top left
set grid
set border 3 lw .45
set tics nomirror
unset key

set style data linespoints

set style line 1 \
    linecolor rgb 'black' \
    pointtype 6 pointsize 1.5

set style line 2 \
    lt 6 lw 1.5

set multiplot layout 6,1 rowsfirst \
              margins 0.05,0.98,0.06,0.96 \
              spacing 0.08,0.08

set label 1 'g=0.01, h=0.0001' @POS
plot '../data/g-h-tracking-train.dat' index 'data1' using ($1/1000.) with points linestyle 1 title "Measurements", \
     ''                        index 'data1' using ($2/1000.) with lines linestyle 2 title "Filter"
set label 1 'g=0.02, h=0.0001' @POS
plot '../data/g-h-tracking-train.dat' index 'data2' using ($1/1000.) with points linestyle 1 title "Measurements", \
     ''                        index 'data2' using ($2/1000.) with lines linestyle 2 title "Filter"
set label 1 'g=0.01, h=0.1' @POS
plot '../data/g-h-tracking-train.dat' index 'data3' using ($1/1000.) with points linestyle 1 title "Measurements", \
     ''                        index 'data3' using ($2/1000.) with lines linestyle 2 title "Filter"
set label 1 'g=0.01, h=0.0001 with acc' @POS
plot '../data/g-h-tracking-train.dat' index 'data4' using ($1/1000.) with points linestyle 1 title "Measurements", \
     ''                        index 'data4' using ($2/1000.) with lines linestyle 2 title "Filter"
set label 1 'g=0.01, h=0.001 with acc' @POS
plot '../data/g-h-tracking-train.dat' index 'data5' using ($1/1000.) with points linestyle 1 title "Measurements", \
     ''                        index 'data5' using ($2/1000.) with lines linestyle 2 title "Filter"

n = 5
# t: top margin in pixels
t = 75.0
# b: key height in pixels (bottom margin)
b = 300.0
# h: height of output in pixels
h = 150.0*n + t + b
bot(i,n,h,t,b) = 1.0 - (t+(h-t-b)*i/n)/h
### Last (key) plot
#set tmargin at screen bot(n,n,h,t,b)
set bmargin at screen 0
set key top right
set border 0
unset tics
unset xlabel
unset ylabel
unset label
set yrange [0:1]
plot 2 title "Measurements" linestyle 1, \
     2 title "Filter" linestyle 2

unset multiplot