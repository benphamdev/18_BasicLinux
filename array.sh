#!/bin/bash

# Define an array
# string_array=("apple" "banana" "cherry" "date" "fig")

# files_array=(*.sh)

n=0
read -p "Enter the number of elements: " n
a=()
sum=0

# way 1
# for (( i=0; i<n; i++ )); do
#     read -p "Enter element $i: " element
#     a+=($element)
#     sum=$((sum+element))
# done

# way 2
for i in $(seq 1 $n); do
    read -p "Enter element $i: " element
    a+=($element)
    sum=$((sum+element))
done

# way 3 : {1..$n} won't work

# for i in {1..$n}; do
#     read -p "Enter element $i: " element
#     a+=($element)
#     sum=$((sum+element))
# done

echo "Array: ${a[@]}"
echo "Length: ${#a[@]}"
echo "Sum: $sum"


