void mrun(){
    gROOT->ProcessLine(".L make_rawid.C+");
    gROOT->ProcessLine("make_rawid t9");
    gROOT->ProcessLine("t9.Loop()");
}
