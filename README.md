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

The standard code to retrieve the ntuples does not create a branch for the lumisection. To create ntuples with also this information, use the modified files PulseDump.cc and PulseDump.h presented in this page. They must replace the standard ones in: CMSSW_9_2_7/src/EcalReconstruction/EcalTools/plugins .

The resulting ntuples (~39G) can be found (at least at the moment) at:

    root://eosinfnts.ts.infn.it//eos/infnts/cms/store/user/adarold/ECAL/TimeScan/AlCaPhiSym/crab_TimeScan/180121_141306/0000/

---

2) RUNNING THE ANALYSIS CODE

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
  
2.4) Draw histograms

There are two separate codes to be used. drawhisto.C simply draws in a somewhat nice way the results obtained with fitter (it can be implemented directly into fitter.C but then it takes a lot of time just to change histogram colours).
The script compare.C provides the plots where the comparison with the standard pulse shape is performed. As input, the file standard_ps.txt is needed. This file can be obtained as explained in Section 3. To compare the data to different standard shapes corresponding to different runs, the file many_run.root must be used as input (the file has the same information as the standard_ps.txt one for more sets of runs and saved in a ntuple form).

---

3) OBTAINING THE STANDARD PULSE SHAPES

The code to obtain the standard pulse shapes is the one that can be found at:

https://github.com/ferriff/usercode

Information on metadata of the shapes can be found at:

https://twiki.cern.ch/twiki/bin/viewauth/CMS/EcalDB#PulseShapes

