void run(){
    gROOT->ProcessLine(".L timeshape_analysis.C+");
    gROOT->ProcessLine("timeshape_analysis t9");
    gROOT->ProcessLine("t9.Loop()");
}
