#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

#for j in 2017 20181 20182; do
for j in 20182; do
  YEAR=$j

  #Execute timeshape_analysis the first time
  CORR=0
  echo "Analysing all the collected data for $YEAR whit corr=$CORR, this could take a while..."
  root-6.02 -l -q -b "run.C ($YEAR,$CORR)"

  if [[ -e event_${YEAR}.root ]]; then
    echo "File event_${YEAR}.root has been created."
  else
    echo "ERROR: unable to create the output file."
  fi  

  echo " "
  echo "-------------------------------------------------------------------"
  echo " "

  RUN_CONDITION=0
  echo "Fitting all the crystals to obtain the pulse shapes for $YEAR whit corr=$CORR. As output, some control histograms are produced. This could take a while..."
  root-6.02 -l -q -b "frun.C ($YEAR,\"$DIR\",$RUN_CONDITION)"
  if [[ -e standard_${YEAR}_histograms.root && -e standard_${YEAR}_meanpar.txt && -e standard_${YEAR}_everycrystal_par.txt ]]; then
    echo "File standard_${YEAR}_histograms.root and standard_${YEAR}_meanpar.txt and standard_${YEAR}_everycrystal_par.txt have been created."
  else
    echo "ERROR: unable to create the output file."
  fi  

  echo " "
  echo "-------------------------------------------------------------------"
  echo " "

  echo "Computing the correction factors for the integral mismatch"
  root-6.02 -l -q -b "correction_factors.C (\"$DIR\",$YEAR)"
  if [[ -e ${YEAR}_corrections_barrel.txt && -e ${YEAR}_corrections_endcap.txt ]]; then
    echo "File ${YEAR}_corrections_barrel.txt and ${YEAR}_corrections_endcap.txt have been created."
  else
    echo "ERROR: unable to create the output file."
  fi  

  echo " "
  echo "-------------------------------------------------------------------"
  echo " "

  #Execute timeshape_analysis with corrections applied for integral mismatch
  CORR=1
  echo "Analysing all the collected data for $YEAR whit corr=$CORR, this could take a while..."
  root-6.02 -l -q -b "run.C ($YEAR,$CORR)"
  if [[ -e event_${YEAR}.root ]]; then
    echo "File event_${YEAR}.root has been created."
  else
    echo "ERROR: unable to create the output file."
  fi

  echo " "
  echo "-------------------------------------------------------------------"
  echo " "

  #Runs fitter the standard configuration
  RUN_CONDITION=0
  echo "Fitting all the crystals to obtain the pulse shapes for $YEAR whit corr=$CORR. As output, some control histograms are produced. This could take a while..."
  root-6.02 -l -q -b "frun.C ($YEAR,\"$DIR\",$RUN_CONDITION)"
  if [[ -e standard_${YEAR}_histograms.root && -e standard_${YEAR}_meanpar.txt ]]; then
    echo "File standard_${YEAR}_histograms.root and standard_${YEAR}_meanpar.txt have been created."
  else
    echo "ERROR: unable to create the output file."
  fi  

  echo " "
  echo "-------------------------------------------------------------------"
  echo " "

  #Computes the needed files for the different fit conditions
  echo "Computing the average values of the parameters in etarings"
  root-6.02 -l -q -b "ringpar.C (\"$DIR\",$YEAR)"
  if [[ -e ${YEAR}_etaring_barrel.txt && -e ${YEAR}_etaring_endcapp.txt && -e ${YEAR}_etaring_endcapm.txt ]]; then
    echo "File ${YEAR}_etaring_barrel.txt and ${YEAR}_etaing_endcapp.txt and ${YEAR}_etaring_endcapm.txt have been created."
  else
    echo "ERROR: unable to create the output file."
  fi  

  echo " "
  echo "-------------------------------------------------------------------"
  echo " "

  echo "Computing the average values of the parameters in supermodules"
  root-6.02 -l -q -b "supermodule.C (\"$DIR\",$YEAR)"
  if [[ -e ${YEAR}_supermodule_info.txt ]]; then
    echo "File ${YEAR}_supermodule_info.txt has been created."
  else
    echo "ERROR: unable to create the output file."
  fi  

  echo " "
  echo "-------------------------------------------------------------------"
  echo " "

  echo "Computing the average values of the parameters in triggertowers"
  root-6.02 -l -q -b "trun.C (\"$DIR\",$YEAR)"
  if [[ -e ${YEAR}_triggertower_info.txt ]]; then
    echo "File ${YEAR}_triggertower_info.txt has been created."
  else
    echo "ERROR: unable to create the output file."
  fi

  echo " "
  echo "-------------------------------------------------------------------"
  echo " "

  #Runs fitter different configurations
  echo "Running fitter.C with the different possible configurations"
  for i in 1 2 3 4; do
    RUN_CONDITION=$i
    root-6.02 -l -q -b "frun.C ($YEAR,\"$DIR\",$RUN_CONDITION)" &
    pids[${i}]=$!
  done

  #waits for all the processes to be done
  for pid in ${pids[*]}; do
    wait $pid
  done

  if [[ -e etaring_${YEAR}_meanpar.txt ]]; then
    echo "File etaring_${YEAR}_meanpar.txt has been created."
  else
    echo "ERROR: unable to create the output file for etaring configuration."
  fi
  if [[ -e etaring_fix_${YEAR}_meanpar.txt ]]; then
    echo "File etaring_fix_${YEAR}_meanpar.txt has been created."
  else
    echo "ERROR: unable to create the output file for etaring_fix configuration."
  fi
  if [[ -e supermodule_fix_${YEAR}_meanpar.txt ]]; then
    echo "File supermodule_fix_${YEAR}_meanpar.txt has been created."
  else
    echo "ERROR: unable to create the output file for supermodule_fix configuration."
  fi
  if [[ -e triggertower_fix_${YEAR}_meanpar.txt ]]; then
    echo "File triggertower_fix_${YEAR}_meanpar.txt has been created."
  else
    echo "ERROR: unable to create the output file for triggertower_fix configuration."
  fi

  #Draws histograms
  echo "Drawing plots"
  for i in 0 1 2 3 4; do
    RUN_CONDITION=$i
  root-6.02 -l -q -b "drawhisto.C (\"${DIR}\",${YEAR},${RUN_CONDITION})"
  done

done

rmdir LSFJOB_*
