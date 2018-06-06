#!/bin/bash

#Settings on which scripts to run
RAWIDLIST="off"
TIMESHAPEANALYSIS="on"
FITTER="on"
DRAWHISTO="on"
COMPARE="on"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

if [[ $RAWIDLIST = on ]]; then

   echo "Creating list of crystals rawid..."
   root-6.02 -l -q -b "mrun.C"

   if [[ -e rawid_list.txt ]]; then
      echo "File rawid_list.txt has been created."
   else
      echo "ERROR: unable to create the output file."
   fi  

fi


if [[ $TIMESHAPEANALYSIS = on ]]; then

   echo "Analysing all the collected data, this could take a while..."
   root-6.02 -l -q -b "run.C"

   if [[ -e event.root ]]; then
      echo "File event.root has been created."
   else
      echo "ERROR: unable to create the output file."
   fi  

fi


if [[ $FITTER = on ]]; then

   echo "Fitting all the crystals to obtain the pulse shapes. As output, some control histograms are produced. This could take a while..."
   root-6.02 -l -q -b "frun.C (\"$DIR\")"

   if [[ -e histograms.root && -e meanpar.txt ]]; then
      echo "File histograms.root and meanpar.txt have been created."
   else
      echo "ERROR: unable to create the output file."
   fi  

fi


if [[ $DRAWHISTO = on ]]; then

   echo "Creating and saving the histograms..."
   root-6.02 -l -q -b "drawhisto.C"

fi


if [[ $COMPARE = on ]]; then

   echo "Creating and saving the comparison histograms..."
   root-6.02 -l -q -b "compare.C"

fi


