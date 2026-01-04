#include"SKFlatPlotter.cc"
#ifndef __FAKEPLOTTER_CC__
#define __FAKEPLOTTER_CC__
class FakePlotter:public SKFlatPlotter{
public:
  FakePlotter(TString mode_="data ^dy+tau_dy+vv+wjets+tt+st+qcdtf+aa",TString Analyzer_="FakeAnalyzer");
  virtual void SetupEntries(TString mode_);
  virtual void SetupSamples();
};
FakePlotter::FakePlotter(TString mode_,TString Analyzer_){Setup(mode_,Analyzer_);}
void FakePlotter::SetupEntries(TString mode_){
  if(mode_=="osss"){
    Plotter::SetupEntries("data-dy-tau_dy-wjets-vv-tt-st-aa data-dy-tau_dy-wjets-vv-tt-st-aa");
    entries[0].title="OS (data-sim)";
    entries[1].title="SS (data-sim)";
    entries[1].styles[0].linecolor=2;
    entries[1].styles[0].markercolor=2;
    entries[1].SetHistPrefix("ss_");
  }else if(mode_=="tfss"){
    Plotter::SetupEntries("qcdtf qcdss");
    entries[0].title+=" (TF method)";
    entries[1].title+=" (SS method)";
    entries[1].styles[1].fillstyle=3004;
  }else{
    Plotter::SetupEntries(mode_);
  }
  mode=mode_;
}
void FakePlotter::SetupSamples(){
  SKFlatPlotter::SetupSamples();
  samples["dy"]=samples["mi"]+"mg10";
  samples["tau_dy"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SUM dytau",kGreen)+"mi"+"mg10");
}

#endif
