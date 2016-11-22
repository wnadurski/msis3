#!/bin/bash
# Skrypt wymaga 3 generatorow liczb losowych obecnych
# w biezacym katalogu: "gen1", "gen2" i "gen3"
#
# Wymaga tez programu "checker", ktory oblicza
# z zadanych liczb srednia, wariancje oraz okres

GENERATORS=(gen1 gen2 gen3)
INPUT_NUMBERS=(1 6 6)
NUMBER_OF_REPS=1000
FROM=100
TO=10000100

echo "Sprawdzacz generatorow testowych"
echo "Output w formacie:"
echo "  Generator #X:"
echo "<x1> <x2>...<xn> : <avg> <variance> <T>"
echo ""

for i in {0..2}
do
  gen=${GENERATORS[$i]}
  numbers=${INPUT_NUMBERS[$i]}

  echo "  Generator #$i"

  for reps in $(seq $NUMBER_OF_REPS)
  do
    for j in $(seq $numbers)
    do
      RANDOM_N[$j]=$(od -vAn -N4 -tu4 < /dev/urandom)
    done

    echo ${RANDOM_N[@]} : $(./$gen $FROM-$TO ${RANDOM_N[@]} | ./checker 10000)
  done
done
