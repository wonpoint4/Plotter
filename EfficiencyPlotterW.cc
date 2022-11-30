#include"SKFlatPlotter.cc"
#ifndef __EFFICIENCYPLOTTERW_CC__
#define __EFFICIENCYPLOTTERW_CC__
class EfficiencyPlotterW:public SKFlatPlotter{
public:
  EfficiencyPlotterW(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa",TString suffix="");
};
EfficiencyPlotterW::EfficiencyPlotterW(TString mode_,TString suffix) : SKFlatPlotter("EfficiencyValidationW"+suffix,mode_){
}
#endif
