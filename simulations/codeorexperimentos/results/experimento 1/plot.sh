#!/usr/bin/gnuplot -persist

set encoding utf8;
set terminal pdf;
set output "plot.pdf";


set title  "";
set xlabel "Tempo (segundos)";
set ylabel "Número de pacotes inovativos recebidos";

set key below;

#set logscale x
#set logscale y

set xrange [0:1];
plot "dados.txt" using 1:2 title "" with linespoints lt 3 pt 2;
