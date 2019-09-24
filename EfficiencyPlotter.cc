#include"Plotter.cc"

class EfficiencyPlotter:public Plotter{
public:
  void SetupEntries();
  void SetupSystematics();
  int Setup(int channel_,int year_,TString mode_,bool withno);
  TString mode;
  bool withno;
  int channel,year;
  TString schannel,syear;
  TString analyzer;
  EfficiencyPlotter();
  double GetChi2(TH1* h1,TH1* h2=NULL);
};
EfficiencyPlotter::EfficiencyPlotter(){
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
  
  vector<TString> syears={"2016","2017","2018"};
  for(const auto& syear:syears){
    samples["muon"+syear]=Sample("muon"+syear,Sample::Type::DATA,kBlack)+TRegexp("SkimTree_SMP_DoubleMuon_[A-Z].*_"+syear);
    samples["electron"+syear]=Sample("electron"+syear,Sample::Type::DATA,kBlack)+TRegexp("SkimTree_SMP_.*EG.*_[A-Z].*_"+syear);

    samples["amc"+syear]=Sample("amc"+syear,Sample::Type::SIGNAL,kRed)+TRegexp("SkimTree_SMP_DYJets_"+syear);
    samples["tau_amc"+syear]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::SIGNAL,kGreen)+TRegexp("SkimTree_SMP_DYJets_"+syear));
    samples["vv"+syear]=Sample("Diboson",Sample::Type::BG,kBlue)+TRegexp("SkimTree_SMP_[W-Z][W-Z]_pythia_"+syear);
    samples["wjets"+syear]=Sample("W",Sample::Type::BG,kYellow)+("SkimTree_SMP_WJets_MG_"+syear);
    samples["tt"+syear]=Sample("t#bar{t}",Sample::Type::BG,kMagenta)+("SkimTree_SMP_TTLL_powheg_"+syear);
  }

  vector<TString> ids={"POGTight_PFIsoTight","POGTight_TrkIsoLoose","MediumID","MediumID_selective","MediumID_Q","MediumID_selective_Q","TightID_Q"};
  for(const auto& [key,sample]:samples){
    if(sample.type!=Sample::Type::UNDEF&&key.Contains(TRegexp("201[0-9]$"))){
      for(const auto& id:ids){
	samples[key+"_"+id]=sample%("_"+id);
	samples[key+"_"+id+"_noefficiencySF"]=(Sample(key+" (noSF)",Sample::Type::A,kMagenta)+sample)%("_"+id+"_noefficiencySF");
      }
    }
  }
}

int EfficiencyPlotter::Setup(int channel_,int year_,TString mode_="",bool withno_=false){
  entries.clear();
  systematics.clear();
  plots.clear();
  channel=channel_;
  year=year_;
  mode=mode_;
  withno=withno_;
  schannel=GetStringChannel((Channel)channel);
  syear=Form("%d",year);

  SetupEntries();
  SetupSystematics();
  SetupPlots("plots_EfficiencyValidation/"+mode+"/"+schannel+syear+"_"+mode+".dat");

  if(DEBUG) cout<<"[Setup] nsample: "<<samples.size()<<endl;
  if(DEBUG) cout<<"[Setup] nsys: "<<systematics.size()<<endl;
  if(DEBUG) cout<<"[Setup] nplot: "<<plots.size()<<endl;

  return 1;
}

void EfficiencyPlotter::SetupEntries(){
  if(DEBUG)  cout<<"[EfficiencyPlotter::SetupEntries]"<<endl;
  vector<tuple<int,int,TString>> availables={make_tuple(0,2016,"POGTight_PFIsoTight"),
					     make_tuple(0,2017,"POGTight_PFIsoTight"),
					     make_tuple(0,2017,"POGTight_TrkIsoLoose_Q"),
					     make_tuple(0,2018,"POGTight_PFIsoTight"),
					     make_tuple(0,2018,"POGTight_TrkIsoLoose"),
					     make_tuple(1,2016,"MediumID"),
					     make_tuple(1,2016,"MediumID_selective_Q"),
					     make_tuple(1,2017,"MediumID"),
					     make_tuple(1,2017,"MediumID_Q"),
					     make_tuple(1,2017,"TightID_Q"),
					     make_tuple(1,2017,"MediumID_selective_Q"),
					     make_tuple(1,2018,"MediumID"),
					     make_tuple(1,2018,"MediumID_selective"),};
  
  for(const auto& avail:availables){
    if(make_tuple(channel,year,mode)==avail){
      entries.push_back(Sample("data",Sample::Type::DATA,kBlack)+(schannel+syear+"_"+mode));
      entries.push_back(Sample("sim",Sample::Type::SUM,kRed)+("amc"+syear+"_"+mode)+("tau_amc"+syear+"_"+mode)+("vv"+syear+"_"+mode)+("wjets"+syear+"_"+mode)+("tt"+syear+"_"+mode));
      if(withno){
	entries.push_back((Sample("sim_noefficiencySF",Sample::Type::A,kBlue)+("amc"+syear+"_"+mode)+("tau_amc"+syear+"_"+mode)+("vv"+syear+"_"+mode)+("wjets"+syear+"_"+mode)+("tt"+syear+"_"+mode))%"_noefficiencySF");
	//if(year==2018) samples["sim"]=MakeSample("sim",Sample::Type::SUM,kRed,make_tuple("mg"+syear+"_"+mode,1.),make_tuple("mgtt"+syear+"_"+mode,1.),make_tuple("vv"+syear+"_"+mode,1.),make_tuple("wjets"+syear+"_"+mode,1.),make_tuple("tt"+syear+"_"+mode,1.));
	//if(year==2018) samples["sim2"]=MakeSample("sim_noefficiencySF",Sample::Type::SUM,kBlue,make_tuple("mg"+syear+"_"+mode+"_noefficiencySF",1.),make_tuple("mgtt"+syear+"_"+mode+"_noefficiencySF",1.),make_tuple("vv"+syear+"_"+mode+"_noefficiencySF",1.),make_tuple("wjets"+syear+"_"+mode+"_noefficiencySF",1.),make_tuple("tt"+syear+"_"+mode+"_noefficiencySF",1.));
      }
      if(DEBUG>1) PrintEntries(1);
      return;
    }
  }
  cout<<"###ERROR#### [EfficiencyPlotter::SetupSamples] Not available configuration"<<endl;
  return;
}
void EfficiencyPlotter::SetupSystematics(){
  if(DEBUG)  cout<<"[SetupSystematics]"<<endl;
  if(channel==Channel::ELECTRON) systematics["RECOSF"]=MakeSystematic("RECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_RECOSF_up _RECOSF_down");
  systematics["IDSF"]=MakeSystematic("IDSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_IDSF_up _IDSF_down");
  if(channel==Channel::MUON) systematics["ISOSF"]=MakeSystematic("ISOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_ISOSF_up _ISOSF_down");
  systematics["triggerSF"]=MakeSystematic("triggerSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_triggerSF_up _triggerSF_down");

  if(channel==Channel::ELECTRON) systematics["noRECOSF"]=MakeSystematic("noRECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noRECOSF");
  systematics["noIDSF"]=MakeSystematic("noIDSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noIDSF");
  if(channel==Channel::MUON) systematics["noISOSF"]=MakeSystematic("noISOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noISOSF");
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
  
