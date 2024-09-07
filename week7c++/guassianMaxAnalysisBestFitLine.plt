set terminal png
set output 'GaussianBestFitLine.png'
set title "Best Fit Line"
set xlabel 'x'
set ylabel 'voltage'

# Set the datafile separator
set datafile separator ','

# Define the fitting function
f(x) = a * x + b

# Set initial parameters with non-zero values
a = 1.0   
b = 1.0   

# Perform the fit
fit f(x) 'Gaussoutput.txt' using 1:2 via a, b

# Plot the data and the fit
plot 'Gaussoutput.txt' using 1:2 with points title 'Data', f(x) with lines title 'Best Fit'