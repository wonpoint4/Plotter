#include"Plotter.cc"
#ifndef __HIGGSPLOTTER_CC__
#define __HIGGSPLOTTER_CC__
class HiggsPlotter:public Plotter{
public:
  int Setup(TString mode_);
  TString mode;
  HiggsPlotter(TString mode_="data ^ggh+zz+dy");
};
HiggsPlotter::HiggsPlotter(TString mode_){
  ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/HiggsAnalyzer/");

  samples["data"]=Sample("data (#mu#mu)",Sample::Type::DATA,kBlack,20)+TRegexp("/DATA/HiggsAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["ggh"]=Sample("H#rightarrowZZ*",Sample::Type::SIGNAL,kGreen)+TRegexp("/HiggsAnalyzer_ggHToZZTo4L$");
  samples["zz"]=Sample("ZZ",Sample::Type::BG,kBlue)+TRegexp("/HiggsAnalyzer_SkimTree_Dilepton_ZZTo4L_powheg$");
  samples["dy"]=Sample("#gamma*/Z",Sample::Type::BG,kRed)+TRegexp("/HiggsAnalyzer_SkimTree_Dilepton_DYJets$");

  Setup(mode_);
}

int HiggsPlotter::Setup(TString mode_){
  Reset();

  mode=mode_;

  SetupEntries(mode_);

  if(DEBUG) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;

  return 1;
}
#endif
