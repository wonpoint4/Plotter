#include"Plotter.cc"
#ifndef __ZPEAKPLOTTER_CC__
#define __ZPEAKPLOTTER_CC__
class ZpeakPlotter:public Plotter{
public:
  int Setup(TString mode_);
  TString mode;
  ZpeakPlotter(TString mode_="data ^mg+tau_mg+vv+wjets+tttw");
};
ZpeakPlotter::ZpeakPlotter(TString mode_){
  ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/ZpeakAnalyzer/");

  samples["muon"]=Sample("data (#mu#mu)",Sample::Type::DATA,kBlack,20)+TRegexp("/DATA/ZpeakAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]")+TRegexp("/DATA/ZpeakAnalyzer_SkimTree_Dilepton_SingleMuon_[A-Z]");
  samples["electron"]=Sample("data (ee)",Sample::Type::DATA,kBlack,20)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_.*EG.*_[A-Z]")+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_SingleElectron_[A-Z]");
  samples["data"]=Sample("data",Sample::Type::DATA,kBlack,20)+"muon"+"electron";
  samples["amc"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets$");
  samples["tau_amc"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets$"));
  samples["minnlo"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJetsTo.*MiNNLO$");
  samples["tau_minnlo"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJetsTo.*MiNNLO$"));
  samples["mg"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets_MG$");
  samples["tau_mg"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_DYJets_MG$"));
  samples["vv"]=Sample("Diboson",Sample::Type::BG,kBlue)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_[W-Z][W-Z]_pythia$");
  samples["ww"]=Sample("WW",Sample::Type::BG,kBlue)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_WW_pythia$");
  samples["wz"]=Sample("WZ",Sample::Type::BG,kBlue)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_WZ_pythia$");
  samples["zz"]=Sample("ZZ",Sample::Type::BG,kBlue)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_ZZ_pythia$");
  samples["wjets"]=Sample("W",Sample::Type::BG,kYellow)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_WJets_MG$");
  samples["tt"]=Sample("t#bar{t}",Sample::Type::BG,kMagenta)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_TTLL_powheg$");
  samples["tw"]=Sample("t#bar{t}",Sample::Type::BG,kMagenta+10)+TRegexp("/ZpeakAnalyzer_SkimTree_Dilepton_SingleTop_tW_.*top_NoFullyHad$");
  samples["tttw"]=Sample("t#bar{t}, tW",Sample::Type::SUM,kMagenta)+"tt"+"tw";
  samples["ss"]="ss_"%(Sample("QCD multi-jet",Sample::Type::SUM,kCyan)+"data"-"mg"-"tau_mg"-"vv"-"wjets"-"tttw");

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
