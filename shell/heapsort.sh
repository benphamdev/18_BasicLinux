#!/bin/bash


heapify() {
    local -i i=$1
    local -i n=$2
    local -i largest=$i
    local -i left=$((2*i + 1))
    local -i right=$((2*i + 2))

    if [[ $left -lt $n && ${array[$left]} -gt ${array[$largest]} ]]; then
        largest=$left
    fi

    if [[ $right -lt $n && ${array[$right]} -gt ${array[$largest]} ]]; then
        largest=$right
    fi

    if [[ $largest -ne $i ]]; then
        local -i temp=${array[$i]}
        array[$i]=${array[$largest]}
        array[$largest]=$temp
        heapify $largest $n
    fi
}

heapSort() {
    local -i n=${#array[@]}
    for ((i=n/2-1; i>=0; i--)); do
        heapify $i $n
    done

    for ((i=n-1; i>=0; i--)); do
        local -i temp=${array[0]}
        array[0]=${array[$i]}
        array[$i]=$temp
        heapify 0 $i
    done
}


set -xv

read -ra array < "$1"
heapSort
echo "${array[@]}" > "$2"

set +xv
