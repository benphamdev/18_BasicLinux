#!/bin/bash

min(){
    echo $(($1<$2?$1:$2))
}

read -p "Enter a array " -a arr

n=${#arr[@]}

res=${arr[0]}

for ((i=1; i<n; i++)); do
    res=$(min ${arr[i]} $res)
done

echo "The minimum value is $res"

