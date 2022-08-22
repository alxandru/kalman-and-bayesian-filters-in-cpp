set terminal pngcairo dashed enhanced size 1080, 480
#set autoscale
set output "g-h-filter-plot8.png"

set macros
# Placement of the labels in the graphs
POS = "at graph 0.5,-0.1 font 'Arial,15"

set key box font "Arial,10" bottom right
set grid
set border 3 lw .45
set tics nomirror

set style data linespoints

set style line 1 \
    linecolor rgb 'black' \
    pointtype 6 pointsize 1.5

set style line 2 \
    linecolor rgb 'blue' \
    lt -1 pi -1 pt 20 lw 1.5

set style line 3 \
    linecolor rgb 'orange' \
    lt -1 pi -1 pt 11 lw 1.5

plot '../data/g-h-filter8.dat' index 'data' using 1 with points linestyle 1 title "Measurements", \
     ''                        index 'data' using 2 linestyle 2 title "g=0.302, h=0.054", \
     ''                        index 'data' using 3 linestyle 3 title "g=0.546, h=0.205", \