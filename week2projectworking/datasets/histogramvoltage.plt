set terminal png
set output 'hitogramvoltage.png'
set title "histogram"
set xlabel 'x'
set ylabel 'voltage'
set style data histogram
set style histogram clustered
set key off

plot for [COL =2:10] "data.csv" using COL:xtic(1)