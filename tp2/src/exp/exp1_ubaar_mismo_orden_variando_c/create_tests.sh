echo "0 0.0 0 tests/exp1/graph.txt 0.00001" > test_1.in
echo "0 0.1 0 tests/exp1/graph.txt 0.00001" > test_2.in
echo "0 0.2 0 tests/exp1/graph.txt 0.00001" > test_3.in
echo "0 0.3 0 tests/exp1/graph.txt 0.00001" > test_4.in
echo "0 0.4 0 tests/exp1/graph.txt 0.00001" > test_5.in
echo "0 0.5 0 tests/exp1/graph.txt 0.00001" > test_6.in
echo "0 0.6 0 tests/exp1/graph.txt 0.00001" > test_7.in
echo "0 0.7 0 tests/exp1/graph.txt 0.00001" > test_8.in
echo "0 0.8 0 tests/exp1/graph.txt 0.00001" > test_9.in
echo "0 0.9 0 tests/exp1/graph.txt 0.00001" > test_10.in


#../bin/tp2 tests/exp1/test_1.in tests/exp1/test_1.out
#../bin/tp2 tests/exp1/test_2.in tests/exp1/test_2.out
#../bin/tp2 tests/exp1/test_3.in tests/exp1/test_3.out
#../bin/tp2 tests/exp1/test_4.in tests/exp1/test_4.out
#../bin/tp2 tests/exp1/test_5.in tests/exp1/test_5.out
#../bin/tp2 tests/exp1/test_6.in tests/exp1/test_6.out
#../bin/tp2 tests/exp1/test_7.in tests/exp1/test_7.out
#../bin/tp2 tests/exp1/test_8.in tests/exp1/test_8.out
#../bin/tp2 tests/exp1/test_9.in tests/exp1/test_9.out
#../bin/tp2 tests/exp1/test_10.in tests/exp1/test_10.out

(paste test_1.out weblist.txt | column -s $'\t' -t) > test_1.tmp && mv test_1.tmp test_1.out
(paste test_2.out weblist.txt | column -s $'\t' -t) > test_2.tmp && mv test_2.tmp test_2.out
(paste test_3.out weblist.txt | column -s $'\t' -t) > test_3.tmp && mv test_3.tmp test_3.out
(paste test_4.out weblist.txt | column -s $'\t' -t) > test_4.tmp && mv test_4.tmp test_4.out
(paste test_5.out weblist.txt | column -s $'\t' -t) > test_5.tmp && mv test_5.tmp test_5.out
(paste test_6.out weblist.txt | column -s $'\t' -t) > test_6.tmp && mv test_6.tmp test_6.out
(paste test_7.out weblist.txt | column -s $'\t' -t) > test_7.tmp && mv test_7.tmp test_7.out
(paste test_8.out weblist.txt | column -s $'\t' -t) > test_8.tmp && mv test_8.tmp test_8.out
(paste test_9.out weblist.txt | column -s $'\t' -t) > test_9.tmp && mv test_9.tmp test_9.out
(paste test_10.out weblist.txt | column -s $'\t' -t) > test_10.tmp && mv test_10.tmp test_10.out


sort -k1 -r test_1.out > tmp && mv tmp test_1.out
sort -k1 -r test_2.out > tmp && mv tmp test_2.out
sort -k1 -r test_3.out > tmp && mv tmp test_3.out
sort -k1 -r test_4.out > tmp && mv tmp test_4.out
sort -k1 -r test_5.out > tmp && mv tmp test_5.out
sort -k1 -r test_6.out > tmp && mv tmp test_6.out
sort -k1 -r test_7.out > tmp && mv tmp test_7.out
sort -k1 -r test_8.out > tmp && mv tmp test_8.out
sort -k1 -r test_9.out > tmp && mv tmp test_9.out
sort -k1 -r test_10.out > tmp && mv tmp test_10.out

