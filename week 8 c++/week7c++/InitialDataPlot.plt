set terminal png
set output '3Dplot.png'
set title "3D Data Plot"
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Voltage'
set datafile separator ' '
set grid
set view 60, 30
splot 'C:\Users\CarlStorm\Desktop\internships2024\Tangla Tech internship\week 7 c++ backup\week7c++\3D_data\3D_data.txt' using 1:2:3 with lines title 'My Data'