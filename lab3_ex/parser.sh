#!/bin/bash


for i in 502 503 504 505 ;  do cat print.out | grep $i > $i; done

for i in 2772 2744 2800; do cat 502 | grep $i > 502-$i; done

for i in 2687 2715; do cat 503 | grep $i > 503-$i; done

for i in 2912 2884 2856 2940 2828 ; do cat 504 | grep $i > 504-$i;   done

for i in 2968 2996 2772 3024 3052  ; do cat 505 | grep $i > 505-$i;  done


# get the lines number of each UID-PID 

echo "Number of context switches for UID = 502 are"
for i in 2772 2744 2800; do cat wc -l  502-$i; done

echo "Number of context switches for UID = 503 are"
for i in 2687 2715; do wc -l  503-$i; done

echo "Number of context switches for UID = 504 are"
for i in 2912 2884 2856 2940 2828 ; do wc -l  $i 504-$i;   done

echo "Number of context switches for UID = 505 are"
for i in 2968 2996 2772 3024 3052  ; do wc -l 505-$i;  done



