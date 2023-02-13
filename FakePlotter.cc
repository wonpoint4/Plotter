#include"SKFlatPlotter.cc"
#ifndef __FAKEPLOTTER_CC__
#define __FAKEPLOTTER_CC__
class FakePlotter:public SKFlatPlotter{
public:
  FakePlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+fake_mi+aa",TString suffix="");
  void SetupEntries(TString mode_);
};
FakePlotter::FakePlotter(TString mode_,TString suffix){
  Setup("FakeAnalyzer"+suffix,mode_);
}
void FakePlotter::SetupEntries(TString mode_){
  TString entrystr=mode_;
  if(mode_=="osss"){
    entrystr="data-mi-tau_mi-wjets-vv-tttw-aa data-mi-tau_mi-wjets-vv-tttw-aa";
  }
  Plotter::SetupEntries(entrystr);
  if(mode_=="osss"){
    entries[0].title="OS (data-sim)";
    entries[1].title="SS (data-sim)";
    entries[1].styles[0].linecolor=2;
    entries[1].styles[0].markercolor=2;
    entries[1].SetHistPrefix("ss_");
  }
  mode=mode_;
}
#endif
