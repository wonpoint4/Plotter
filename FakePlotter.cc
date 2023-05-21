#include"SKFlatPlotter.cc"
#ifndef __FAKEPLOTTER_CC__
#define __FAKEPLOTTER_CC__
class FakePlotter:public SKFlatPlotter{
public:
  FakePlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tt+st+qcdss+aa",TString Analyzer_="FakeAnalyzer");
  void SetupEntries(TString mode_);
};
FakePlotter::FakePlotter(TString mode_,TString Analyzer_){Setup(mode_,Analyzer_);}
void FakePlotter::SetupEntries(TString mode_){
  if(mode_=="osss"){
    Plotter::SetupEntries("data-mi-tau_mi-wjets-vv-tt-st-aa data-mi-tau_mi-wjets-vv-tt-st-aa");
    entries[0].title="OS (data-sim)";
    entries[1].title="SS (data-sim)";
    entries[1].styles[0].linecolor=2;
    entries[1].styles[0].markercolor=2;
    entries[1].SetHistPrefix("ss_");
  }else if(mode_=="sstf"){
    Plotter::SetupEntries("qcdss qcdtf");
    entries[0].styles[0]=Style(kBlue);
    entries[0].styles[1]=Style(kBlue,-1,3004,"e2");
    entries[0].title+=" (SS method)";
  }else{
    Plotter::SetupEntries(mode_);
  }
  mode=mode_;
}
#endif
