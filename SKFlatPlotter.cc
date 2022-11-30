#include"Plotter.cc"
#ifndef __SKFLATPLOTTER_CC__
#define __SKFLATPLOTTER_CC__
class SKFlatPlotter:public Plotter{
public:
  SKFlatPlotter(TString Analyzer_,TString mode_="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa");

  TString Analyzer;
  TString AnalyzerSuffix;
  TString SkimTree=".*";

  virtual int Setup(TString Analyzer_,TString mode_);
  virtual void SetAnalyzer(TString Analyzer_);
  virtual void SetupSystematics();
  virtual void SetupSamples();
};
SKFlatPlotter::SKFlatPlotter(TString Analyzer_,TString mode_){
  Setup(Analyzer_,mode_);
}
void SKFlatPlotter::SetAnalyzer(TString Analyzer_){
  int pos=Analyzer_.Index("_backup");
  if(pos<0) pos=Analyzer_.Length();
  Analyzer=Analyzer_(0,pos);
  AnalyzerSuffix=Analyzer_(pos,Analyzer_.Length());
  return;
}
void SKFlatPlotter::SetupSamples(){
  samples["muon"]=Sample("data (#mu#mu)","SAMPLE data",kBlack,20)+TRegexp("/DATA/"+Analyzer+SkimTree+"_DoubleMuon_[A-Z]")+TRegexp("/DATA/"+Analyzer+SkimTree+"_SingleMuon_[A-Z]");
  samples["electron"]=Sample("data (ee)","SAMPLE data",kBlack,20)+TRegexp("/"+Analyzer+SkimTree+"_.*EG.*_[A-Z]")+TRegexp("/"+Analyzer+SkimTree+"_SingleElectron_[A-Z]");
  samples["data"]=Sample("data","SAMPLE data",kBlack,20)+"muon"+"electron";
  samples["amc"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJets$");
  samples["tau_amc"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/"+Analyzer+SkimTree+"_DYJets$"));
  samples["amcS20"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJets_Summer20$");
  samples["tau_amcS20"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/"+Analyzer+SkimTree+"_DYJets_Summer20$"));
  samples["mi"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJetsTo.*MiNNLO$");
  samples["tau_mi"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/"+Analyzer+SkimTree+"_DYJetsTo.*MiNNLO$"));
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJets_MG$");
  samples["tau_mg"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/"+Analyzer+SkimTree+"_DYJets_MG$"));
  samples["vv"]=Sample("Diboson","SAMPLE sim",kBlue)+TRegexp("/"+Analyzer+SkimTree+"_[W-Z][W-Z]_pythia$");
  samples["wjets"]=Sample("W","SAMPLE sim",kYellow)+TRegexp("/"+Analyzer+SkimTree+"_WJets_MG$");
  samples["tt"]=Sample("t#bar{t}","SAMPLE sim",kMagenta)+TRegexp("/"+Analyzer+SkimTree+"_TTLL_powheg$");
  samples["tw"]=Sample("t#bar{t}","SAMPLE sim",kMagenta+10)+TRegexp("/"+Analyzer+SkimTree+"_SingleTop_tW_.*top_NoFullyHad$");
  samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  samples["aa"]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim",kYellow+1)+TRegexp("/"+Analyzer+SkimTree+"_GGToLL$");
  samples["ss"]="ss_"%(Sample("QCD multi-jet","SUM",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");
  samples["ss_mi"]="ss_"%(Sample("QCD multi-jet","SUM",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");

  samples["sim_stack"]=Sample("sim","STACK",Style(kRed,-1,3001,"e2"),Style(kCyan,-1,3001,"e2"))+"amc"+"tau_amc"+"vv"+"wjets"+"tttw";
  samples["sim"]=Sample("simulation","SUM",Style(kRed,22,3001,"e2"),Style(kCyan,-1,3001,"e2"))+"amc"+"tau_amc"+"vv"+"wjets"+"tttw";
  samples["sim_noSF"]=(Sample("w/o efficiency SF","SUM",kBlue)+"amc"+"tau_amc"+"vv"+"wjets"+"tttw")%"_noefficiencySF";
  //for(auto& sub:samples["sim_noSF"].subs) sub.type=Sample::Type::A;
  samples["sim_noSF"].styles[0].linewidth=1;

}

int SKFlatPlotter::Setup(TString Analyzer_,TString mode_){
  Reset();
  SetAnalyzer(Analyzer_);
  ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/"+Analyzer+AnalyzerSuffix+"/");
  SetupSamples();
  SetupEntries(mode_);
  SetupSystematics();

  if(DEBUG) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(DEBUG) std::cout<<"[Setup] nsys: "<<systematics.size()<<endl;

  return 1;
}
void SKFlatPlotter::SetupSystematics(){
  if(DEBUG)  cout<<"[SetupSystematics]"<<endl;
  int nreplica=20;
  AddSystematic("efficiencySF_stat","efficiencySF_stat",Systematic::Type::GAUSSIAN,FormRange("_efficiencySF_stat%d",Range(20)),"sim");
  AddSystematic("electronRECOSF_sys1","electronRECOSF_sys1",Systematic::Type::ENVELOPE,"_electronRECOSF_s1_m0 _electronRECOSF_s1_m1","sim");
  AddSystematic("electronRECOSF_sys2","electronRECOSF_sys2",Systematic::Type::ENVELOPE,"_electronRECOSF_s2_m0 _electronRECOSF_s2_m1","sim");
  AddSystematic("electronRECOSF_sys3","electronRECOSF_sys3",Systematic::Type::ENVELOPE,"_electronRECOSF_s3_m0 _electronRECOSF_s3_m1","sim");
  AddSystematic("electronRECOSF_sys4","electronRECOSF_sys4",Systematic::Type::ENVELOPE,"_electronRECOSF_s4_m0 _electronRECOSF_s4_m1","sim");
  AddSystematic("electronRECOSF","electronRECOSF_sys",Systematic::Type::MULTI,"electronRECOSF_sys1 electronRECOSF_sys2 electronRECOSF_sys3 electronRECOSF_sys4","sim");
  AddSystematic("electronIDSF_sys1","electronIDSF_sys1",Systematic::Type::ENVELOPE,"_electronIDSF_s1_m0","sim");
  AddSystematic("electronIDSF_sys2","electronIDSF_sys2",Systematic::Type::ENVELOPE,"_electronIDSF_s2_m0","sim");
  AddSystematic("electronIDSF_sys3","electronIDSF_sys3",Systematic::Type::ENVELOPE,"_electronIDSF_s3_m0","sim");
  AddSystematic("electronIDSF_sys4","electronIDSF_sys4",Systematic::Type::ENVELOPE,"_electronIDSF_s4_m0","sim");
  AddSystematic("electronIDSF_sys5","electronIDSF_sys5",Systematic::Type::ENVELOPE,"_electronIDSF_s5_m0","sim");
  AddSystematic("electronIDSF_sys6","electronIDSF_sys6",Systematic::Type::ENVELOPE,"_electronIDSF_s6_m0","sim");
  AddSystematic("electronIDSF_sys7","electronIDSF_sys7",Systematic::Type::ENVELOPE,"_electronIDSF_s7_m0","sim");
  AddSystematic("electronIDSF","electronIDSF_sys",Systematic::Type::MULTI,"electronIDSF_sys1 electronIDSF_sys2 electronIDSF_sys3 electronIDSF_sys4 electronIDSF_sys5 electronIDSF_sys6 electronIDSF_sys7","sim");
  AddSystematic("muonIDSF_sys1","muonIDSF_sys1",Systematic::Type::ENVELOPE,"_muonIDSF_s1_m0 _muonIDSF_s1_m1","sim");
  AddSystematic("muonIDSF_sys2","muonIDSF_sys2",Systematic::Type::ENVELOPE,"_muonIDSF_s2_m0 _muonIDSF_s2_m1","sim");
  AddSystematic("muonIDSF_sys3","muonIDSF_sys3",Systematic::Type::ENVELOPE,"_muonIDSF_s3_m0","sim");
  AddSystematic("muonIDSF_sys4","muonIDSF_sys4",Systematic::Type::ENVELOPE,"_muonIDSF_s4_m0 _muonIDSF_s4_m1","sim");
  AddSystematic("muonIDSF","muonIDSF_sys",Systematic::Type::MULTI,"muonIDSF_sys1 muonIDSF_sys2 muonIDSF_sys3 muonIDSF_sys4","sim");
  AddSystematic("triggerSF_sys1","triggerSF_sys1",Systematic::Type::ENVELOPE,"_triggerSF_s1_m0 _triggerSF_s1_m1","sim");
  AddSystematic("triggerSF_sys2","triggerSF_sys2",Systematic::Type::ENVELOPE,"_triggerSF_s2_m0 _triggerSF_s2_m1","sim");
  AddSystematic("triggerSF_sys3","triggerSF_sys3",Systematic::Type::ENVELOPE,"_triggerSF_s3_m0","sim");
  AddSystematic("triggerSF_sys4","triggerSF_sys4",Systematic::Type::ENVELOPE,"_triggerSF_s4_m0","sim");
  AddSystematic("triggerSF_sys5","triggerSF_sys5",Systematic::Type::ENVELOPE,"_triggerSF_s5_m0","sim");
  AddSystematic("triggerSF_sys6","triggerSF_sys6",Systematic::Type::ENVELOPE,"_triggerSF_s6_m0","sim");
  AddSystematic("triggerSF","triggerSF_sys",Systematic::Type::MULTI,"triggerSF_sys1 triggerSF_sys2 triggerSF_sys3 triggerSF_sys4 triggerSF_sys5 triggerSF_sys6","sim");

  AddSystematic("noRECOSF","noRECOSF",Systematic::Type::ENVELOPE,"_noRECOSF","sim");
  AddSystematic("noIDSF","noIDSF",Systematic::Type::ENVELOPE,"_noIDSF","sim");
  AddSystematic("noISOSF","noISOSF",Systematic::Type::ENVELOPE,"_noISOSF","sim");
  AddSystematic("notriggerSF","notriggerSF",Systematic::Type::ENVELOPE,"_notriggerSF","sim");

  AddSystematic("nozptcor","nozptcor",Systematic::Type::ENVELOPE,"_nozptcor","dy");
  AddSystematic("noefficiencySF","noefficiencySF",Systematic::Type::ENVELOPE,"_noefficiencySF","sim");
  AddSystematic("efficiencySF","eff. SF unc.",Systematic::Type::MULTI,"efficiencySF_stat electronRECOSF electronIDSF muonIDSF triggerSF");

}
#endif
