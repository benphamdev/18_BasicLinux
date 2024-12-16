#iterations -> for

#while , for, until, select

for i in {1..100}; do
	echo $i
done

#convert decimal to binary

read -p "Enter a number: " num

res=""
while [ $num -gt 0 ]; do
	rem=$((num%2))
	res=$rem$res
	num=$((num/2))
done

echo "Binary: $res"
