set xrange [0:20]
set yrange [0:400]
plot "plot.dat" using 1:2 with lines
pause 1
reread