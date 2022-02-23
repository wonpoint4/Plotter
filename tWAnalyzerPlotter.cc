#include"Plotter.cc"
#ifndef __EFFICIENCYPLOTTER_CC__
#define __EFFICIENCYPLOTTER_CC__
class tWAnalyzerPlotter:public Plotter{
public:
  void SetupSystematics();
  int Setup(TString mode_);
  TString mode;
  tWAnalyzerPlotter(TString mode_="data ^tw+ttlj+ttll+wjets+mi+vv");
};
tWAnalyzerPlotter::tWAnalyzerPlotter(TString mode_){
  ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/tWAnalyzer/");

  samples["muon"]=Sample("data (#mu#mu)","SAMPLE data",kBlack,20)+TRegexp("/DATA/tWAnalyzer_DoubleMuon_[A-Z]")+TRegexp("/DATA/tWAnalyzer_SingleMuon_[A-Z]");
  samples["electron"]=Sample("data (ee)","SAMPLE data",kBlack,20)+TRegexp("/tWAnalyzer_.*EG.*_[A-Z]")+TRegexp("/tWAnalyzer_SingleElectron_[A-Z]");
  samples["data"]=Sample("data","SAMPLE data",kBlack,20)+"muon"+"electron";
  samples["amc"]=Sample("DYJets","SAMPLE sim dy",kCyan)+TRegexp("/tWAnalyzer_DYJets$");
  samples["mi"]=Sample("DYJets","SAMPLE sim dy",kCyan)+TRegexp("/tWAnalyzer_DYJetsTo.*MiNNLO$");
  samples["mg"]=Sample("DYJets","SAMPLE sim dy",kCyan)+TRegexp("/tWAnalyzer_DYJets_MG$");
  samples["vv"]=Sample("Diboson","SAMPLE sim",kMagenta)+TRegexp("/tWAnalyzer_[W-Z][W-Z]_pythia$");
  samples["wjets"]=Sample("WJets","SAMPLE sim",kYellow)+TRegexp("/tWAnalyzer_WJets_MG$");
  //samples["tt"]=Sample("t#bar{t}","SAMPLE sim",kMagenta)+TRegexp("/tWAnalyzer_TTLL_powheg$");
  samples["tw"]=Sample("tW","SAMPLE sim",kGreen+2)+TRegexp("/tWAnalyzer_SingleTop_tW_.*top_NoFullyHad$");
  samples["ttll"]=Sample("TTLL","SAMPLE sim",kBlue)+TRegexp("/tWAnalyzer_TTLL_powheg$");
  samples["ttlj"]=Sample("TTLJ","SAMPLE sim",kRed)+TRegexp("/tWAnalyzer_TTLJ_powheg$");
  //samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  //samples["ss"]="ss_"%(Sample("QCD multi-jet","SUM",kBlue-5)+"data"-"amc"-"tau_amc"-"vv"-"wjets"-"tttw");
  //samples["ss_mi"]="ss_"%(Sample("QCD multi-jet","SUM",kBlue-5)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw");

  //samples["sim_stack"]=Sample("sim","STACK",Style(kRed,-1,3001,"e2"),Style(kCyan,-1,3001,"e2"))+"amc"+"tau_amc"+"vv"+"wjets"+"tttw";
  //samples["sim"]=Sample("simulation","SUM",Style(kRed,22,3001,"e2"),Style(kCyan,-1,3001,"e2"))+"amc"+"tau_amc"+"vv"+"wjets"+"tttw";
  //samples["sim_noSF"]=(Sample("w/o efficiency SF","SUM",kBlue)+"amc"+"tau_amc"+"vv"+"wjets"+"tttw")%"_noefficiencySF";
  //for(auto& sub:samples["sim_noSF"].subs) sub.type=Sample::Type::A;
  //samples["sim_noSF"].styles[0].linewidth=1;

  Setup(mode_);
}

int tWAnalyzerPlotter::Setup(TString mode_){
  Reset();

  mode=mode_;

  SetupEntries(mode_);
  SetupSystematics();

  if(DEBUG) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(DEBUG) std::cout<<"[Setup] nsys: "<<systematics.size()<<endl;
  if(DEBUG) std::cout<<"[Setup] nplot: "<<plots.size()<<endl;

  return 1;
}
void tWAnalyzerPlotter::SetupSystematics(){
  /*
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
  AddSystematic("electronIDSF","electronIDSF_sys",Systematic::Type::MULTI,"electronIDSF_sys1 electronIDSF_sys2 electronIDSF_sys3 electronIDSF_sys4 electronIDSF_sys5 electronIDSF_sys6","sim");
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
  */
}
#endif
