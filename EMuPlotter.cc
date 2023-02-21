#include"SKFlatPlotter.cc"
#ifndef __EMUPLOTTER_CC__
#define __EMUPLOTTER_CC__
class EMuPlotter:public SKFlatPlotter{
public:
  EMuPlotter(TString mode_="data ^tt+st+mi+tau_mi+vv+wjets+qcdss+aa",TString Analyzer_="EMuAnalyzer");
};
EMuPlotter::EMuPlotter(TString mode_,TString Analyzer_):SKFlatPlotter(mode_,Analyzer_){}
#endif
