#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

for j in 2017 20181 20182; do
  YEAR=$j

  #Draws histograms
  echo "Drawing plots"
  for i in 0 1 2 3 4; do
    RUN_CONDITION=$i
  root-6.02 -l -q -b "drawhisto.C (\"${DIR}\",${YEAR},${RUN_CONDITION})"
  done

done

#Draws year comparison histograms
for i in 0 1 2 3 4; do
  RUN_CONDITION=$i
  root-6.02 -l -q -b "many_years.C (\"${DIR}\",${RUN_CONDITION})"
done

rmdir LSFJOB_*
