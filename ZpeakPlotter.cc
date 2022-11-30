#include"SKFlatPlotter.cc"
#ifndef __ZPEAKPLOTTER_CC__
#define __ZPEAKPLOTTER_CC__
class ZpeakPlotter:public SKFlatPlotter{
public:
  ZpeakPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa");
};
ZpeakPlotter::ZpeakPlotter(TString mode_) : SKFlatAnalyzer("ZpeakAnalyzer",mode_){
}
#endif
