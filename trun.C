void trun(string path, int year){
    char command0[100];
    char command1[100];
    sprintf (command0, "tt t9 (\"%s\")", (path).c_str());
    sprintf (command1, "t9.Loop(\"%s\",%d)", (path).c_str(), year);
    gROOT->ProcessLine(".L tt.C+");
    gROOT->ProcessLine(command0);
    gROOT->ProcessLine(command1);
}
