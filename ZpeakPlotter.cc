#include"Plotter.cc"
#ifndef __ZPEAKPLOTTER_CC__
#define __ZPEAKPLOTTER_CC__
class ZpeakPlotter:public Plotter{
public:
  int Setup(TString mode_);
  TString mode;
  ZpeakPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa");
};
ZpeakPlotter::ZpeakPlotter(TString mode_){
  ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/ZpeakAnalyzer/");

  samples["muon"]=Sample("data (#mu#mu)","SAMPLE data",kBlack,20)+TRegexp("/DATA/ZpeakAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]")+TRegexp("/DATA/ZpeakAnalyzer_SkimTree_Dilepton_SingleMuon_[A-Z]");
  samples["electron"]=Sample("data (ee)","SAMPLE data",kBlack,20)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_.*EG.*_[A-Z]")+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_SingleElectron_[A-Z]");
  samples["data"]=Sample("data","SAMPLE data",kBlack,20)+"muon"+"electron";
  samples["amc"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets$");
  samples["tau_amc"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets$"));
  samples["amcS20"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets_Summer20$");
  samples["tau_amcS20"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets_Summer20$"));
  samples["mi"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJetsTo.*MiNNLO$");
  samples["tau_mi"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJetsTo.*MiNNLO$"));
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets_MG$");
  samples["tau_mg"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets_MG$"));
  samples["vv"]=Sample("Diboson","SAMPLE sim",kBlue)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_[W-Z][W-Z]_pythia$");
  samples["wjets"]=Sample("W","SAMPLE sim",kYellow)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_WJets_MG$");
  samples["tt"]=Sample("t#bar{t}","SAMPLE sim",kMagenta)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_TTLL_powheg$");
  samples["tw"]=Sample("t#bar{t}","SAMPLE sim",kMagenta+10)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_SingleTop_tW_.*top_NoFullyHad$");
  samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  samples["aa"]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim",kYellow+1)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_GGToLL$");
  samples["ss"]="ss_"%(Sample("QCD multi-jet","SUM",kBlue-5)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");
  samples["ss_mi"]="ss_"%(Sample("QCD multi-jet","SUM",kBlue-5)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");


  Setup(mode_);
}

int ZpeakPlotter::Setup(TString mode_){
  Reset();
  mode=mode_;
  SetupEntries(mode_);
  if(DEBUG) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;
  return 1;
}
#endif
