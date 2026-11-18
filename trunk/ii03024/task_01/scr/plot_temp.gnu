set encoding utf8
set terminal windows font "Arial,12"
set grid
set title "Model 11 (Linear)"
set xlabel "Time step (tau)"
set ylabel "Output y(tau)"
set datafile separator ","
plot 'Modell1_Step.csv' skip 1 using 1:3 with lines linewidth 2 title 'y(tau)'
pause -1 "Press Enter to close the plot..."
