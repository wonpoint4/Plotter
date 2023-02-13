#include"Plotter.cc"
#ifndef __SKFLATPLOTTER_CC__
#define __SKFLATPLOTTER_CC__
class SKFlatPlotter:public Plotter{
public:
  SKFlatPlotter(TString Analyzer_="",TString mode_="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa");

  TString Analyzer;
  TString AnalyzerSuffix;
  TString SkimTree=".*";

  virtual int Setup(TString Analyzer_,TString mode_);
  virtual TString GetSetupStringForCondor();
  virtual void SetAnalyzer(TString Analyzer_);
  virtual void SetupSystematics();
  virtual void SetupSamples();
};
SKFlatPlotter::SKFlatPlotter(TString Analyzer_,TString mode_){
  Setup(Analyzer_,mode_);
}
void SKFlatPlotter::SetAnalyzer(TString Analyzer_){
  if(Analyzer_=="") return;
  int pos=Analyzer_.Index("_backup");
  if(pos<0) pos=Analyzer_.Length();
  Analyzer=Analyzer_(0,pos);
  AnalyzerSuffix=Analyzer_(pos,Analyzer_.Length());
  return;
}
void SKFlatPlotter::SetupSamples(){
  samples["muon"]=Sample("data (#mu#mu)","SAMPLE data",kBlack,20)+TRegexp("/DATA/"+Analyzer+SkimTree+"_DoubleMuon_[A-Z]")+TRegexp("/DATA/"+Analyzer+SkimTree+"_SingleMuon_[A-Z]");
  samples["electron"]=Sample("data (ee)","SAMPLE data",kBlack,20)+TRegexp("/"+Analyzer+SkimTree+"_.*EG.*_[A-Z]")+TRegexp("/"+Analyzer+SkimTree+"_SingleElectron_[A-Z]");
  samples["electron2018ab"]=Sample("data AB (ee)","SAMPLE data",kBlack,20)+TRegexp("2018/DATA/"+Analyzer+SkimTree+"_.*EG.*_[AB]");
  samples["electron2018cd"]=Sample("data CD (ee)","SAMPLE data",kBlack,20)+TRegexp("2018/DATA/"+Analyzer+SkimTree+"_.*EG.*_[CD]");
  samples["electron2018a"]=4.16*Sample("data A (ee)","SAMPLE data",kBlack,20)+TRegexp("2018/DATA/"+Analyzer+SkimTree+"_.*EG.*_[A]");
  samples["electron2018b"]=9.55*Sample("data B (ee)","SAMPLE data",kRed,20)+TRegexp("2018/DATA/"+Analyzer+SkimTree+"_.*EG.*_[B]");
  samples["electron2018c"]=9.24*Sample("data C (ee)","SAMPLE data",kBlue,20)+TRegexp("2018/DATA/"+Analyzer+SkimTree+"_.*EG.*_[C]");
  samples["electron2018d"]=1.83*Sample("data D (ee)","SAMPLE data",kMagenta,20)+TRegexp("2018/DATA/"+Analyzer+SkimTree+"_.*EG.*_[D]");
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
  samples["tt"]=Sample("t#bar{t}","SAMPLE sim tt",kMagenta)+TRegexp("/"+Analyzer+SkimTree+"_TT[LJ][LJ]_powheg$");
  samples["ttll"]=Sample("t#bar{t}#rightarrowll","SAMPLE sim tt",kMagenta+1)+TRegexp("/"+Analyzer+SkimTree+"_TTLL_powheg$");
  samples["ttlj"]=Sample("t#bar{t}#rightarrowlj","SAMPLE sim tt",kMagenta+2)+TRegexp("/"+Analyzer+SkimTree+"_TTLJ_powheg$");
  samples["ttjj"]=Sample("t#bar{t}#rightarrowjj","SAMPLE sim tt",kMagenta+3)+TRegexp("/"+Analyzer+SkimTree+"_TTJJ_powheg$");
  samples["tw"]=Sample("t#bar{t}","SAMPLE sim",kMagenta+10)+TRegexp("/"+Analyzer+SkimTree+"_SingleTop_tW_.*top_NoFullyHad$");
  samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  samples["aa"]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim",kYellow+1)+TRegexp("/"+Analyzer+SkimTree+"_GGToLL$");
  samples["ss"]="ss_"%(Sample("QCD multi-jet","SUM",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");
  samples["ss_mi"]="ss_"%(Sample("QCD multi-jet","SUM qcdss",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");

  samples["qcdss_ee"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eE]&&!0bjet&&!nbjet qcdss_ee",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.043;
  samples["qcdss_ee_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eE]&&0bjet qcdss_ee_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.036;
  samples["qcdss_ee_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eE]&&nbjet qcdss_ee_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.169;
  samples["qcdss_mm"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mM]&&!0bjet&&!nbjet qcdss_mm",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*2.252;
  samples["qcdss_mm_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mM]&&0bjet qcdss_mm_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*2.461;
  samples["qcdss_mm_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mM]&&nbjet qcdss_mm_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*2.064;
  samples["qcdss_em"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&!0bjet&&!nbjet qcdss_em",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.000;
  samples["qcdss_em_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&0bjet qcdss_em_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.000;
  samples["qcdss_em_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&nbjet qcdss_em_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.000;
  samples["qcdss_me"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&!0bjet&&!nbjet qcdss_me",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.000;
  samples["qcdss_me_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&0bjet qcdss_me_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.000;
  samples["qcdss_me_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&nbjet qcdss_me_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa")*1.000;
  for(auto& [key,sample]:samples){
    if(!key.Contains("qcdss_")) continue;
    for(auto& sub:sample.subs){
      if(!sub.HasTag("data")) sub.AddTag("qcdss_prompt");
    }
  }

  samples["qcdss"]=Sample("QCD multi-jet","SUM",Style(kCyan),Style(kRed,-1,3004,"e2"))+"qcdss_ee"+"qcdss_ee_0bjet"+"qcdss_ee_nbjet"+"qcdss_mm"+"qcdss_mm_0bjet"+"qcdss_mm_nbjet"+"qcdss_em"+"qcdss_em_0bjet"+"qcdss_em_nbjet"+"qcdss_me"+"qcdss_me_0bjet"+"qcdss_me_nbjet";

  samples["fake_mi"]="fake_"%(Sample("QCD multi-jet (TF)","SUM",Style(kRed),Style(kRed,-1,3005,"e2"))+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");
  samples["fakecpt_mi"]="fakecpt_"%(Sample("QCD multi-jet (TF, cone pT)","SUM",kBlue)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");
  samples["fakempt_mi"]="fakempt_"%(Sample("QCD multi-jet (TF, modified pT)","SUM",kGreen)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");
  samples["fakelj_mi"]="fakelj_"%(Sample("QCD multi-jet (TF, lj)","SUM",kMagenta)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");

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
TString SKFlatPlotter::GetSetupStringForCondor(){
  return "Setup\\(\\\\\\\""+Analyzer+AnalyzerSuffix+"\\\\\\\",\\\\\\\""+mode+"\\\\\\\"\\)";
}
void SKFlatPlotter::SetupSystematics(){
  if(DEBUG)  cout<<"[SetupSystematics]"<<endl;
  AddSystematic("PUweight","PUweight",Systematic::Type::ENVELOPE,"_PUweight_up _PUweight_down","sim");
  AddSystematic("prefireweight","prefiring unc.",Systematic::Type::ENVELOPE,"_prefireweight_up _prefireweight_down","sim");
  AddSystematic("CFSF","charge-flip",Systematic::Type::ENVELOPE,"_CFSF_up _CFSF_down","sim");

  AddSystematic("btagh","b tagging (heavy)",Systematic::Type::ENVELOPE,"_btagSF_hup _btagSF_hdown","sim");
  AddSystematic("btagl","b tagging (light)",Systematic::Type::ENVELOPE,"_btagSF_lup _btagSF_ldown","sim");
  AddSystematic("btag","b tagging",Systematic::Type::MULTI,"btagh btagl");
  
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

  AddSystematic("noefficiencySF","noefficiencySF",Systematic::Type::ENVELOPE,"_noefficiencySF","sim");
  AddSystematic("efficiencySF","eff. SF unc.",Systematic::Type::MULTI,"efficiencySF_stat electronRECOSF electronIDSF muonIDSF triggerSF");

  AddSystematic("qcdss_norm_ee","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.028","qcdss_ee");
  AddSystematic("qcdss_norm_ee_0bjet","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.02","qcdss_ee_0bjet");
  AddSystematic("qcdss_norm_ee_nbjet","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.134","qcdss_ee_nbjet");
  AddSystematic("qcdss_norm_mm","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.124","qcdss_mm");
  AddSystematic("qcdss_norm_mm_0bjet","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.146","qcdss_mm_0bjet");
  AddSystematic("qcdss_norm_mm_nbjet","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.09","qcdss_mm_nbjet");
  AddSystematic("qcdss_norm","SS method norm.",Systematic::Type::MULTI,"qcdss_norm_ee qcdss_norm_ee_0bjet qcdss_norm_ee_nbjet qcdss_norm_mm qcdss_norm_mm_0bjet qcdss_norm_mm_nbjet");
  AddSystematic("qcdss_prompt","SS method prompt contam.",Systematic::Type::ENVELOPE,"scale:1.05","qcdss_prompt");
  AddSystematic("qcdss","SS method",Systematic::Type::MULTI,"qcdss_norm qcdss_prompt");
  AddSystematic("qcdtf","TF method",Systematic::Type::ENVELOPE,"_faketf_up _faketf_down");
  AddSystematic("qcd","data-driven QCD est.",Systematic::Type::MULTI,"qcdss qcdtf");

  AddSystematic("dyscale","#mu_{R}/#mu_{F}",Systematic::Type::ENVELOPE,"_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8","dy");
  AddSystematic("dyalphaS","alphaS",Systematic::Type::ENVELOPE,"_alphaS_up _alphaS_down","dy");
  AddSystematic("dypdf","PDF",Systematic::Type::HESSIAN,FormRange("_pdf%d",Range(100)),"dy");
  AddSystematic("nozptweight","nozptweight",Systematic::Type::ENVELOPE,"_nozptweight","dy");

  AddSystematic("ttscale","#mu_{R}/#mu_{F}",Systematic::Type::ENVELOPE,"_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8","tt");
  AddSystematic("ttalphaS","alphaS",Systematic::Type::ENVELOPE,"_alphaS_up _alphaS_down","tt");
  AddSystematic("ttpdf","PDF",Systematic::Type::HESSIAN,FormRange("_pdf%d",Range(100)),"tt");
  AddSystematic("notopptweight","notopptweight",Systematic::Type::ENVELOPE,"_notopptweight","tt");

  AddSystematic("sys","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight btag CFSF efficiencySF");
  AddSystematic("dytheory","theory (DY)",Systematic::Type::MULTI,"dyalphaS dyscale dypdf nozptweight");
  AddSystematic("tttheory","theory (t#bar{t})",Systematic::Type::MULTI,"ttalphaS ttscale ttpdf notopptweight");
  AddSystematic("totalsys","syst. unc.",Systematic::Type::MULTI,"sys dytheory tttheory");
  AddSystematic("test","totalsys",Systematic::Type::MULTI,"btag dyscale ttscale");
}
#endif
