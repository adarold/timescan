void run(int year, int corr){
    char command0[100];
    char command1[100];
    sprintf (command0, "timeshape_analysis t9 (%d)", year);
    sprintf (command1, "t9.Loop(%d,%d)", year, corr);
    gROOT->ProcessLine(".L timeshape_analysis.C+");
    gROOT->ProcessLine(command0);
    gROOT->ProcessLine(command1);
}
