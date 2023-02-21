#include"SKFlatPlotter.cc"
#ifndef __EFFICIENCYPLOTTER_CC__
#define __EFFICIENCYPLOTTER_CC__
class EfficiencyPlotter:public SKFlatPlotter{
public:
  EfficiencyPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+qcdss+aa",TString Analyzer_="EfficiencyValidation");
};
EfficiencyPlotter::EfficiencyPlotter(TString mode_,TString Analyzer_):SKFlatPlotter(mode_,Analyzer_){}
#endif
