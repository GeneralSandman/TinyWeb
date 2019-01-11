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

echo "create gzip test files\n"
n=1
while(($n<=1024))
do
    fillFile $n
    let "n++"
done
