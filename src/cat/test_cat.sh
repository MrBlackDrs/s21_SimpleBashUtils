#!/bin/bash

# cat ../common/test.txt
echo "1: test without flags"
./s21_cat ../common/test.txt > file1
cat ../common/test.txt > file2
out="$(diff -s file1 file2)"
if [ "Files file1 and file2 are identical" == "$out" ]; then
    echo "success"
else
    echo "failed"
fi

i=2
for var in -n -b -s -e -t
do
    echo "$i: test with $var"
    ./s21_cat $var ../common/test.txt > file1
    cat $var ../common/test.txt > file2
    out="$(diff -s file1 file2)"
    if [ "Files file1 and file2 are identical" == "$out" ]; then
        echo "success"
    else
        echo "failed"
    fi
    (( i++ ))
done

echo "\ntesting with non-existing files"
echo "$i: test cat -n ../common/tests.txt"
./s21_cat -n ../common/tests.txt
cat -n ../common/tests.txt
# out="$(diff -s file1 file2)"
# if [ "Files file1 and file2 are identical" == "$out" ]; then
#     echo "success"
# else
#     echo "failed"
# fi
(( i++ ))

echo "\ntesting flags combinations..."
for var in -nb -bs -et -nbs -nbset
do
    echo "$i: test with $var"
    ./s21_cat $var ../common/test.txt > file1
    cat $var ../common/test.txt > file2
    out="$(diff -s file1 file2)"
    if [ "Files file1 and file2 are identical" == "$out" ]; then
        echo "success"
    else
        echo "failed"
    fi
    (( i++ ))
done

echo "\n$i: test with 2 flags -b -e"
./s21_cat -b -e ../common/test.txt > file1
cat -b -e ../common/test.txt > file2
out="$(diff -s file1 file2)"
if [ "Files file1 and file2 are identical" == "$out" ]; then
    echo "success"
else
    echo "failed"
fi
(( i++ ))

echo "\n$i: test with 2 files"
./s21_cat -b -s ../common/test.txt ../common/test.txt > file1
cat -b -s ../common/test.txt ../common/test.txt > file2
out="$(diff -s file1 file2)"
if [ "Files file1 and file2 are identical" == "$out" ]; then
    echo "success"
else
    echo "failed"
fi

rm file1 file2