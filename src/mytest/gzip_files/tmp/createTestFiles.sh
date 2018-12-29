#!/bin/sh

fillFile(){
    file=$1"k"
    echo "create file:"$file
    touch $file

    i=1
    while(($i<=$1))
    do
        cat source.txt >> $file
        let "i++"
    done

}

echo "create gzip test files\n"
n=1
while(($n<=63))
do
    fillFile $n
    let "n++"
done
