#include"SKFlatPlotter.cc"
#ifndef __LTPLOTTER_CC__
#define __LTPLOTTER_CC__
class LTPlotter:public SKFlatPlotter{
public:
  LTPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+qcdss+aa",TString Analyzer_="LTAnalyzer");
  virtual int Setup(TString mode_,TString Analyzer_);
};
LTPlotter::LTPlotter(TString mode_,TString Analyzer_) : SKFlatPlotter(mode_,Analyzer_){}
int LTPlotter::Setup(TString mode_,TString Analyzer_){
  Reset();
  SetAnalyzer(Analyzer_);
  if(Analyzer!="") ScanFiles("/data6/Users/hsseo/SKFlatAnalyzer_UL/LTAnalyzer");
  SetupSamples();
  SetupEntries(mode_);
  SetupSystematics();

  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[Setup] nsys: "<<systematics.size()<<endl;

  return 1;
}

#endif
