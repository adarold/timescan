void frun(int year, string path, int run_condition){
    char command0[100];
    char command1[100];
    sprintf (command0, "fitter t9 (%d,\"%s\")", year, (path).c_str());
    sprintf (command1, "t9.Loop(%d,%d)", year, run_condition);
    gROOT->ProcessLine(".L fitter.C+");
    gROOT->ProcessLine(command0);
    gROOT->ProcessLine(command1);
}
