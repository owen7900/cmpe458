#!/bin/bash
for i in *.pt
do
    echo "=== ptc $i; ${i%.*}.out ==="
    /bin/rm -f ${i%.*}.out
    if [ -f ${i%.*}.dat ]; then
        ./ptc $i
        ./${i%.*}.out ${i%.*}.dat < ${i%.*}.dat
    else
        ./ptc $i 
        ./${i%.*}.out 
    fi
    /bin/rm ${i%.*}.out
done
for i in *.pt
do
    echo "=== pti $i ==="
    if [ -f ${i%.*}.dat ]; then
        ./pti $i ${i%.*}.dat < ${i%.*}.dat
    else
        ./pti $i 
    fi
done
echo "=== ==="
