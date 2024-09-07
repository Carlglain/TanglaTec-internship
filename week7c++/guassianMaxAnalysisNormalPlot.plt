set terminal png                  # Set the output format to PNG
set datafile separator ','         # Use comma as the data separator
set output 'Guassian.png'           # Specify the output file name
set title "gaussian final data plot" # Set the title of the plot
set xlabel 'x'                    # Label for the x-axis
set ylabel 'voltage'              # Label for the y-axis
plot 'Gaussoutput.txt' using 1:2 with lines title 'My Data' # Plot data from file