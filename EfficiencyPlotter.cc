#include"SKFlatPlotter.cc"
#ifndef __EFFICIENCYPLOTTER_CC__
#define __EFFICIENCYPLOTTER_CC__
class EfficiencyPlotter:public SKFlatPlotter{
public:
  EfficiencyPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa",TString suffix="");
};
EfficiencyPlotter::EfficiencyPlotter(TString mode_,TString suffix) : SKFlatPlotter("EfficiencyValidation"+suffix,mode_){
}
#endif
