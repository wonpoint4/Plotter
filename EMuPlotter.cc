#include"SKFlatPlotter.cc"
#ifndef __EMUPLOTTER_CC__
#define __EMUPLOTTER_CC__
class EMuPlotter:public SKFlatPlotter{
public:
  EMuPlotter(TString mode_="data ^tttw+mi+tau_mi+vv+wjets+qcdss+aa",TString suffix="");
};
EMuPlotter::EMuPlotter(TString mode_,TString suffix){
  Setup("EMuAnalyzer"+suffix,mode_);
}
#endif
