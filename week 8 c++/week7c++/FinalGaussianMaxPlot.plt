set terminal png # Set the output format to PNG
set output '3DplotDirect.png' # Specify the output file name
set title "3D Data Plot" # Set the title of the plot
set xlabel 'X' # Label for the x-axis
set ylabel 'Y' # Label for the y-axis
set zlabel 'Voltage' # Label for the z-axis
set datafile separator ','
# Optional: Set grid for better readability
set grid

# Set view angle for 3D plot
set view 60, 30

# Plot data from the file assuming the format is: x,y,z
splot 'dirMaxOutput.txt' using 1:2:3 with lines title 'My Data'