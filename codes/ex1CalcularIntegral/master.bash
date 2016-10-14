#!/bin/bash
./slave -0.4 -0.2 > proc01.out &
./slave -0.2 -0.0 > proc02.out &
./slave  0.0  0.2 > proc03.out &
./slave  0.2 -0.4 > proc04.out &
while [ "$(pidof slave)" ]; do
    sleep 1
done
var1=$(cat proc01.out)
var2=$(cat proc02.out)
var3=$(cat proc03.out)
var4=$(cat proc04.out)
python -c "print $var1+$var2+$var3+$var4"
rm proc*.out
