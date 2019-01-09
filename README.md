TIME SCAN ANALYSIS OF THE PULSE SHAPES

---

INDEX
1) Building the ntuples
2) Running the analysis code
3) Obtaining the standard pulse shapes

---

1) BUILDING THE NTUPLES

To get the ntuples from the data files, the code from Emanuele Di Marco can be used. Instructions to use it can be found here:

https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideCMSPhysicsObjectSchool2017ECAL

The datasets can be found at:

    /eos/cms/store/data/Run2017F/AlCaPhiSym/RAW/v1/000/305/746/00000/

and subsequent folders, the analysed data are from run 305746 to run 305757. To run the analysis using multiple crab jobs, the code multicrab.py with the samples.py file can be used (code from Andrea Massironi that can be found at https://github.com/amassiro/EcalWeights/tree/master/TimeScan).

The standard code to retrieve the ntuples does not create a branch for the lumisection. To create ntuples with also this information, use the modified files PulseDump.cc and PulseDump.h presented in this page. They must replace the standard ones in: CMSSW_9_2_7/src/EcalReconstruction/EcalTools/plugins . The path in the process source of the file pulsedump_physymm_lonebunch.py must be changed according to the location of the datasets.

---
2) RUNNING THE ANALYSIS CODE

VERSION 1: OPTIMISED

There is a launch_all.sh script that does all the job described in Version 0. The code is run for all the three datasets: October 2017, June 2018 and September 2018. The plots for the single year and the comparison ones are produced and saved.

VERSION 0: OLD BUT USEFUL TO UNDERSTAND WHAT HAPPENS

The launch.sh file allows to turn either on or off the execution of some of the scripts. All the ntuples are analysed through a makeclass method, so that two files (.C and .h) are created. To allow the scripts to be compiled and run, the files #run.C are exploited.

2.1) Creation of the list of crystals

Since not all the crystals have recorded pulse shapes for this analysis, a list of all the cristals with at least one event is created with the script make_rawid.C . This is a dummy procedure, the output file is rawid_list.txt and the code should not be run again.

2.2) Creation of the shapes

The code timeshape_analysis.C (and hence timeshape_analysis.h) is exploited to create a root file where the information on the shapes for all time scans for each crystal is stored. The scripts reads the ntuples created via crab and associates the events to their nominal time delay and their crystal. Moreover, the shapes are normalised to their area. To do so, the correction parameters on the area must be provided (with the correction_barrel.txt and correction_endcap.txt file). To obtain them, the parameters for the analytical function are required. What should be done is then: run the script without the corrections (there is an option in the timeshape_analysis.C script to do so), obtain the mean parameters from fitter.C (see next section), compute the integral corrections using these parameters as input to the analytical function, store the corrections in the correction_barrel.txt and correction_endcap.txt files and then run the timeshape_analysis.C script with the corrections. As output, the timeshape_analysis script produces an ntuple, called event.root, where one pulseshape is associated to each crystal.

2.3) Fit the shapes

The code fitter.C (and hence fitter.h) is the one that fits the shapes.
The script reads all the pulse shapes obtained by the timeshape_analysis script and fits them with the alpha-beta function. The starting parameters for the fit are different for barrel and endcap crystals (it does not change much to use the same set of parameters). As an output, the script produces 3 files:
- meanpar.txt where the mean values for the fit parameters for barrel and endcaps (and inner part and outer part of them) are saved
- everycrystal_par.txt where the points of the fitted shapes are saved for each crystal
- histograms.root where all the histograms are created and stored

At the beginning of the program a variable must be set to decide in which way to fit the crystals:
- standard     = starting fit parameters are set manually at a certain value
- etaring      = starting fit parameters are set exploting external files with etaring parameters
- etaring_fix  = alpha and beta parameters are FIXED according to external files with etaring parameters
- producer     = starting fit parameters are set exploting external files with parameters divided by crystal producer
- producer_fix = alpha and beta parameters are FIXED exploting external files with parameters divided by crystal producer
- triggertower_fix = alpha and beta parameters are FIXED exploting external files with parameters divided by trigger towers

The etaring and producer options needs additional external files to run (see next subsection).

Contour plots for the alpha beta fitting parameters are produced for some crystals (10 random crystals); this option can be changed according to available space and needs.

2.3.1) Additional features

It is possible to fit the shapes using as starting parameters the average values of the fitted parameters in eta rings. To do so, a fitting procedure in the "standard" way must be done (setting manually the initial parameters) and then run the script ringpar.C exploiting the everycrystal_par.txt file that has been created. In this way, three .txt files are created (etaring_barrel.txt, etaring_endcapp.txt and etaring_endcapm.txt) with the averaged parameters in terms of etaring. It is possible to use these parameters as starting points for the fitting procedure or as fixed parameters to see the dependences on the other parameters of the function.

In the same way, it is possible to perfom a fitting procedure that is dependent on the trigger towers. To do so, run the trun.C script where as input the everycrystal_par.txt file must be provided. This will produce a txt file called triggertower_info.txt. Then, running the fitter.C script with the option "triggertower_fix" will provide the distribution of the parameters divided by trigger towers. Running then the trun.C script will produce the maps with the maximum time displacement from the average value of peaking time into a trigger tower and a single crystal peaking time. 
  
2.4) Draw histograms

There are two separate codes to be used. drawhisto.C simply draws in a somewhat nice way the results obtained with fitter (it can be implemented directly into fitter.C but then it takes a lot of time just to change histogram colours).
The script compare.C provides the plots where the comparison with the standard pulse shape is performed. As input, the file standard_ps.txt is needed. This file can be obtained as explained in Section 3. The many_years.C script draws comparison histograms between different runs (October 2017, June 2018 and September 2018).

---

3) OBTAINING THE STANDARD PULSE SHAPES

The code to obtain the standard pulse shapes is the one that can be found at:

https://github.com/ferriff/usercode

Information on metadata of the shapes can be found at:

https://twiki.cern.ch/twiki/bin/viewauth/CMS/EcalDB#PulseShapes

