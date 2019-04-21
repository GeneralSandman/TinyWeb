#!/bin/sh

fillFile(){
    file=$1"k.txt"
    echo "create file:"$file
    touch $file

    cat source.txt >> $file
    num1=$1
    num2=1
    if test $[num1] -gt $[num2]
    then
        val=`expr $num1 - $num2`
        old=$val"k.txt"
        cat $old >> $file
    fi

}

createUrlFile(){
    file="http://127.0.0.1:9090/1-63k_files/"$1"k.txt"
    echo "create url:"$file
    echo $file >> urlsk.txt

    file="http://127.0.0.1:9091/1-63k_files/"$1"k.txt"
    echo "create url:"$file
    echo $file >> urlsk.txt

    file="http://127.0.0.1:9092/1-63k_files/"$1"k.txt"
    echo "create url:"$file
    echo $file >> urlsk.txt

}

echo "create gzip test files\n"
echo "touch urls file\n"
touch urlsk.txt
n=1
while(($n<=1024*2))
do
    fillFile $n
    createUrlFile $n
    let "n = n + 1"
done
