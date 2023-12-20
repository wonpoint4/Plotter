#include"SKFlatPlotter.cc"
#ifndef __AFBSYSTPLOTTER_CC__
#define __AFBSYSTPLOTTER_CC__
class AFBSystPlotter:public SKFlatPlotter{
public:
  AFBSystPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tt+st+qcdss+aa",TString Analyzer_="AFBAnalyzerSyst");
};
AFBSystPlotter::AFBSystPlotter(TString mode_,TString Analyzer_):SKFlatPlotter(mode_,Analyzer_){}
#endif
