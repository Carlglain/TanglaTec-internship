set terminal png                  # Set the output format to PNG
set datafile separator ','         # Use comma as the data separator
set output 'directNormalplot.png'           # Specify the output file name
set title "direct final data plot" # Set the title of the plot
set xlabel 'x'                    # Label for the x-axis
set ylabel 'voltage'              # Label for the y-axis
plot 'dirMaxOutput.txt' using 1:2 with lines title 'My Data' # Plot data from file