#include"Plotter.cc"
#ifndef __SKFLATPLOTTER_CC__
#define __SKFLATPLOTTER_CC__
class SKFlatPlotter:public Plotter{
public:
  SKFlatPlotter(TString mode_="",TString Analyzer_="");

  TString Analyzer;
  TString AnalyzerSuffix;
  TString SkimTree=".*";

  virtual int Setup(TString mode_,TString Analyzer_);
  virtual TString GetSetupStringForCondor();
  virtual void SetAnalyzer(TString Analyzer_);
  virtual void SetupSamples();
  virtual void SetupSystematics();
};
SKFlatPlotter::SKFlatPlotter(TString mode_,TString Analyzer_){
  Setup(mode_,Analyzer_);
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
  samples["vv"]=Sample("Diboson","SAMPLE sim",kBlue)+TRegexp("/"+Analyzer+SkimTree+"_[W-Z][W-Z]_pythia$");
  samples["wjets"]=Sample("W","SAMPLE sim",kYellow)+TRegexp("/"+Analyzer+SkimTree+"_WJets_MG$");
  samples["tt"]=Sample("t#bar{t}","SAMPLE sim tt",kMagenta)+TRegexp("/"+Analyzer+SkimTree+"_TT[LJ][LJ]_powheg$");
  samples["ttll"]=Sample("t#bar{t}#rightarrowll","SAMPLE sim tt",kMagenta+1)+TRegexp("/"+Analyzer+SkimTree+"_TTLL_powheg$");
  samples["ttlj"]=Sample("t#bar{t}#rightarrowlj","SAMPLE sim tt",kMagenta+2)+TRegexp("/"+Analyzer+SkimTree+"_TTLJ_powheg$");
  samples["ttjj"]=Sample("t#bar{t}#rightarrowjj","SAMPLE sim tt",kMagenta+3)+TRegexp("/"+Analyzer+SkimTree+"_TTJJ_powheg$");
  samples["tw"]=Sample("tW","SAMPLE sim",kMagenta+10)+TRegexp("/"+Analyzer+SkimTree+"_SingleTop_tW_.*top_NoFullyHad$");
  samples["st"]=Sample("Single top","SAMPLE sim",kMagenta+10)+TRegexp("/"+Analyzer+SkimTree+"_SingleTop_.*$");
  samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  samples["ttst"]=Sample("t#bar{t}, single top","SUM",kMagenta)+"tt"+"st";
  samples["aa"]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim",kYellow+1)+TRegexp("/"+Analyzer+SkimTree+"_GGToLL$");

  samples["amc"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJets$");
  samples["mi"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJetsTo.*MiNNLO$");
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJets_MG$");
  samples["amcJet"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_.*DY[0-9]Jets$");
  samples["amcPt"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
  samples["amcM"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_M-[0-9]*to[0-9Inf]*$");
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_MG$");
  TString dysamples[]={"amc","amcJet","amcPt","amcM","mg","mi"};
  for(auto dysample:dysamples){
    samples["tau_"+dysample]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+dysample);
    samples["lhe_"+dysample]="lhe_"%(Sample("#gamma*/Z#rightarrowll (LHE)","SAMPLE sim dy",kBlue)+dysample);
    samples["gen_"+dysample]="gen_"%(Sample("#gamma*/Z#rightarrowll (GEN)","SAMPLE sim dy",kGreen)+dysample);
    samples["genfid_"+dysample]="genfid_"%(Sample("#gamma*/Z#rightarrowll (GEN fiducial)","SAMPLE sim dy",kMagenta)+dysample);
    samples["truth_"+dysample]="truth_"%(Sample("#gamma*/Z#rightarrowll (truth)","SAMPLE sim dy",kCyan)+dysample);
    samples["ss_"+dysample]="ss_"%(Sample("QCD multi-jet","SUM",kCyan)+"data"-dysample-("tau_"+dysample)-"vv"-"wjets"-"tt"-"st"-"aa");
    samples["fake_"+dysample]="fake_"%(Sample("QCD multi-jet","SUM",kOrange)+"data"-dysample-("tau_"+dysample)-"vv"-"wjets"-"tt"-"st"-"aa");
  }
  samples["ss"]=samples["ss_mi"];
  samples["amcPt_stack"]=Sample("DY Pt-binned","STACK",kBlue)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
  for(auto& sub:samples["amcPt_stack"].subs) sub.title=sub.title(TRegexp("Pt-[0-9]*To[0-9Inf]*"));
  samples["amcM_stack"]=Sample("DY M-binned","STACK",kBlue)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_M-[0-9]*to[0-9Inf]*$");
  for(auto& sub:samples["amcM_stack"].subs) sub.title=sub.title(TRegexp("M-[0-9]*to[0-9Inf]*"));  

  samples["qcdss_ee"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eE]&&!0bjet&&!nbjet qcdss_ee",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.043;
  samples["qcdss_ee_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eE]&&0bjet qcdss_ee_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.036;
  samples["qcdss_ee_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eE]&&nbjet qcdss_ee_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.169;
  samples["qcdss_mm"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mM]&&!0bjet&&!nbjet qcdss_mm",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*2.252;
  samples["qcdss_mm_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mM]&&0bjet qcdss_mm_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*2.461;
  samples["qcdss_mm_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mM]&&nbjet qcdss_mm_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*2.064;
  samples["qcdss_em"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&!0bjet&&!nbjet qcdss_em",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_em_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&0bjet qcdss_em_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_em_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&nbjet qcdss_em_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_me"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&!0bjet&&!nbjet qcdss_me",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_me_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&0bjet qcdss_me_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_me_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&nbjet qcdss_me_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  for(auto& [key,sample]:samples){
    if(!key.Contains("qcdss_")) continue;
    for(auto& sub:sample.subs){
      if(!sub.HasTag("data")) sub.AddTag("qcdss_prompt");
    }
  }

  samples["qcdss"]=Sample("QCD multi-jet","SUM",Style(kCyan),Style(kRed,-1,3004,"e2"))+"qcdss_ee"+"qcdss_ee_0bjet"+"qcdss_ee_nbjet"+"qcdss_mm"+"qcdss_mm_0bjet"+"qcdss_mm_nbjet"+"qcdss_em"+"qcdss_em_0bjet"+"qcdss_em_nbjet"+"qcdss_me"+"qcdss_me_0bjet"+"qcdss_me_nbjet";

  samples["fake_mi"]="fake_"%(Sample("QCD multi-jet (TF)","SUM",Style(kRed),Style(kRed,-1,3005,"e2"))+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["fakecpt_mi"]="fakecpt_"%(Sample("QCD multi-jet (TF, cone pT)","SUM",kBlue)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["fakempt_mi"]="fakempt_"%(Sample("QCD multi-jet (TF, modified pT)","SUM",kGreen)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["fakelj_mi"]="fakelj_"%(Sample("QCD multi-jet (TF, lj)","SUM",kMagenta)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");

}

int SKFlatPlotter::Setup(TString mode_,TString Analyzer_){
  Reset();
  SetAnalyzer(Analyzer_);
  if(Analyzer!="") ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/"+Analyzer+AnalyzerSuffix+"/");
  SetupSamples();
  SetupEntries(mode_);
  SetupSystematics();

  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[Setup] nsys: "<<systematics.size()<<endl;

  return 1;
}
TString SKFlatPlotter::GetSetupStringForCondor(){
  return "Setup\\(\\\\\\\""+mode+"\\\\\\\",\\\\\\\""+Analyzer+AnalyzerSuffix+"\\\\\\\"\\)";
}
void SKFlatPlotter::SetupSystematics(){
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[SetupSystematics]"<<endl;
  AddSystematic("PUweight","PUweight",Systematic::Type::ENVELOPE,"_PUweight_up _PUweight_down","sim");
  AddSystematic("prefireweight","prefiring unc.",Systematic::Type::ENVELOPE,"_prefireweight_up _prefireweight_down","sim");
  AddSystematic("CFSF","charge-flip",Systematic::Type::ENVELOPE,"_CFSF_up _CFSF_down","sim");

  AddSystematic("btagh","b tagging (heavy)",Systematic::Type::ENVELOPE,"_btagSF_hup _btagSF_hdown","sim");
  AddSystematic("btagl","b tagging (light)",Systematic::Type::ENVELOPE,"_btagSF_lup _btagSF_ldown","sim");
  AddSystematic("btag","b tagging",Systematic::Type::MULTI,"btagh btagl");

  AddSystematic("JES","JES",Systematic::Type::ENVELOPE,"_jet_scale_up _jet_scale_down","sim");
  AddSystematic("JER","JER",Systematic::Type::ENVELOPE,"_jet_smear_up _jet_smear_down","sim");
  AddSystematic("JEC","JEC",Systematic::Type::MULTI,"JES JER");
  
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

  AddSystematic("sys","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight btag CFSF efficiencySF qcdss JEC");
  AddSystematic("dytheory","theory (DY)",Systematic::Type::MULTI,"dyalphaS dyscale dypdf nozptweight");
  AddSystematic("tttheory","theory (t#bar{t})",Systematic::Type::MULTI,"ttalphaS ttscale ttpdf notopptweight");
  AddSystematic("totalsys","syst. unc.",Systematic::Type::MULTI,"sys dytheory tttheory");
  AddSystematic("totalsys_table","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight btag CFSF efficiencySF dyalphaS dyscale dypdf ttalphaS ttscale ttpdf");
  AddSystematic("test","totalsys",Systematic::Type::MULTI,"btag dyscale ttscale");
}
#endif
