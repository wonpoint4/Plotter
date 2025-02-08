#include"SKFlatPlotter.cc"
#ifndef __ZPEAKPLOTTER_CC__
#define __ZPEAKPLOTTER_CC__
class ZpeakPlotter:public SKFlatPlotter{
public:
  ZpeakPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tt+st+qcdss+aa",TString Analyzer_="ZpeakAnalyzer");
};
ZpeakPlotter::ZpeakPlotter(TString mode_,TString Analyzer_) : SKFlatPlotter(mode_,Analyzer_){}
#endif
