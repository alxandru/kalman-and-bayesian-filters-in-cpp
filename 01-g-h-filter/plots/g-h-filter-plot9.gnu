set terminal pngcairo dashed enhanced size 1080, 480
#set autoscale
set output "g-h-filter-plot9.png"

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
    lt 6 lw 1.5

set style line 3 \
    linecolor rgb 'orange' \
    lt -1 pi -1 pt 11 lw 1.5

set style line 4 \
    linecolor rgb 'green' \
    lt -1 pi -1 pt 20 lw 1.5

plot '../data/g-h-filter9.dat' index 'data' using 1 with points linestyle 1 title "Measurements", \
     ''                        index 'data' using 2 with lines linestyle 2 title "dx=0, h=0.05", \
     ''                        index 'data' using 3 linestyle 3 title "dx=2, h=0.05", \
     ''                        index 'data' using 4 linestyle 4 title "dx=2, h=0.5", \