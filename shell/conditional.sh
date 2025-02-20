#!/bin/bash

read -p "Enter a number: " a
read -p "Enter b number: " b
read -p "Enter c number: " c

solve_linear_equation() {
    local b=$1
    local c=$2

    if [ $b -eq 0 ]; then
        if [ $c -eq 0 ]; then
            echo "The equation has infinite solutions"
        else
            echo "The equation has no solution"
        fi
    else
        x=$(echo "scale=2; -1 * $c / $b" | bc)
        echo "The solution is x = $x"
    fi
}

if [ $a -eq 0 ]; then
    solve_linear_equation $b $c
else
    delta=$(echo "scale=2; $b * $b - 4 * $a * $c" | bc)

    if [ $(echo "$delta < 0" | bc) -eq 1 ]; then
        echo "The equation has no solution"
    elif [ $(echo "$delta == 0" | bc) -eq 1 ]; then
        x=$(echo "scale=2; -1 * $b / (2 * $a)" | bc)
        echo "The solution is x = $x"
    else
        sqrt_delta=$(echo "scale=2; sqrt($delta)" | bc)
        x1=$(echo "scale=2; (-1 * $b + $sqrt_delta) / (2 * $a)" | bc)
        x2=$(echo "scale=2; (-1 * $b - $sqrt_delta) / (2 * $a)" | bc)
        echo "The solutions are x1 = $x1 and x2 = $x2"
    fi
fi