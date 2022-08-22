set terminal pngcairo dashed enhanced size 1080, 480
#set autoscale
set output "g-h-filter-plot2.png"

set macros
# Placement of the labels in the graphs
POS = "at graph 0.5,-0.1 font 'Arial,15"

set key box font "Arial,10" bottom right
set grid
set border 3 lw .45
set tics nomirror

set style line 1 \
    linecolor rgb 'black' \
    dashtype 3 linewidth 2

set style line 2 \
    linecolor rgb 'black' \
    pointtype 6 pointsize 1.5

set style line 3 \
    linetype 6 linewidth 2


plot '../data/g-h-filter2.dat' index 'data' using 1 with points linestyle 2 title "Measurements", \
     ''                        index 'data' using 2 with lines linestyle 3 title "Filter"