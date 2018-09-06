void trun(){
    gROOT->ProcessLine(".L tt.C+");
    gROOT->ProcessLine("tt t9");
    gROOT->ProcessLine("t9.Loop()");
}
