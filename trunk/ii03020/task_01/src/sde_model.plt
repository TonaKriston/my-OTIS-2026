set terminal pngcairo size 1000,700 enhanced font 'Verdana,10'
set output 'sde_model.png'
set encoding utf8
set datafile separator ','
set datafile missing '-'
set grid
set multiplot layout 2,1 title 'Differential equation'
set xlabel 'step'
set ylabel 'y'
plot 'sde_model.csv' every ::1 using 1:2 with linespoints lw 2 pt 7 lc rgb '#1f77b4' title 'y(step)'
set ylabel 'u'
plot 'sde_model.csv' every ::1 using 1:3 with linespoints lw 2 pt 5 lc rgb '#d62728' title 'u(step)'
unset multiplot
