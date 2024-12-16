#!/bin/bash

#expr

#let

# $(( ))

a=0
b=0

echo "Enter a: "
read a
echo "Enter b: "
read b


echo "a + b = $((a + b))"

let substraction=$a-$b
echo "a - b = $substraction"
echo "a * b = `expr $a \* $b`"
echo "a / b = $((a / b))"

read -p "Enter r: " r
PI=3.14159
area=`echo "scale=2; $PI * ($r * $r)" | bc`
echo "Area of circle is: $area"
parimeter=`echo "scale=2; 2 * $PI * $r" | bc`
echo "Parimeter of circle is: $parimeter"

# caculate the area of triangle
read -p "Enter a: " a
read -p "Enter b: " b
read -p "Enter c: " c
s=`echo "scale=2; ($a + $b + $c) / 2" | bc`
area=`echo "scale=2; sqrt($s * ($s - $a) * ($s - $b) * ($s - $c))" | bc`
echo "Area of triangle is: $area"
parimeter=`echo "scale=2; $a + $b + $c" | bc`
echo "Parimeter of triangle is: $parimeter"