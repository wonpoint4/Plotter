#include"Plotter.cc"
#ifndef __EFFICIENCYPLOTTER_CC__
#define __EFFICIENCYPLOTTER_CC__
class EfficiencyPlotter:public Plotter{
public:
  void SetupEntries();
  void SetupSystematics();
  int Setup(int mode_=0);
  int mode;
  TString analyzer;
  EfficiencyPlotter(int mode_=-1);
  double GetChi2(TH1* h1,TH1* h2=NULL);
};
EfficiencyPlotter::EfficiencyPlotter(int mode_){
  analyzer="EfficiencyValidation";
  TObjArray* arr=gSystem->GetFromPipe("find $SKFlatOutputDir$SKFlatV/"+analyzer+"/ -type f").Tokenize("\n");
  for(int i=0;i<arr->GetEntries();i++){
    TString file=((TObjString*)arr->At(i))->String();
    TString year=file("/"+analyzer+"/[0-9]+/"); year=year(analyzer.Length()+2,4);
    TString key=file(analyzer+"_.*\\.root");
    key=key(analyzer.Length()+1,key.Index(".root")-analyzer.Length()-1)+"_"+year;
    Sample sample;
    sample.files.push_back(make_tuple(file,1.,"",""));
    samples[key]=sample;
  } 
  /*
  vector<TString> ids={"POGTight_PFIsoTight","POGTight_TrkIsoLoose","MediumID","MediumID_selective","MediumID_Q","MediumID_selective_Q","TightID_Q"};
  for(const auto& [key,sample]:samples){
    if(sample.type!=Sample::Type::UNDEF&&key.Contains(TRegexp("201[0-9]$"))){
      for(const auto& id:ids){
	samples[key+"_"+id]=sample%("_"+id);
	samples[key+"_"+id+"_noefficiencySF"]=(Sample(key+" (noSF)",Sample::Type::A,kMagenta)+sample)%("_"+id+"_noefficiencySF");
      }
    }
  }
  */
  samples["muon"]=Sample("data (ee)",Sample::Type::DATA,kBlack)+TRegexp("SkimTree_SMP_DoubleMuon_[A-Z].*_201[6-8]$");
  samples["electron"]=Sample("data (#mu#mu)",Sample::Type::DATA,kBlack)+TRegexp("SkimTree_SMP_.*EG.*_[A-Z].*_201[6-8]$");
  samples["data"]=Sample("data",Sample::Type::DATA,kBlack)+"muon"+"electron";
  samples["amc"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("SkimTree_SMP_DYJets_201[6-8]$");
  samples["amctt"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+TRegexp("SkimTree_SMP_DYJets_201[6-8]$"));
  samples["vv"]=Sample("Diboson",Sample::Type::BG,kBlue)+TRegexp("SkimTree_SMP_[W-Z][W-Z]_pythia_201[6-8]$");
  samples["wjets"]=Sample("W",Sample::Type::BG,kYellow)+TRegexp("SkimTree_SMP_WJets_MG_201[6-8]$");
  samples["tt"]=Sample("t#bar{t}",Sample::Type::BG,kMagenta)+TRegexp("SkimTree_SMP_TTLL_powheg_201[6-8]$");
  samples["sim"]=Sample("sim",Sample::Type::SUM,kRed)+"amc"+"amctt"+"vv"+"wjets"+"tt";
  if(mode_>-1) Setup(mode_);
}

int EfficiencyPlotter::Setup(int mode_){
  Reset();

  mode=mode_;

  SetupEntries();
  SetupSystematics();
  SetupPlots(Form("plots_EfficiencyValidation/mode%d/plot.dat",mode));

  if(DEBUG) cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(DEBUG) cout<<"[Setup] nsys: "<<systematics.size()<<endl;
  if(DEBUG) cout<<"[Setup] nplot: "<<plots.size()<<endl;

  return 1;
}

void EfficiencyPlotter::SetupEntries(){
  if(DEBUG)  cout<<"[EfficiencyPlotter::SetupEntries]"<<endl;
  entries.push_back(samples["data"]);
  entries.push_back(samples["sim"]);    
  if(mode==1){
    entries.push_back((Sample("sim_noefficiencySF",Sample::Type::A,kBlue)+"sim")%"_noefficiencySF");
  }
  if(DEBUG>1) PrintEntries(1);
  return;
}
void EfficiencyPlotter::SetupSystematics(){
  if(DEBUG)  cout<<"[SetupSystematics]"<<endl;
  systematics["RECOSF"]=MakeSystematic("RECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_RECOSF_up _RECOSF_down");
  systematics["IDSF"]=MakeSystematic("IDSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_IDSF_up _IDSF_down");
  systematics["ISOSF"]=MakeSystematic("ISOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_ISOSF_up _ISOSF_down");
  systematics["triggerSF"]=MakeSystematic("triggerSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_triggerSF_up _triggerSF_down");

  systematics["noRECOSF"]=MakeSystematic("noRECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noRECOSF");
  systematics["noIDSF"]=MakeSystematic("noIDSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noIDSF");
  systematics["noISOSF"]=MakeSystematic("noISOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noISOSF");
  systematics["notriggerSF"]=MakeSystematic("notriggerSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_notriggerSF");

  systematics["nozptcor"]=MakeSystematic("nozptcor",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL),"_nozptcor");
  systematics["noefficiencySF"]=MakeSystematic("noefficiencySF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noefficiencySF");
  systematics["efficiencySF"]=MakeSystematic("efficiencySF",Systematic::Type::MULTI,0,"RECOSF IDSF ISOSF triggerSF");

}

double EfficiencyPlotter::GetChi2(TH1* h1,TH1* h2){
  double chi2=0;
  for(int i=h1->GetXaxis()->GetFirst();i<h1->GetXaxis()->GetLast()+1;i++){
    double x1=h1->GetBinContent(i);
    double ex1=h1->GetBinError(i);
    double x2=h2?h2->GetBinContent(i):0.;
    double ex2=h2?h2->GetBinError(i):0.;
    chi2+=pow((x1-x2)/(ex1-ex2),2);
  }
  chi2/=h1->GetXaxis()->GetLast()-h1->GetXaxis()->GetFirst()+1;
  return chi2;
}
  
#endif
