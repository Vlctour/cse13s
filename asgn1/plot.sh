#!/bin/bash

make clean && make collatz   # Rebuild the collatz executable.

# ensures that these temp files dont affect this script
rm -f /tmp/collatz_nums.dat /tmp/collatz_nums2.dat /tmp/collatz.dat /tmp/collatz_his.dat
# for loop that iterates through 2-10000
for (( i=2; i <= 10000; i++))
# run collatz program to generate the specific sequence of a given number and store it in a file.
do ./collatz -n $i> /tmp/collatz_nums.dat;
# put the corresponding iteration number inside the collatz.dat file with no newline.
echo -n "$i ">> /tmp/collatz.dat;
# grab the length of the sequence number that we kept in a seperate file and append it to
# collatz.dat. I learned this also from the asgn1.pdf examples
cat /tmp/collatz_nums.dat | wc -l>> /tmp/collatz.dat;
done

# This is the heredoc that is sent to gnuplot.
# I specified my graph to have dots and take in the data points inside /tmp/collatz.dat
# and use that for the graph. I found out about dots through 
gnuplot <<END
    set terminal pdf
    set output "collatz.pdf"
    set title "Collatz Sequence Lengths"
    set xlabel "n"
    set ylabel "length"
    set zeroaxis
    plot "/tmp/collatz.dat" with dots title ""
END


# removes previous data stored for the last graph so we can use it again for the next graph
rm -f /tmp/collatz.dat
# for loop that iterates through 2-10000
for (( i=2; i <= 10000; i++))
# run collatz program to generate the specific sequence of a given number and store it in a file. 
do ./collatz -n $i> /tmp/collatz_nums.dat;
# sort via numerically and in reverse and store it back into the same file.
sort -n -r -o /tmp/collatz_nums.dat /tmp/collatz_nums.dat;
# take the first value in the file (should be the highest value in the sequence)
head -n 1 /tmp/collatz_nums.dat > /tmp/collatz_nums2.dat;
# put the corresponding iteration number inside the collatz.dat file with no newline.
echo -n "$i ">> /tmp/collatz.dat;
# stick the highest value that we stored inside the previous file and append it to collatz.dat
cat /tmp/collatz_nums2.dat>> /tmp/collatz.dat;
done

# I specified my graph to use dots, have the y range be fixed to 0 - 100000, and use the
# /tmp/collatz.dat file as the data points for the graph.
gnuplot <<END
    set terminal pdf
    set output "collatz_max.pdf"
    set title "Maximum Collatz Sequence Value"
    set xlabel "n"
    set ylabel "value"
    set yrange [0:100000]
    set zeroaxis
    plot "/tmp/collatz.dat" with dots title ""
END


# removes previous data stored for the last graph so we can use it again for the last graph
rm -f /tmp/collatz.dat
# for loop that iterates through 2-10000
for (( i=2; i <= 10000; i++))
# run collatz program to generate the specific sequence of a given number and store all the
# lengths of each sequence into /tmp/collatz_his.dat file.
do ./collatz -n $i | wc -l >> /tmp/collatz_his.dat;
done
# sort the numbers numerically that were inside /tmp/collatz_his.dat file, count the occurances
# of the same number, and swap the position of the numbers. 
sort -n /tmp/collatz_his.dat | uniq -c | awk '{print $2, $1}'> /tmp/collatz.dat;

# I specified my graph to use impulses, fix the x range to be from 0 - 225, and set the xtics.
gnuplot <<END
    set terminal pdf
    set output "collatz_hist.pdf"
    set title "Collatz Sequence Length Histogram"
    set xlabel "length"
    set xrange [0:225]
    set xtics 0,25,225
    set ylabel "frequency"
    set zeroaxis
    plot "/tmp/collatz.dat" with impulses title ""
END
