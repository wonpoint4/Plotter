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
  samples["ttll"]=Sample("t#bar{t}#rightarrowll","SAMPLE sim tt ttll",kMagenta+1)+TRegexp("/"+Analyzer+SkimTree+"_TTLL_powheg$");
  samples["ttlj"]=Sample("t#bar{t}#rightarrowlj","SAMPLE sim tt",kMagenta+2)+TRegexp("/"+Analyzer+SkimTree+"_TTLJ_powheg$");
  samples["ttjj"]=Sample("t#bar{t}#rightarrowjj","SAMPLE sim tt",kMagenta+3)+TRegexp("/"+Analyzer+SkimTree+"_TTJJ_powheg$");
  samples["tt"]=Sample("t#bar{t}","SUM",kMagenta)+"ttll"+"ttlj"+"ttjj";
  samples["tw"]=Sample("tW","SAMPLE sim",kMagenta+10)+TRegexp("/"+Analyzer+SkimTree+"_SingleTop_tW_.*top_NoFullyHad$");
  samples["st"]=Sample("Single top","SAMPLE sim",kMagenta+10)+TRegexp("/"+Analyzer+SkimTree+"_SingleTop_.*$");
  samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  samples["ttst"]=Sample("t#bar{t}, single top","SUM",kMagenta)+"tt"+"st";
  samples["aa"]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim",kYellow+1)+TRegexp("/"+Analyzer+SkimTree+"_GGToLL$");

  samples["amc"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJets$");
  samples["mi"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJetsTo.*MiNNLO$");
  for(auto& sub:samples["mi"].subs){
    if(sub.title.Contains("DYJetsToEE")&&sub.title.Contains("2017"))
      sub.weight=0.99;
  }
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_DYJets_MG$");
  samples["amcJet"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_.*DY[0-9]Jets$");
  samples["amcPt"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
  samples["amcM"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_M-[0-9]*to[0-9Inf]*$");
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_MG$");
  TString dysamples[]={"amc","amcJet","amcPt","amcM","mg","mi"};
  for(auto dysample:dysamples){
    samples["bx_"+dysample]="bx_"%(Sample("#gamma*/Z#rightarrowll (b init)","SAMPLE sim dy",kRed-5)+dysample); //temp
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

  samples["qcdss_ee"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eEl]&&!0bjet&&!nbjet qcdss_ee",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.043;
  samples["qcdss_ee_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eEl]&&0bjet qcdss_ee_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.044;
  samples["qcdss_ee_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eEl]&&nbjet qcdss_ee_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.007;
  samples["qcdss_mm"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mMu]&&!0bjet&&!nbjet qcdss_mm",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*2.252;
  samples["qcdss_mm_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mMu]&&0bjet qcdss_mm_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*2.284;
  samples["qcdss_mm_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mMu]&&nbjet qcdss_mm_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.192;
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

  samples["qcdss"]=Sample("QCD multi-jet","SUM",Style(kCyan),Style(kCyan,-1,3004,"e2"))+"qcdss_ee"+"qcdss_ee_0bjet"+"qcdss_ee_nbjet"+"qcdss_mm"+"qcdss_mm_0bjet"+"qcdss_mm_nbjet"+"qcdss_em"+"qcdss_em_0bjet"+"qcdss_em_nbjet"+"qcdss_me"+"qcdss_me_0bjet"+"qcdss_me_nbjet";

  samples["fake_mi"]="fake_"%(Sample("QCD multi-jet (TF method)","SUM",Style(kRed),Style(kRed,-1,3005,"e2"))+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["qcdtf"]=samples["fake_mi"];
  samples["fakecpt_mi"]="fakecpt_"%(Sample("QCD multi-jet (TF, cone pT)","SUM",kBlue)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["fakempt_mi"]="fakempt_"%(Sample("QCD multi-jet (TF, modified pT)","SUM",kGreen)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["fakelj_mi"]="fakelj_"%(Sample("QCD multi-jet (TF, lj)","SUM",kMagenta)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");

  samples["zp"]=Sample("Z'","SAMPLE sim",kRed)+TRegexp("/"+Analyzer+SkimTree+"_ZPrime$");
  samples["zpsb"]=Sample("Z'","SAMPLE sim",kGreen+2)+TRegexp("/"+Analyzer+SkimTree+"_ZPrimeToMuMuSB$");
  samples["bbllpll"]=Sample("bbll (#eta_{LL})","SAMPLE sim",kBlue)+TRegexp("/"+Analyzer+SkimTree+"_bbll_M-[0-9]*To[0-9Inf]*_Lambda-10TeV_posEtaLL$");
  samples["bbllplr"]=Sample("bbll (#eta_{LR})","SAMPLE sim",kRed)+TRegexp("/"+Analyzer+SkimTree+"_bbll_M-[0-9]*To[0-9Inf]*_Lambda-10TeV_posEtaLR$");
  samples["bbllprl"]=Sample("bbll (#eta_{RL})","SAMPLE sim",kGreen+1)+TRegexp("/"+Analyzer+SkimTree+"_bbll_M-[0-9]*To[0-9Inf]*_Lambda-10TeV_posEtaRL$");
  samples["bbllprr"]=Sample("bbll (#eta_{RR})","SAMPLE sim",kCyan+1)+TRegexp("/"+Analyzer+SkimTree+"_bbll_M-[0-9]*To[0-9Inf]*_Lambda-10TeV_posEtaRR$");
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
  AddSystematic("noz0weight","noz0weight",Systematic::Type::ENVELOPE,"_noz0weight","sim");
  AddSystematic("z0weight","z0weight",Systematic::Type::ENVELOPE,"_z0weight","sim");
  AddSystematic("CFSF","charge-flip",Systematic::Type::ENVELOPE,"_CFSF_up _CFSF_down","sim");

  AddSystematic("btagh","b tagging (heavy)",Systematic::Type::ENVELOPE,"_btagSF_hup _btagSF_hdown","sim");
  AddSystematic("btagl","b tagging (light)",Systematic::Type::ENVELOPE,"_btagSF_lup _btagSF_ldown","sim");
  AddSystematic("btag","b tagging",Systematic::Type::MULTI,"btagh btagl");

  AddSystematic("JES","JES",Systematic::Type::ENVELOPE,"_jet_scale_up _jet_scale_down","sim");
  AddSystematic("JER","JER",Systematic::Type::ENVELOPE,"_jet_smear_up _jet_smear_down","sim");
  AddSystematic("JEC","JEC",Systematic::Type::MULTI,"JES JER");
  
  int nreplica=20;
  AddSystematic("efficiencySF_stat","efficiencySF_stat",Systematic::Type::GAUSSIAN,FormRange("_efficiencySF_stat%d",Range(20)),"sim");
  AddSystematic("electronRECOSF_sys1","electronRECOSF_sys1",Systematic::Type::ENVELOPE,"_electronRECOSF_s1_m0","sim");
  AddSystematic("electronRECOSF_sys2","electronRECOSF_sys2",Systematic::Type::ENVELOPE,"_electronRECOSF_s2_m0","sim");
  AddSystematic("electronRECOSF_sys3","electronRECOSF_sys3",Systematic::Type::ENVELOPE,"_electronRECOSF_s3_m0","sim");
  AddSystematic("electronRECOSF_sys4","electronRECOSF_sys4",Systematic::Type::ENVELOPE,"_electronRECOSF_s4_m0","sim");
  AddSystematic("electronRECOSF_sys5","electronRECOSF_sys5",Systematic::Type::ENVELOPE,"_electronRECOSF_s5_m0","sim");
  AddSystematic("electronRECOSF_sys6","electronRECOSF_sys6",Systematic::Type::ENVELOPE,"_electronRECOSF_s6_m0","sim");
  AddSystematic("electronRECOSF_sys7","electronRECOSF_sys7",Systematic::Type::ENVELOPE,"_electronRECOSF_s7_m0 _electronRECOSF_s7_m1","sim");
  AddSystematic("electronRECOSF_sys8","electronRECOSF_sys8",Systematic::Type::ENVELOPE,"_electronRECOSF_s8_m0 _electronRECOSF_s8_m1","sim"); 
  AddSystematic("electronRECOSF_sys9","electronRECOSF_sys9",Systematic::Type::ENVELOPE,"_electronRECOSF_s9_m0","sim"); 
  AddSystematic("electronRECOSF_sys10","electronRECOSF_sys10",Systematic::Type::ENVELOPE,"_electronRECOSF_s10_m0","sim");
  AddSystematic("electronRECOSF","electronRECOSF_sys",Systematic::Type::MULTI,FormRange("electronRECOSF_sys%d",Range(1,11)),"sim");
  AddSystematic("electronIDSF_sys1","electronIDSF_sys1",Systematic::Type::ENVELOPE,"_electronIDSF_s1_m0","sim");
  AddSystematic("electronIDSF_sys2","electronIDSF_sys2",Systematic::Type::ENVELOPE,"_electronIDSF_s2_m0","sim");
  AddSystematic("electronIDSF_sys3","electronIDSF_sys3",Systematic::Type::ENVELOPE,"_electronIDSF_s3_m0","sim");
  AddSystematic("electronIDSF_sys4","electronIDSF_sys4",Systematic::Type::ENVELOPE,"_electronIDSF_s4_m0","sim");
  AddSystematic("electronIDSF_sys5","electronIDSF_sys5",Systematic::Type::ENVELOPE,"_electronIDSF_s5_m0","sim");
  AddSystematic("electronIDSF_sys6","electronIDSF_sys6",Systematic::Type::ENVELOPE,"_electronIDSF_s6_m0","sim");
  AddSystematic("electronIDSF_sys7","electronIDSF_sys7",Systematic::Type::ENVELOPE,"_electronIDSF_s7_m0 _electronIDSF_s7_m1","sim");
  AddSystematic("electronIDSF_sys8","electronIDSF_sys8",Systematic::Type::ENVELOPE,"_electronIDSF_s8_m0 _electronIDSF_s8_m1","sim"); 
  AddSystematic("electronIDSF_sys9","electronIDSF_sys9",Systematic::Type::ENVELOPE,"_electronIDSF_s9_m0","sim"); 
  AddSystematic("electronIDSF_sys10","electronIDSF_sys10",Systematic::Type::ENVELOPE,"_electronIDSF_s10_m0","sim");
  AddSystematic("electronIDSF_sys11","electronIDSF_sys11",Systematic::Type::ENVELOPE,"_electronIDSF_s11_m0","sim");
  AddSystematic("electronIDSF","electronIDSF_sys",Systematic::Type::MULTI,FormRange("electronIDSF_sys%d",Range(1,12)),"sim");
  AddSystematic("muonTrackingSF_sys1","muonTrackingSF_sys1",Systematic::Type::ENVELOPE,"_muonTrackingSF_s1_m0 _muonTrackingSF_s1_m1","sim");
  AddSystematic("muonTrackingSF_sys2","muonTrackingSF_sys2",Systematic::Type::ENVELOPE,"_muonTrackingSF_s2_m0 _muonTrackingSF_s2_m1","sim");
  AddSystematic("muonTrackingSF_sys3","muonTrackingSF_sys3",Systematic::Type::ENVELOPE,"_muonTrackingSF_s3_m0","sim");
  AddSystematic("muonTrackingSF_sys4","muonTrackingSF_sys4",Systematic::Type::ENVELOPE,"_muonTrackingSF_s4_m0 _muonTrackingSF_s4_m1","sim");
  AddSystematic("muonTrackingSF_sys5","muonTrackingSF_sys5",Systematic::Type::ENVELOPE,"_muonTrackingSF_s5_m0","sim");
  AddSystematic("muonTrackingSF_sys6","muonTrackingSF_sys6",Systematic::Type::ENVELOPE,"_muonTrackingSF_s6_m0","sim");
  AddSystematic("muonTrackingSF_sys7","muonTrackingSF_sys7",Systematic::Type::ENVELOPE,"_muonTrackingSF_s7_m0","sim");
  AddSystematic("muonTrackingSF_sys8","muonTrackingSF_sys8",Systematic::Type::ENVELOPE,"_muonTrackingSF_s8_m0","sim");
  AddSystematic("muonTrackingSF_sys9","muonTrackingSF_sys9",Systematic::Type::ENVELOPE,"_muonTrackingSF_s9_m0 _muonTrackingSF_s9_m1","sim");
  AddSystematic("muonTrackingSF_sys10","muonTrackingSF_sys10",Systematic::Type::ENVELOPE,"_muonTrackingSF_s10_m0 _muonTrackingSF_s10_m1","sim");
  AddSystematic("muonTrackingSF","muonTrackingSF_sys",Systematic::Type::MULTI,FormRange("muonTrackingSF_sys%d",Range(1,11)),"sim");
  AddSystematic("muonRECOSF_sys1","muonRECOSF_sys1",Systematic::Type::ENVELOPE,"_muonRECOSF_s1_m0 _muonRECOSF_s1_m1","sim");
  AddSystematic("muonRECOSF_sys2","muonRECOSF_sys2",Systematic::Type::ENVELOPE,"_muonRECOSF_s2_m0 _muonRECOSF_s2_m1","sim");
  AddSystematic("muonRECOSF_sys3","muonRECOSF_sys3",Systematic::Type::ENVELOPE,"_muonRECOSF_s3_m0","sim");
  AddSystematic("muonRECOSF_sys4","muonRECOSF_sys4",Systematic::Type::ENVELOPE,"_muonRECOSF_s4_m0 _muonRECOSF_s4_m1","sim");
  AddSystematic("muonRECOSF_sys5","muonRECOSF_sys5",Systematic::Type::ENVELOPE,"_muonRECOSF_s5_m0","sim");
  AddSystematic("muonRECOSF_sys6","muonRECOSF_sys6",Systematic::Type::ENVELOPE,"_muonRECOSF_s6_m0","sim");
  AddSystematic("muonRECOSF_sys7","muonRECOSF_sys7",Systematic::Type::ENVELOPE,"_muonRECOSF_s7_m0","sim");
  AddSystematic("muonRECOSF_sys8","muonRECOSF_sys8",Systematic::Type::ENVELOPE,"_muonRECOSF_s8_m0","sim");
  AddSystematic("muonRECOSF_sys9","muonRECOSF_sys9",Systematic::Type::ENVELOPE,"_muonRECOSF_s9_m0 _muonRECOSF_s9_m1","sim");
  AddSystematic("muonRECOSF_sys10","muonRECOSF_sys10",Systematic::Type::ENVELOPE,"_muonRECOSF_s10_m0 _muonRECOSF_s10_m1","sim");
  AddSystematic("muonRECOSF","muonRECOSF_sys",Systematic::Type::MULTI,FormRange("muonRECOSF_sys%d",Range(1,11)),"sim");
  AddSystematic("muonIDSF_sys1","muonIDSF_sys1",Systematic::Type::ENVELOPE,"_muonIDSF_s1_m0 _muonIDSF_s1_m1","sim");
  AddSystematic("muonIDSF_sys2","muonIDSF_sys2",Systematic::Type::ENVELOPE,"_muonIDSF_s2_m0 _muonIDSF_s2_m1","sim");
  AddSystematic("muonIDSF_sys3","muonIDSF_sys3",Systematic::Type::ENVELOPE,"_muonIDSF_s3_m0","sim");
  AddSystematic("muonIDSF_sys4","muonIDSF_sys4",Systematic::Type::ENVELOPE,"_muonIDSF_s4_m0 _muonIDSF_s4_m1","sim");
  AddSystematic("muonIDSF_sys5","muonIDSF_sys5",Systematic::Type::ENVELOPE,"_muonIDSF_s5_m0","sim");
  AddSystematic("muonIDSF_sys6","muonIDSF_sys6",Systematic::Type::ENVELOPE,"_muonIDSF_s6_m0","sim");
  AddSystematic("muonIDSF_sys7","muonIDSF_sys7",Systematic::Type::ENVELOPE,"_muonIDSF_s7_m0","sim");
  AddSystematic("muonIDSF_sys8","muonIDSF_sys8",Systematic::Type::ENVELOPE,"_muonIDSF_s8_m0","sim");
  AddSystematic("muonIDSF_sys9","muonIDSF_sys9",Systematic::Type::ENVELOPE,"_muonIDSF_s9_m0 _muonIDSF_s9_m1","sim");
  AddSystematic("muonIDSF_sys10","muonIDSF_sys10",Systematic::Type::ENVELOPE,"_muonIDSF_s10_m0 _muonIDSF_s10_m1","sim");
  AddSystematic("muonIDSF","muonIDSF_sys",Systematic::Type::MULTI,FormRange("muonIDSF_sys%d",Range(1,11)),"sim");
  AddSystematic("triggerSF_sys1","triggerSF_sys1",Systematic::Type::ENVELOPE,"_triggerSF_s1_m0 _triggerSF_s1_m1","sim");
  AddSystematic("triggerSF_sys2","triggerSF_sys2",Systematic::Type::ENVELOPE,"_triggerSF_s2_m0 _triggerSF_s2_m1","sim");
  AddSystematic("triggerSF_sys3","triggerSF_sys3",Systematic::Type::ENVELOPE,"_triggerSF_s3_m0","sim");
  AddSystematic("triggerSF_sys4","triggerSF_sys4",Systematic::Type::ENVELOPE,"_triggerSF_s4_m0 _triggerSF_s4_m1","sim");
  AddSystematic("triggerSF_sys5","triggerSF_sys5",Systematic::Type::ENVELOPE,"_triggerSF_s5_m0","sim");
  AddSystematic("triggerSF_sys6","triggerSF_sys6",Systematic::Type::ENVELOPE,"_triggerSF_s6_m0","sim");
  AddSystematic("triggerSF_sys7","triggerSF_sys7",Systematic::Type::ENVELOPE,"_triggerSF_s7_m0 _triggerSF_s7_m1","sim");
  AddSystematic("triggerSF_sys8","triggerSF_sys8",Systematic::Type::ENVELOPE,"_triggerSF_s8_m0 _triggerSF_s8_m1","sim"); 
  AddSystematic("triggerSF_sys9","triggerSF_sys9",Systematic::Type::ENVELOPE,"_triggerSF_s9_m0 _triggerSF_s9_m1","sim"); 
  AddSystematic("triggerSF_sys10","triggerSF_sys10",Systematic::Type::ENVELOPE,"_triggerSF_s10_m0","sim");
  AddSystematic("triggerSF_sys11","triggerSF_sys11",Systematic::Type::ENVELOPE,"_triggerSF_s11_m0","sim");
  AddSystematic("triggerSF","triggerSF_sys",Systematic::Type::MULTI,FormRange("triggerSF_sys%d",Range(1,12)),"sim");

  AddSystematic("noRECOSF","noRECOSF",Systematic::Type::ENVELOPE,"_noRECOSF","sim");
  AddSystematic("noIDSF","noIDSF",Systematic::Type::ENVELOPE,"_noIDSF","sim");
  AddSystematic("noISOSF","noISOSF",Systematic::Type::ENVELOPE,"_noISOSF","sim");
  AddSystematic("notriggerSF","notriggerSF",Systematic::Type::ENVELOPE,"_notriggerSF","sim");

  AddSystematic("noefficiencySF","noefficiencySF",Systematic::Type::ENVELOPE,"_noefficiencySF","sim");
  AddSystematic("efficiencySF","eff. SF unc.",Systematic::Type::MULTI,"efficiencySF_stat electronRECOSF electronIDSF muonTrackingSF muonRECOSF muonIDSF triggerSF");
  AddSystematic("effAN","systematic unc.",Systematic::Type::MULTI,"efficiencySF z0weight");

  AddSystematic("qcdss_norm_ee","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.032","qcdss_ee");
  AddSystematic("qcdss_norm_ee_0bjet","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.031","qcdss_ee_0bjet");
  AddSystematic("qcdss_norm_ee_nbjet","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.057","qcdss_ee_nbjet");
  AddSystematic("qcdss_norm_mm","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.135","qcdss_mm");
  AddSystematic("qcdss_norm_mm_0bjet","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.124","qcdss_mm_0bjet");
  AddSystematic("qcdss_norm_mm_nbjet","SS method norm.",Systematic::Type::ENVELOPE,"scale:1.451","qcdss_mm_nbjet");
  AddSystematic("qcdss_norm","SS method norm.",Systematic::Type::MULTI,"qcdss_norm_ee qcdss_norm_ee_0bjet qcdss_norm_ee_nbjet qcdss_norm_mm qcdss_norm_mm_0bjet qcdss_norm_mm_nbjet");
  AddSystematic("qcdss_prompt","SS method prompt contam.",Systematic::Type::ENVELOPE,"scale:1.05","qcdss_prompt");
  AddSystematic("qcdss","SS method",Systematic::Type::MULTI,"qcdss_norm qcdss_prompt");
  AddSystematic("qcdtf","TF method",Systematic::Type::ENVELOPE,"_faketf_up _faketf_down");
  AddSystematic("qcd","data-driven QCD est.",Systematic::Type::MULTI,"qcdss qcdtf");

  AddSystematic("dyscale","#mu_{R}/#mu_{F}",Systematic::Type::ENVELOPE,"_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8","dy");
  AddSystematic("dyalphaS","alphaS",Systematic::Type::ENVELOPE,"_alphaS_up _alphaS_down","dy");
  AddSystematic("dypdf","PDF",Systematic::Type::HESSIAN,FormRange("_pdf%d",Range(100)),"dy");
  AddSystematic("nozptweight","nozptweight",Systematic::Type::ENVELOPE,"_nozptweight","dy");

  AddSystematic("ttscale","#mu_{R}/#mu_{F}",Systematic::Type::ENVELOPE,"_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8","ttll");
  AddSystematic("ttalphaS","alphaS",Systematic::Type::ENVELOPE,"_alphaS_up _alphaS_down","ttll");
  AddSystematic("ttpdf","PDF",Systematic::Type::HESSIAN,FormRange("_pdf%d",Range(100)),"ttll");
  AddSystematic("notopptweight","notopptweight",Systematic::Type::ENVELOPE,"_notopptweight","tt");

  AddSystematic("sys","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight noz0weight btag CFSF efficiencySF qcdss JEC");
  AddSystematic("dytheory","theory (DY)",Systematic::Type::MULTI,"dyalphaS dyscale dypdf nozptweight");
  AddSystematic("tttheory","theory (t#bar{t})",Systematic::Type::MULTI,"ttalphaS ttscale ttpdf notopptweight");
  AddSystematic("totalsys","syst. unc.",Systematic::Type::MULTI,"sys dytheory tttheory");
  AddSystematic("totalsys_table","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight btag CFSF efficiencySF dyalphaS dyscale dypdf ttalphaS ttscale ttpdf");
  AddSystematic("test","totalsys",Systematic::Type::MULTI,"btag dyscale ttscale");
}
#endif
