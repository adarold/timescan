void frun(string path){
    gROOT->ProcessLine(".L fitter.C+");
    gROOT->ProcessLine(("fitter t9 (\""+path+"\")").c_str());
    gROOT->ProcessLine("t9.Loop()");
}
