#include"AFBSystPlotter.cc"
#ifndef __GENPLOTTER_CC__
#define __GENPLOTTER_CC__
class GenPlotter:public AFBSystPlotter{
public:
  GenPlotter(TString mode_="eemm",TString Analyzer_="GenTest");
  void SetupEntries(TString mode_);
};
GenPlotter::GenPlotter(TString mode_,TString Analyzer_){Setup(mode_,Analyzer_);}
void GenPlotter::SetupEntries(TString mode_){
  if(mode_=="eemm"){
    Plotter::SetupEntries("mi mi");
    entries[0].title="ee";
    entries[0].replace["LL"]="ee";
    entries[1].title="#mu#mu";
    entries[1].replace["LL"]="mm";
    entries[1].styles[0].linecolor=4;
  }else{
    Plotter::SetupEntries(mode_);
  }
  mode=mode_;
}
#endif
