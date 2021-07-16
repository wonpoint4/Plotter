#include"Plotter.cc"
#ifndef __EFFICIENCYPLOTTER_CC__
#define __EFFICIENCYPLOTTER_CC__
class EfficiencyPlotter:public Plotter{
public:
  void SetupSystematics();
  int Setup(TString mode_);
  TString mode;
  EfficiencyPlotter(TString mode_="data ^amc+tau_amc+vv+wjets+tttw");
};
EfficiencyPlotter::EfficiencyPlotter(TString mode_){
  ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/EfficiencyValidation/");

  samples["muon"]=Sample("data (#mu#mu)",Sample::Type::DATA,kBlack,20)+TRegexp("/DATA/EfficiencyValidation_SkimTree_Dilepton_DoubleMuon_[A-Z]")+TRegexp("/DATA/EfficiencyValidation_SkimTree_Dilepton_SingleMuon_[A-Z]");
  samples["electron"]=Sample("data (ee)",Sample::Type::DATA,kBlack,20)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_.*EG.*_[A-Z]")+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_SingleElectron_[A-Z]");
  samples["data"]=Sample("data",Sample::Type::DATA,kBlack,20)+"muon"+"electron";
  samples["amc"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_DYJets$");
  samples["tau_amc"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_DYJets$"));
  samples["amcS20"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_DYJets_Summer20$");
  samples["tau_amcS20"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_DYJets_Summer20$"));
  samples["minnlo"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_DYJetsTo.*MiNNLO$");
  samples["tau_minnlo"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_DYJetsTo.*MiNNLO$"));
  samples["mg"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_DYJets_MG$");
  samples["tau_mg"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_DYJets_MG$"));
  samples["vv"]=Sample("Diboson",Sample::Type::BG,kBlue)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_[W-Z][W-Z]_pythia$");
  samples["wjets"]=Sample("W",Sample::Type::BG,kYellow)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_WJets_MG$");
  samples["tt"]=Sample("t#bar{t}",Sample::Type::BG,kMagenta)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_TTLL_powheg$");
  samples["tw"]=Sample("t#bar{t}",Sample::Type::BG,kMagenta+10)+TRegexp("/EfficiencyValidation_SkimTree_Dilepton_SingleTop_tW_.*top_NoFullyHad$");
  samples["tttw"]=Sample("t#bar{t}, tW",Sample::Type::SUM,kMagenta)+"tt"+"tw";
  samples["ss"]="ss_"%(Sample("QCD multi-jet",Sample::Type::SUM,kCyan)+"data"-"amc"-"tau_amc"-"vv"-"wjets"-"tttw");
  samples["ss_minnlo"]="ss_"%(Sample("QCD multi-jet",Sample::Type::SUM,kCyan)+"data"-"minnlo"-"tau_minnlo"-"vv"-"wjets"-"tttw");

  samples["sim_stack"]=Sample("sim",Sample::Type::STACK,Style(kRed,-1,3001,"e2"),Style(kCyan,-1,3001,"e2"))+"amc"+"tau_amc"+"vv"+"wjets"+"tttw";
  samples["sim"]=Sample("simulation",Sample::Type::SUM,Style(kRed,22,3001,"e2"),Style(kCyan,-1,3001,"e2"))+"amc"+"tau_amc"+"vv"+"wjets"+"tttw";
  samples["sim_noSF"]=(Sample("w/o efficiency SF",Sample::Type::SUM,kBlue)+"amc"+"tau_amc"+"vv"+"wjets"+"tttw")%"_noefficiencySF";
  for(auto& sub:samples["sim_noSF"].subs) sub.type=Sample::Type::A;
  samples["sim_noSF"].style.linewidth=1;

  Setup(mode_);
}

int EfficiencyPlotter::Setup(TString mode_){
  Reset();

  mode=mode_;

  SetupEntries(mode_);
  SetupSystematics();

  if(DEBUG) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(DEBUG) std::cout<<"[Setup] nsys: "<<systematics.size()<<endl;
  if(DEBUG) std::cout<<"[Setup] nplot: "<<plots.size()<<endl;

  return 1;
}
void EfficiencyPlotter::SetupSystematics(){
  if(DEBUG)  cout<<"[SetupSystematics]"<<endl;
  int nreplica=20;
  AddSystematic("RECOSF","RECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_RECOSF_up _RECOSF_down");
  AddSystematic("electronRECOSF_stat","electronRECOSF_stat",Systematic::Type::GAUSSIAN,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),FormRange("_electronRECOSF_s0_m%d",Range(nreplica)));
  AddSystematic("electronRECOSF_sys1","electronRECOSF_sys1",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_electronRECOSF_s1_m0 _electronRECOSF_s1_m1");
  AddSystematic("electronRECOSF_sys2","electronRECOSF_sys2",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_electronRECOSF_s2_m0 _electronRECOSF_s2_m1");
  AddSystematic("electronRECOSF_sys3","electronRECOSF_sys3",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_electronRECOSF_s3_m0 _electronRECOSF_s3_m1");
  AddSystematic("electronRECOSF_sys4","electronRECOSF_sys4",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_electronRECOSF_s4_m0 _electronRECOSF_s4_m1");
  AddSystematic("electronRECOSF_sys","electronRECOSF_sys",Systematic::Type::MULTI,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"electronRECOSF_sys1 electronRECOSF_sys2 electronRECOSF_sys3 electronRECOSF_sys4");
  AddSystematic("electronRECOSF","electronRECOSF",Systematic::Type::MULTI,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"electronRECOSF_stat electronRECOSF_sys");
  AddSystematic("electronIDSF_stat","electronIDSF_stat",Systematic::Type::GAUSSIAN,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),FormRange("_electronIDSF_s0_m%d",Range(nreplica)));
  AddSystematic("electronIDSF_sys1","electronIDSF_sys1",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_electronIDSF_s1_m0");
  AddSystematic("electronIDSF_sys2","electronIDSF_sys2",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_electronIDSF_s2_m0");
  AddSystematic("electronIDSF_sys3","electronIDSF_sys3",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_electronIDSF_s3_m0");
  AddSystematic("electronIDSF_sys4","electronIDSF_sys4",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_electronIDSF_s4_m0");
  AddSystematic("electronIDSF_sys","electronIDSF_sys",Systematic::Type::MULTI,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"electronIDSF_sys1 electronIDSF_sys2 electronIDSF_sys3 electronIDSF_sys4");
  AddSystematic("electronIDSF","electronIDSF",Systematic::Type::MULTI,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"electronIDSF_stat electronIDSF_sys");
  AddSystematic("muonIDSF_stat","muonIDSF_stat",Systematic::Type::GAUSSIAN,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),FormRange("_muonIDSF_s0_m%d",Range(nreplica)));
  AddSystematic("muonIDSF_sys1","muonIDSF_sys1",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_muonIDSF_s1_m0 _muonIDSF_s1_m1");
  AddSystematic("muonIDSF_sys2","muonIDSF_sys2",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_muonIDSF_s2_m0 _muonIDSF_s2_m1");
  AddSystematic("muonIDSF_sys3","muonIDSF_sys3",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_muonIDSF_s3_m0 _muonIDSF_s3_m1");
  AddSystematic("muonIDSF_sys4","muonIDSF_sys4",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_muonIDSF_s4_m0 _muonIDSF_s4_m1");
  AddSystematic("muonIDSF_sys","muonIDSF_sys",Systematic::Type::MULTI,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"muonIDSF_sys1 muonIDSF_sys2 muonIDSF_sys3 muonIDSF_sys4");
  AddSystematic("muonIDSF","muonIDSF",Systematic::Type::MULTI,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"muonIDSF_stat muonIDSF_sys");
  AddSystematic("triggerSF_stat","triggerSF_stat",Systematic::Type::GAUSSIAN,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),FormRange("_triggerSF_s0_m%d",Range(nreplica)));
  AddSystematic("triggerSF_sys1","triggerSF_sys1",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_triggerSF_s1_m0");
  AddSystematic("triggerSF_sys2","triggerSF_sys2",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_triggerSF_s2_m0");
  AddSystematic("triggerSF_sys3","triggerSF_sys3",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_triggerSF_s3_m0");
  AddSystematic("triggerSF_sys4","triggerSF_sys4",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_triggerSF_s4_m0");
  AddSystematic("triggerSF_sys","triggerSF_sys",Systematic::Type::MULTI,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"triggerSF_sys1 triggerSF_sys2 triggerSF_sys3 triggerSF_sys4");
  AddSystematic("triggerSF","triggerSF",Systematic::Type::MULTI,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"triggerSF_stat triggerSF_sys");

  AddSystematic("noRECOSF","noRECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noRECOSF");
  AddSystematic("noIDSF","noIDSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noIDSF");
  AddSystematic("noISOSF","noISOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noISOSF");
  AddSystematic("notriggerSF","notriggerSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_notriggerSF");

  AddSystematic("nozptcor","nozptcor",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL),"_nozptcor");
  AddSystematic("noefficiencySF","noefficiencySF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noefficiencySF");
  AddSystematic("efficiencySF","efficiencySF",Systematic::Type::MULTI,0,"electronRECOSF electronIDSF muonIDSF triggerSF");

}
#endif
