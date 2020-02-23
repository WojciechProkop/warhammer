#!/bin/bash

#Usage: ./dice.sh [[#][d#][(+|-)#]]
#Examples:
#1d20+30
#d13
#
#2+10

#Outputs a random number from 1 to $1, up to 2 bytes
function rnum () {
	num=$((0x$(xxd -l 2 -ps /dev/random)))
	while (($num > $1 - 1)); do
		num=$(($num - $1))
	done
	echo $(($num + 1))
}

#Gets number of rolls
if ! rolls=$(grep -oP '^[1-9][0-9]*' <<<$1); then
	rolls=1
fi

#Gets number of sides
if ! sides=$(grep -oP '(?<=d)[1-9][0-9]*' <<<$1); then
	sides=10
fi

#Gets modifier value (no sign)
if ! modifier=$(grep -oP '(?<=[+-])[1-9][0-9]*$' <<<$1); then
	modifier=0
fi

#If the modifier is negative, flip sign of modifier variable
if grep -oP '(?<=-)[0-9]+$' <<<$1 > /dev/null; then
	modifier=$((-$modifier))
fi

#echo "rolls is $rolls, sides is $sides, modifier is $modifier"

result=0

for roll in $(seq 1 $rolls); do
	result=$(($result + $(rnum $sides)))
done

result=$(($result + $modifier))

echo $result
