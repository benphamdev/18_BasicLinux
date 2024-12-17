#!/bin/bash

isPrime() {
    local n=$1
    if [ $n -lt 2 ]; then
        return 1
    fi
    for ((i=2; i*i<=n; i++)); do
        if [ $((n%i)) -eq 0 ]; then
            return 1
        fi
    done
    return 0
}

read -p "Enter a number: " num
if isPrime $num; then
    echo "$num is a prime number."
else
    echo "$num is not a prime number."
fi