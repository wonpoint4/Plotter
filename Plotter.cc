#ifndef __PLOTTER_CC__
#define __PLOTTER_CC__
#include"Global.h"
#include"Sample.cc"
#include"Plot.cc"
#include"Utils.h"
#include"Systematic.cc"
class Plotter{
public:
  Plotter();
  ~Plotter();
  vector<Sample> entries;
  map<TString,Systematic> systematics;
  map<TString,Plot> plots;
  static TDirectory *pdir;
  TString plotfile;
  TString plotdir;

  //Print
  void PrintSamples(TRegexp regexp=".*");
  void PrintEntries(bool detail=false,TRegexp regexp=".*");
  void PrintHistKeys(const Sample& sample,TRegexp regexp=".*");
  void PrintPlots(TRegexp reg=".*");
  void PrintSystematics();

  //Hist
  TH1* GetHistRaw(TString filename,TString histname);  
  TH1* GetHist(const Sample& sample,TString histname,TString suffix="",int varibit=0);
  tuple<TH1*,TH1*> GetHistPair(const Sample& sample,TString histname,TString sysname);
  TH1* GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den);
  TH1* GetHistAFB(TH1* hist_forward,TH1* hist_backward);

  //Uncertainty
  TH1* GetEnvelope(TH1* central,const vector<TH1*>& variations);
  TH1* GetEnvelope(TH1* central,TH1* variation1,TH1* variation2=NULL,TH1* variation3=NULL,TH1* variation4=NULL,TH1* variation5=NULL,TH1* variation6=NULL,TH1* variation7=NULL,TH1* variation8=NULL,TH1* variation9=NULL);
  TH1* GetHessianError(TH1* central,const vector<TH1*>& variations);
  TH1* GetRMSError(TH1* central,const vector<TH1*>& variations);
  int AddError(TH1* hist,TH1* sys);

  //Canvas
  vector<tuple<TH1*,TH1*>> GetHistPairs(Plot& plot);
  TCanvas* GetCompare(vector<tuple<TH1*,TH1*>> hists,TString option);
  vector<tuple<TH1*,TH1*>> GetRatioHistPairs(vector<tuple<TH1*,TH1*>> histpairs,TString option);
  TCanvas* GetRatio(vector<tuple<TH1*,TH1*>> hists,TString option);
  TCanvas* GetDiff(vector<tuple<TH1*,TH1*>> hists,TString option);
  TCanvas* GetCompareAndRatio(vector<tuple<TH1*,TH1*>> hists,TString option);
  TCanvas* GetCompareAndDiff(vector<tuple<TH1*,TH1*>> hists,TString option);
  TCanvas* GetDoubleRatio(Plot& plot);
  TCanvas* GetCanvas(TCanvas* (Plotter::*func)(vector<tuple<TH1*,TH1*>>,TString),TString histname,TString sysname,int rebin,double xmin,double xmax,TString option);
  TCanvas* GetCanvas(TCanvas* (Plotter::*func)(vector<tuple<TH1*,TH1*>>,TString),TString histname,TString suffix,int varibit,int rebin,double xmin,double xmax,TString option);
  TCanvas* GetCanvas(TString plotkey);
  vector<TCanvas*> GetCanvases(TRegexp plotkey);
  void SaveCanvases(TString plotkey);
  void SaveCanvas(TString plotkey);
  void SaveCanvasAll();

  //Canvas tools
  TString GetHistNameWithPrefixAndSuffix(TString histname,TString prefix,TString suffix);
  static TH1* GetAxisParent(TVirtualPad* pad);
  TH1* GetTH1(TH1* hstack);
  bool CheckHists(vector<TH1*> hists);
  static vector<TH1*> VectorTH1(vector<tuple<TH1*,TH1*>>& hists);
  tuple<double,double> GetMinMax(const vector<TH1*>& hists);
  void RebinXminXmax(TH1* hist,int rebin,double xmin,double xmax);
  void Normalize(vector<TH1*> hists,double val=1.);
  static TLegend* GetLegend(const vector<TH1*>& hists,TString option);
  static TLegend* GetLegend(const vector<tuple<TH1*,TH1*>>& hists,TString option);
  
  //Plot
  void SetupPlots(TString filename);
  set<TString> GetHistKeys(TList* keys,TRegexp regexp=".*");
  set<TString> GetHistKeys(const Sample& sample,TRegexp regexp=".*");
  set<TString> ParseHistKeys(set<TString> histkeys,set<TString> fixes,set<TString> excludes={});
  set<TString> GetParsedHistKeys(set<TString> excludes={});
  void AddPlot(TString plotkey,TString plotoption="");
  void AddPlots(TRegexp regexp=".*",TString plotoption="");
  void RemovePlot(TString plotkey);
  void RemovePlots(TRegexp regexp=".*");
  void SetPlotRebinXminXmaxAuto(TString plotkey);
  void SetPlotsRebinXminXmaxAuto();
  void SetPlotOption(TString plotname,TString option);
  void SetPlotsOption(TRegexp plotname,TString option);
  void RemovePlotOption(TString plotname,TString option);
  void RemovePlotsOption(TRegexp plotname,TString option);
  void SavePlotFile();

  //MakeSystematic
  Systematic MakeSystematic(TString name,Systematic::Type type,int varibit,vector<TString> includes);
  Systematic MakeSystematic(TString name,Systematic::Type type,int varibit,TString includes);

  //etc
  void Reset();
  
  //working

  //TCanvas* GetCompareAFBAll(vector<TString> histnames,int sysbit=0,TString option="");
  //TCanvas* GetCompareAFBAll(TRegexp regexp,int sysbit=0,TString option="");
};

TDirectory* Plotter::pdir=NULL;

Plotter::Plotter(){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::Plotter()]"<<endl;
  TH1::SetDefaultSumw2(kTRUE);
}
Plotter::~Plotter(){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::~Plotter()]"<<endl;
  SavePlotFile();
}


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Print ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Plotter::PrintSamples(TRegexp regexp){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::PrintSamples(TRegexp regexp)]"<<endl;
  for(const auto& [key,sample]:samples){
    if(key.Contains(regexp)){
      std::cout<<" @ Key: "<<key<<" ";
      sample.Print();
      std::cout<<endl;
    }
  }
}
void Plotter::PrintEntries(bool detail,TRegexp regexp){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::PrintEntries(bool detail,TRegexp regexp)]"<<endl;
  for(const auto& entry:entries){
    if(entry.title.Contains(regexp)){
      entry.Print(detail);
      std::cout<<endl;
    }
  }
}
void Plotter::PrintHistKeys(const Sample& sample,TRegexp regexp=".*"){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::PrintHistKeys(TString samplename,TRegexp regexp=\".*\")]"<<endl;
  set<TString> histkeys=GetHistKeys(sample,regexp);
  for(const auto& key:histkeys) std::cout<<key<<endl;
}
void Plotter::PrintPlots(TRegexp reg){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::PrintPlots(TRegexp reg)]"<<endl;
  for(auto it=plots.begin();it!=plots.end();it++){
    if(it->first.Contains(reg)) it->second.Print();
  }
}
void Plotter::PrintSystematics(){
  for(const auto& [key,sys]:systematics){
    sys.Print();
  }
}


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Hist /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TH1* Plotter::GetHistRaw(TString filename,TString histname){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHistRaw(TString filename,TString histname)]"<<endl;
  TFile *f=new TFile(filename);
  TH1* hist=(TH1*)f->Get(histname);
  if(hist){
    hist->SetDirectory(pdir);
    if(DEBUG>2) std::cout<<"###INFO### [Plotter::GetHistRaw] get "<<histname<<" in "<<filename<<endl;
  }else{
    if(DEBUG>1) std::cout<<"###WARNING### [Plotter::GetHistRaw] no "<<histname<<" in "<<filename<<endl;
  }
  f->Close();
  delete f;
  return hist;
}
TH1* Plotter::GetHist(const Sample& sample,TString histname,TString suffix,int varibit){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHist(const Sample& sample,TString histname,TString suffix,int varibit)]"<<endl;
  TH1* hist=NULL;
  if(histname.Contains("weightedAFB")){
    TString histname_forward_num=histname; histname_forward_num.ReplaceAll("weightedAFB","forward_num");
    TString histname_forward_den=histname; histname_forward_den.ReplaceAll("weightedAFB","forward_den");
    TString histname_backward_num=histname; histname_backward_num.ReplaceAll("weightedAFB","backward_num");
    TString histname_backward_den=histname; histname_backward_den.ReplaceAll("weightedAFB","backward_den");
    TH1* hist_forward_num=GetTH1(GetHist(sample,histname_forward_num,suffix,varibit));
    TH1* hist_forward_den=GetTH1(GetHist(sample,histname_forward_den,suffix,varibit));
    TH1* hist_backward_num=GetTH1(GetHist(sample,histname_backward_num,suffix,varibit));
    TH1* hist_backward_den=GetTH1(GetHist(sample,histname_backward_den,suffix,varibit));
    hist=GetHistWeightedAFB(hist_forward_num,hist_forward_den,hist_backward_num,hist_backward_den);
    delete hist_forward_num; delete hist_forward_den; delete hist_backward_num; delete hist_backward_den;
    return hist;
  }else if(histname.Contains("AFB")){
    TString histname_forward=histname; histname_forward.ReplaceAll("AFB","forward");
    TString histname_backward=histname; histname_backward.ReplaceAll("AFB","backward");
    TH1* hist_forward=GetTH1(GetHist(sample,histname_forward,suffix,varibit));
    TH1* hist_backward=GetTH1(GetHist(sample,histname_backward,suffix,varibit));
    hist=GetHistAFB(hist_forward,hist_backward);
    delete hist_forward; delete hist_backward;
    return hist;
  }else{
    if(sample.type==Sample::Type::STACK){
      for(const auto& sub:sample.subs){
	TH1* this_hist=GetHist(sub,histname,suffix,varibit);
	if(this_hist){
	  if(!hist){
	    hist=(TH1*)new THStack(sample.title,sample.title);
	    hist->SetDirectory(pdir);
	  }
	  ((THStack*)hist)->Add(this_hist,"HIST");
	}
      }
    }else if(sample.type==Sample::Type::SUM){
      for(const auto& sub:sample.subs){
	TH1* this_hist=GetHist(sub,histname,suffix,varibit);
	if(this_hist){
	  if(!hist){
	    hist=(TH1*)this_hist->Clone();
	    hist->SetDirectory(pdir);
	    hist->Reset();
	  }
	  hist->Add(this_hist);
	  delete this_hist;
	}
      }
    }else{
      for(const auto& [filepath,weight,pre,suf]:sample.files){
	TString finalhistname=GetHistNameWithPrefixAndSuffix(histname,pre,suf+((1<<sample.type)&varibit?suffix:""));
	TH1* this_hist=GetHistRaw(filepath,finalhistname);
	if(this_hist){
	  this_hist->SetName(sample.title);
	  if(!hist){
	    hist=(TH1*)this_hist->Clone();
	    hist->SetDirectory(pdir);
	    hist->Reset();
	  }
	  hist->Add(this_hist,weight);
	  delete this_hist;
	}
      }
    }
    sample.ApplyStyle(hist);
    hist->SetTitle(histname+suffix);
    return hist;
  }
}
tuple<TH1*,TH1*> Plotter::GetHistPair(const Sample& sample,TString histname,TString sysname){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHistPair(const Sample& sample,TString histname,TString sysname)]"<<endl;
  TH1* central=GetHist(sample,histname);
  THStack* hstack=NULL;
  if(strstr(central->ClassName(),"THStack")!=NULL){
    hstack=(THStack*)central;
    central=GetTH1((TH1*)hstack);
  }
  int sysbit=systematics[sysname].sysbit;
  vector<TH1*> syss;
  for(const auto& sysmap:systematics){
    const Systematic& systematic=sysmap.second;
    if(systematic.type==Systematic::Type::MULTI) continue;
    if(sysbit&systematic.sysbit){
      if(DEBUG>1) std::cout<<"###INFO### [Plotter::GetHistPair] sysname="<<systematic.name<<" systype="<<systematic.GetTypeString()<<endl;
      vector<TH1*> variations;
      for(const auto& suffix:systematic.suffixes){
	TH1* this_hist=GetHist(sample,histname,suffix,systematic.varibit);
	variations.push_back(GetTH1(this_hist));
	delete this_hist;
      }
      if(systematic.type==Systematic::Type::ENVELOPE){
	syss.push_back(GetEnvelope(central,variations));
      }else if(systematic.type==Systematic::Type::GAUSSIAN){
	syss.push_back(GetRMSError(central,variations));
      }else if(systematic.type==Systematic::Type::HESSIAN){
	syss.push_back(GetHessianError(central,variations));
      }else{
	if(DEBUG>0) std::cout<<"###ERROR### [Plotter::GetHistPair] Wrong Systematic::Type "<<systematic.type<<endl;
      }
      if(DEBUG>2) std::cout<<"###INFO### [Plotter::GetHistPair] "<<systematic.name+": "<<variations.size()<<" variations"<<endl;
      for(unsigned int j=0;j<variations.size();j++){
	delete variations.at(j);
      }
    }
  }
  TH1 *total=NULL;
  if(syss.size()>0){
    total=(TH1*)central->Clone();
    total->SetDirectory(pdir);
    for(int i=0;i<(int)syss.size();i++){
      AddError(total,syss.at(i));
      delete syss.at(i);
    }
  }
  if(hstack){
    delete central;
    central=(TH1*)hstack;
  }
  central->SetLineWidth(2);
  return make_tuple(central,total);
}

TH1* Plotter::GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den)]"<<endl;
  TH1* hist=(TH1*)hist_forward_num->Clone();
  hist->SetDirectory(pdir);
  hist->Reset();
  for(int i=0;i<hist->GetNbinsX()+2;i++){
    double valfn=hist_forward_num->GetBinContent(i);
    double valbn=hist_backward_num->GetBinContent(i);
    double valfd=hist_forward_den->GetBinContent(i);
    double efd=hist_forward_den->GetBinError(i);
    double valbd=hist_backward_den->GetBinContent(i);
    double ebd=hist_backward_den->GetBinError(i);
    hist->SetBinContent(i,3./8.*(valfd-valbd)/(valfn+valbn));
    hist->SetBinError(i,3./8.*(valbn*valfd+valfn*valbd)/pow(valfn+valbn,2)*sqrt(pow(efd/valfd,2)+pow(ebd/valbd,2)));
  }
  TString this_title=hist_forward_num->GetTitle();
  hist->SetTitle(this_title.ReplaceAll("forward_num","weightedAFB"));
  return hist;
}
TH1* Plotter::GetHistAFB(TH1* hist_forward,TH1* hist_backward){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHistAFB(TH1* hist_forward,TH1* hist_backward)]"<<endl;
  if(!hist_forward||!hist_backward){
    if(DEBUG>1) std::cout<<"###WARING### [Plotter::GetHistAFB] It is null TH1*"<<endl;
    return NULL;
  }
  TH1* hist=(TH1*)hist_forward->Clone();
  hist->SetDirectory(pdir);
  hist->Reset();
  for(int i=0;i<hist->GetNbinsX()+2;i++){
    double valf=hist_forward->GetBinContent(i);
    double ef=hist_forward->GetBinError(i);
    double valb=hist_backward->GetBinContent(i);
    double eb=hist_backward->GetBinError(i);
    hist->SetBinContent(i,(valf-valb)/(valf+valb));
    hist->SetBinError(i,2*sqrt(ef*ef*valb*valb+eb*eb*valf*valf)/pow(valf+valb,2));
  }
  TString this_title=hist_forward->GetTitle();
  hist->SetTitle(this_title.ReplaceAll("forward","AFB"));
  return hist;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Uncertainty //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TH1* Plotter::GetEnvelope(TH1* central,const vector<TH1*>& variations){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetEnvelope(TH1* central,const vector<TH1*>& variations)]"<<endl;
  if(strstr(central->ClassName(),"THStack")) central=GetTH1(central);
  TH1* syshist=(TH1*)central->Clone("sys");
  syshist->SetDirectory(pdir);
  for(int i=1;i<syshist->GetNbinsX()+1;i++) syshist->SetBinError(i,0);
  for(int i=0;i<(int)variations.size();i++){
    for(int j=0;j<syshist->GetNbinsX()+1;j++){
      double diff=fabs(syshist->GetBinContent(j)-variations.at(i)->GetBinContent(j));
      if(diff>syshist->GetBinError(j)) syshist->SetBinError(j,diff);
    }
  }
  return syshist;
}
TH1* Plotter::GetEnvelope(TH1* central,TH1* variation1,TH1* variation2=NULL,TH1* variation3=NULL,TH1* variation4=NULL,TH1* variation5=NULL,TH1* variation6=NULL,TH1* variation7=NULL,TH1* variation8=NULL,TH1* variation9=NULL){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetEnvelope(TH1* central,TH1* variation1,TH1* variation2=NULL,TH1* variation3=NULL,TH1* variation4=NULL,TH1* variation5=NULL,TH1* variation6=NULL,TH1* variation7=NULL,TH1* variation8=NULL,TH1* variation9=NULL)]"<<endl;
  vector<TH1*> variations;
  if(variation1) variations.push_back(variation1);
  if(variation2) variations.push_back(variation2);
  if(variation3) variations.push_back(variation3);
  if(variation4) variations.push_back(variation4);
  if(variation5) variations.push_back(variation5);
  if(variation6) variations.push_back(variation6);
  if(variation7) variations.push_back(variation7);
  if(variation8) variations.push_back(variation8);
  if(variation9) variations.push_back(variation9);
  return GetEnvelope(central,variations);
}    
TH1* Plotter::GetHessianError(TH1* central,const vector<TH1*>& variations){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHessianError(TH1* central,const vector<TH1*>& variations)]"<<endl;
  if(strstr(central->ClassName(),"THStack")) central=GetTH1(central);
  TH1* syshist=(TH1*)central->Clone("sys");
  syshist->SetDirectory(pdir);
  for(int i=1;i<syshist->GetNbinsX()+1;i++) syshist->SetBinError(i,0);
  for(int i=0;i<(int)variations.size();i++){
    for(int j=0;j<syshist->GetNbinsX()+1;j++){
      double diff=fabs(syshist->GetBinContent(j)-variations.at(i)->GetBinContent(j));
      syshist->SetBinError(j,sqrt(pow(syshist->GetBinError(j),2)+pow(diff,2)));
    }
  }
  return syshist;
}  
TH1* Plotter::GetRMSError(TH1* central,const vector<TH1*>& variations){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetRMSError(TH1* central,const vector<TH1*>& variations)]"<<endl;
  if(strstr(central->ClassName(),"THStack")) central=GetTH1(central);
  TH1* syshist=(TH1*)central->Clone("sys");
  syshist->SetDirectory(pdir);
  for(int i=1;i<syshist->GetNbinsX()+1;i++) syshist->SetBinError(i,0);
  for(int i=0;i<(int)variations.size();i++){
    for(int j=0;j<syshist->GetNbinsX()+1;j++){
      double diff=fabs(syshist->GetBinContent(j)-variations.at(i)->GetBinContent(j));
      syshist->SetBinError(j,sqrt(pow(syshist->GetBinError(j),2)+pow(diff,2)));
    }
  }
  for(int i=1;i<syshist->GetNbinsX()+1;i++) syshist->SetBinError(i,syshist->GetBinError(i)/sqrt(variations.size()));
  return syshist;
}  
int Plotter::AddError(TH1* hist,TH1* sys){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::AddError(TH1* hist,TH1* sys)]"<<endl;
  for(int i=1;i<hist->GetNbinsX()+1;i++){
    if(fabs(hist->GetBinContent(i)-sys->GetBinContent(i))*1000000>fabs(hist->GetBinContent(i))){
      if(DEBUG>0){
	std::cout<<"###ERROR### [AddError] systematic hist is wrong"<<endl;
	std::cout.precision(20);
	std::cout<<i<<" "<<hist->GetBinContent(i)<<" "<<sys->GetBinContent(i)<<" "<<fabs(hist->GetBinContent(i)-sys->GetBinContent(i))<<endl;
      }
      return -1;
    }
  }
  for(int i=1;i<hist->GetNbinsX()+1;i++){
    double err1=hist->GetBinError(i);
    double err2=sys->GetBinError(i);
    hist->SetBinError(i,sqrt(err1*err1+err2*err2));
  }
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Canvas ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
vector<tuple<TH1*,TH1*>> Plotter::GetHistPairs(Plot& plot){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHistPairs(Plot& plot)]"<<endl;
  vector<tuple<TH1*,TH1*>> histpairs;
  for(const auto& entry:entries){
    if(plot.varibit!=0){
      histpairs.push_back(make_tuple(GetHist(entry,plot.histname,plot.suffix,plot.varibit),(TH1*)NULL));
    }else if(plot.sysname!=""){
      histpairs.push_back(GetHistPair(entry,plot.histname,plot.sysname));
    }else{
      histpairs.push_back(make_tuple(GetHist(entry,plot.histname),(TH1*)NULL));
    }
    RebinXminXmax(get<0>(histpairs.back()),plot.rebin,plot.xmin,plot.xmax);
    RebinXminXmax(get<1>(histpairs.back()),plot.rebin,plot.xmin,plot.xmax);
  }
  if(plot.option.Contains("norm")) Normalize(VectorTH1(histpairs));
  return histpairs;
}  
TCanvas* Plotter::GetCompare(vector<tuple<TH1*,TH1*>> histpairs,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetCompare(vector<tuple<TH1*,TH1*>> histpairs,TString option)]"<<endl;
  vector<TH1*> hists=VectorTH1(histpairs);
  if(hists.size()==0){
    if(DEBUG>0) std::cout<<"###ERROR### [Plotter::GetCompare] hists is zero length"<<endl;
    return NULL;
  }
  TH1* axisowner=get<1>(histpairs[0]);
  if(!axisowner) axisowner=get<0>(histpairs[0]);

  TCanvas* c=new TCanvas;
  axisowner->SetStats(0);
  axisowner->Draw();
  if(c->GetPrimitive("title")) ((TPaveText*)c->GetPrimitive("title"))->SetTextSize(0.075);

  for(const auto& hist:hists)
    if(strstr(hist->ClassName(),"THStack")){
      hist->Draw("same");
      TH1* hist_sum=GetTH1(hist);
      hist_sum->Draw(TString("same ")+hist_sum->GetOption());
    }

  for(const auto& hist:hists)
    if(strstr(hist->ClassName(),"THStack")==NULL&&hist->GetFillColor()>0)
      hist->Draw(TString("same ")+hist->GetOption());

  for(const auto& hist:hists)
    if(strstr(hist->ClassName(),"THStack")==NULL&&hist->GetFillColor()==0)
      hist->Draw(TString("same ")+hist->GetOption());

  TLegend* legend=GetLegend(histpairs,option);
  if(!option.Contains("noleg")) legend->Draw();

  if(option.Contains("logy")) c->SetLogy();
  else{
    tuple<double,double> minmax=GetMinMax(hists);
    double minimum=get<0>(minmax),maximum=get<1>(minmax);
    double range=fabs(maximum-minimum);
    axisowner->GetYaxis()->SetRangeUser(minimum/range<-0.01?minimum-0.1*range:0,maximum+0.1*range);
  }
  return c;
}
vector<tuple<TH1*,TH1*>> Plotter::GetRatioHistPairs(vector<tuple<TH1*,TH1*>> histpairs,TString option){
  vector<tuple<TH1*,TH1*>> histpairs_new;
  TH1* base=NULL;
  TString baseopt=option("base:[0-9]*");
  if(baseopt!="") base=GetTH1(get<0>(histpairs[((TString)baseopt(5,100)).Atoi()]));
  else if(histpairs.size()>2) base=GetTH1(get<0>(histpairs[0]));
  else base=GetTH1(get<0>(histpairs[1]));
  
  for(int i=0;i<base->GetNbinsX()+2;i++){
    base->SetBinError(i,0);
  }
  for(const auto& [hist0,hist1]:histpairs){
    TH1 *hist0_new=NULL,*hist1_new=NULL;
    if(hist0){
      hist0_new=GetTH1(hist0);
      hist0_new->Divide(base);
    }
    if(hist1){
      hist1_new=GetTH1(hist1);
      hist1_new->Divide(base);
    }
    histpairs_new.push_back(make_tuple(hist0_new,hist1_new));
  }  
  delete base;
  return histpairs_new;
}  
TCanvas* Plotter::GetRatio(vector<tuple<TH1*,TH1*>> histpairs,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetRatio(vector<tuple<TH1*,TH1*>> histpairs,TString option)]"<<endl;
  vector<tuple<TH1*,TH1*>> histpairs_new=GetRatioHistPairs(histpairs,option);
  TCanvas* c=GetCompare(histpairs_new,option.ReplaceAll("norm",""));
  TH1* axisowner=GetAxisParent(c);
  axisowner->GetYaxis()->SetTitle("Ratio");
  if(option.Contains("widewidey")){
    axisowner->GetYaxis()->SetRangeUser(0.01,1.99);
    axisowner->GetYaxis()->SetNdivisions(506);
  }else if(option.Contains("widey")){
    axisowner->GetYaxis()->SetRangeUser(0.501,1.499);
    axisowner->GetYaxis()->SetNdivisions(506);
  }else{
    axisowner->GetYaxis()->SetRangeUser(0.801,1.199);
    axisowner->GetYaxis()->SetNdivisions(504);
  }
  return c;
}
TCanvas* Plotter::GetDiff(vector<tuple<TH1*,TH1*>> tu_hists,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetDiff(vector<tuple<TH1*,TH1*>> tu_hists,TString option)]"<<endl;
  vector<tuple<TH1*,TH1*>> tu_hists_new;
  TH1* base=NULL;
  TString baseopt=option("base:[0-9]*");
  if(baseopt!="") base=GetTH1(get<0>(tu_hists[((TString)baseopt(5,100)).Atoi()]));
  else if(tu_hists.size()>2) base=GetTH1(get<0>(tu_hists[0]));
  else base=GetTH1(get<0>(tu_hists[1]));
  
  for(int i=0;i<base->GetNbinsX()+2;i++){
    base->SetBinError(i,0);
  }
  for(const auto& histtuple:tu_hists){
    TH1 *hist0=NULL,*hist1=NULL;
    if(get<0>(histtuple)){
      hist0=GetTH1(get<0>(histtuple));
      hist0->Add(base,-1);
    }
    if(get<1>(histtuple)){
      hist1=(TH1*)get<1>(histtuple)->Clone();
      hist1->SetDirectory(pdir);
      hist1->Add(base,-1);
    }
    tu_hists_new.push_back(make_tuple(hist0,hist1));
  }  
  delete base;
  TCanvas* c=GetCompare(tu_hists_new,option);
  TH1* axisowner=GetAxisParent(c);
  
  axisowner->GetYaxis()->SetTitle("Diff");
  axisowner->GetYaxis()->SetLabelSize(0.06);

  tuple<double,double> minmax=GetMinMax(VectorTH1(tu_hists_new));
  double minimum=get<0>(minmax),maximum=get<1>(minmax);
  double range=fabs(maximum-minimum);
  axisowner->GetYaxis()->SetRangeUser(minimum/range<-0.01?minimum-0.1*range:0,maximum+0.1*range);
  return c;
}
TCanvas* Plotter::GetCompareAndRatio(vector<tuple<TH1*,TH1*>> hists,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetCompareAndRatio(vector<tuple<TH1*,TH1*>> hists,TString option)]"<<endl;
  TString option1,option2;
  for(const auto& opt:Split(option," ")){
    if(opt.Contains("1:")) option1+=opt(2,100)+" ";
    else if(opt.Contains("2:")) option2+=opt(2,100)+" ";
    else{
      option1+=opt+" ";
      option2+=opt+" ";
    }
  } 
    
  TCanvas* c=new TCanvas;
  c->Divide(1,2);
  TCanvas* c1temp=GetCompare(hists,option1);
  c->cd(1);
  c1temp->DrawClonePad();
  delete c1temp;
  if(c->GetPad(1)->GetPrimitive("title")) ((TPaveText*)c->GetPad(1)->GetPrimitive("title"))->SetTextSize(0.075);
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  TH1* axisparent=GetAxisParent(gPad);
  axisparent->GetXaxis()->SetLabelSize(0);
  axisparent->GetXaxis()->SetTitle("");

  TCanvas* c2temp=GetRatio(hists,option2+" noleg");
  c->cd(2);
  c2temp->DrawClonePad();
  delete c2temp;
  gPad->SetPad(0,0,1,0.365);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.2);
  gPad->SetGridx();gPad->SetGridy();
  gPad->SetFillStyle(0);
  axisparent=GetAxisParent(gPad);
  axisparent->SetTitle("");
  axisparent->SetStats(0);
  axisparent->GetYaxis()->SetLabelSize(0.1);
  axisparent->GetYaxis()->SetTitle("Ratio");
  axisparent->GetYaxis()->SetTitleSize(0.1);
  axisparent->GetYaxis()->SetTitleOffset(0.5);
  axisparent->GetXaxis()->SetTitle(axisparent->GetTitle());
  axisparent->GetXaxis()->SetTitleSize(0.09);
  axisparent->GetXaxis()->SetLabelSize(0.09);
  
  return c;
}
TCanvas* Plotter::GetCompareAndDiff(vector<tuple<TH1*,TH1*>> hists,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetCompareAndDiff(vector<tuple<TH1*,TH1*>> hists,TString option)]"<<endl;
  TString option1,option2;
  for(const auto& optionelement:*option.Tokenize(" ")){
    TString opt=((TObjString*)optionelement)->String();
    if(opt.Contains("1:")) option1+=opt(2,100)+" ";
    if(opt.Contains("2:")) option2+=opt(2,100)+" ";
    else{
      option1+=opt+" ";
      option2+=opt+" ";
    }
  } 
    
  TCanvas* c=new TCanvas;
  c->Divide(1,2);
  TCanvas* c1temp=GetCompare(hists,option1);
  c->cd(1);
  c1temp->DrawClonePad();
  delete c1temp;
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  TH1* axisparent=GetAxisParent(gPad);
  axisparent->GetXaxis()->SetLabelSize(0);
  axisparent->GetXaxis()->SetTitle("");

  TCanvas* c2temp=GetDiff(hists,option2+" noleg");
  c->cd(2);
  c2temp->DrawClonePad();
  delete c2temp;
  gPad->SetPad(0,0,1,0.365);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.2);
  gPad->SetGridx();gPad->SetGridy();
  gPad->SetFillStyle(0);

  axisparent=GetAxisParent(gPad);
  axisparent->SetTitle("");
  axisparent->SetStats(0);
  axisparent->GetYaxis()->SetLabelSize(0.1);
  axisparent->GetYaxis()->SetTitleSize(0.1);
  axisparent->GetYaxis()->SetTitleOffset(0.5);
  axisparent->GetXaxis()->SetTitle(axisparent->GetTitle());
  axisparent->GetXaxis()->SetTitleSize(0.09);
  axisparent->GetXaxis()->SetLabelSize(0.09);
  return c;
}
TCanvas* Plotter::GetDoubleRatio(Plot& plot){
  vector<vector<tuple<TH1*,TH1*>>> vec_histpairs;
  for(auto& sub:plot.subplots){
    vec_histpairs.push_back(GetRatioHistPairs(GetHistPairs(sub),plot.option));
  }
  int baseindex=0;
  if(plot.subplots.size()==2){
    baseindex=1;
    for(int i=0;i<vec_histpairs[baseindex].size();i++){
      TH1* hist=get<0>(vec_histpairs[baseindex][i]);
      bool flag=false;
      for(int j=1;j<hist->GetNbinsX()+1;j++) if(hist->GetBinContent(j)!=1.) flag=true;
      if(!flag){
	for(int j=0;j<vec_histpairs.size();j++){
	  vec_histpairs[j].erase(vec_histpairs[j].begin()+i);
	}
      }
    }
    vector<tuple<TH1*,TH1*>> histpairs_new;
    for(int i=0;i<vec_histpairs[baseindex].size();i++){
      TH1* base=(TH1*)get<0>(vec_histpairs[baseindex][i])->Clone();
      for(int j=0;j<base->GetNbinsX()+2;j++) base->SetBinError(j,0);
      for(int j=0;j<vec_histpairs.size();j++){
	if(get<0>(vec_histpairs[j][i])) get<0>(vec_histpairs[j][i])->Divide(base);
	if(get<1>(vec_histpairs[j][i])) get<1>(vec_histpairs[j][i])->Divide(base);
	histpairs_new.push_back(make_tuple(get<0>(vec_histpairs[j][i]),get<1>(vec_histpairs[j][i])));
      }
      delete base;
    }
    TCanvas* c=GetCompare(histpairs_new,plot.option.ReplaceAll("norm",""));
    TH1* axisowner=GetAxisParent(c);
    axisowner->GetYaxis()->SetTitle("DoubleRatio");
    if(plot.option.Contains("widewidey")){
      axisowner->GetYaxis()->SetRangeUser(0.01,1.99);
      axisowner->GetYaxis()->SetNdivisions(506);
    }else if(plot.option.Contains("widey")){
      axisowner->GetYaxis()->SetRangeUser(0.501,1.499);
      axisowner->GetYaxis()->SetNdivisions(506);
    }else{
      axisowner->GetYaxis()->SetRangeUser(0.801,1.199);
      axisowner->GetYaxis()->SetNdivisions(504);
    }
    return c;
  }else{
    if(DEBUG>0) std::cout<<"###ERROR### not yet"<<endl;
    exit(1);
  }
}
  

TCanvas* Plotter::GetCanvas(TCanvas* (Plotter::*func)(vector<tuple<TH1*,TH1*>>,TString),TString histname,TString sysname,int rebin,double xmin,double xmax,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetCanvas(TCanvas* (*func)(vector<tuple<TH1*,TH1*>>,TString),TString histname,TString sysname,int rebin,double xmin,double xmax,TString option)]"<<endl;
  vector<tuple<TH1*,TH1*>> hists;
  for(const auto& entry:entries){
    hists.push_back(GetHistPair(entry,histname,sysname));
    RebinXminXmax(get<0>(hists.back()),rebin,xmin,xmax);
    RebinXminXmax(get<1>(hists.back()),rebin,xmin,xmax);
  }
  return (this->*func)(hists,option);
}
TCanvas* Plotter::GetCanvas(TCanvas* (Plotter::*func)(vector<tuple<TH1*,TH1*>>,TString),TString histname,TString suffix,int varibit,int rebin,double xmin,double xmax,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetCanvas(TCanvas* (*func)(vector<tuple<TH1*,TH1*>>,TString),TString histname,TString suffix,int varibit,int rebin,double xmin,double xmax,TString option)]"<<endl;
  vector<tuple<TH1*,TH1*>> hists;
  for(const auto& entry:entries){
    hists.push_back(make_tuple(GetHist(entry,histname,suffix,varibit),(TH1*)NULL));
    RebinXminXmax(get<0>(hists.back()),rebin,xmin,xmax);
    RebinXminXmax(get<1>(hists.back()),rebin,xmin,xmax);
  }
  return (this->*func)(hists,option);
}
TCanvas* Plotter::GetCanvas(TString plotkey){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetCanvas(TString plotkey)]"<<endl;
  Plot& plot=plots[plotkey];
  TCanvas* c=NULL;
  if(plot.type==Plot::Type::DoubleRatio){
    c=GetDoubleRatio(plot);
  }else{
    vector<tuple<TH1*,TH1*>> histpairs=GetHistPairs(plot);
    if(plot.type==Plot::Type::CompareAndRatio) c=GetCompareAndRatio(histpairs,plot.option);
    else if(plot.type==Plot::Type::CompareAndDiff) c=GetCompareAndDiff(histpairs,plot.option);
  }    
  return c;
}
vector<TCanvas*> Plotter::GetCanvases(TRegexp plotkey){
  vector<TCanvas*> canvases;
  for(const auto& [key,plot]:plots)
    if(key.Contains(plotkey)) canvases.push_back(GetCanvas(key));
  return canvases;
}
void Plotter::SaveCanvas(TString plotkey){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::SaveCanvas(TString plotkey)]"<<endl;
  pdir=new TDirectory;
  TCanvas* c=GetCanvas(plotkey);
  gSystem->Exec("mkdir -p "+plotdir+"/"+Dirname(plotkey));
  c->SaveAs(plotdir+"/"+plotkey+".png");
  delete c;
  pdir->Delete();
}
void Plotter::SaveCanvases(TString plotkey){
  for(const auto& [key,plot]:plots)
    if(key.Contains(plotkey)) SaveCanvas(key);
}  
void Plotter::SaveCanvasAll(){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::SaveCanvasAll(TString plotkey)]"<<endl;
  for(const auto& [key,plot]:plots) SaveCanvas(key);
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Canvas Tools /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TString Plotter::GetHistNameWithPrefixAndSuffix(TString histname,TString prefix,TString suffix){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHistNameWithPrefixAndSuffix(TString histname,TString prefix,TString suffix)]"<<endl;
  TString this_histname=histname(0,histname.Last('/')+1)+prefix+histname(histname.Last('/')+1,histname.Length())+suffix;
  return this_histname;
}
TH1* Plotter::GetAxisParent(TVirtualPad* pad){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetAxisParent(TVirtualPad* pad)]"<<endl;
  TList* list=pad->GetListOfPrimitives();
  for(int i=0;i<list->GetSize();i++){
    if(strstr(list->At(i)->ClassName(),"TH")!=NULL) return (TH1*)list->At(i);
  }
  return NULL;
}
TH1* Plotter::GetTH1(TH1* hstack){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetTH1(TH1* hstack)]"<<endl;
  TH1* hist=NULL;
  if(strstr(hstack->ClassName(),"THStack")!=NULL){
    hist=(TH1*)((THStack*)hstack)->GetHists()->Last()->Clone();
    hist->SetDirectory(pdir);
    hist->Reset();
    for(const auto& this_hist:*((THStack*)hstack)->GetHists()){
      hist->Add((TH1*)this_hist);
    }
    //samples[hstack->GetName()].ApplyHistStyle(hist);
  }else{
    hist=(TH1*)hstack->Clone();
    hist->SetDirectory(pdir);
  }
  return hist;
}
bool Plotter::CheckHists(vector<TH1*> hists){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::CheckHists(vector<TH1*> hists)]"<<endl;
  bool flag_data=false,flag_signal=false;
  for(unsigned int i=0;i<entries.size();i++){
    if(!hists.at(i)) continue;
    if(entries[i].type==Sample::Type::DATA) flag_data=true;
    if(entries[i].type==Sample::Type::SIGNAL) flag_signal=true;
  }
  if(flag_data&&flag_signal) return true;
  else return false;
}
vector<TH1*> Plotter::VectorTH1(vector<tuple<TH1*,TH1*>>& hists){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::VectorTH1(const vector<tuple<TH1*,TH1*>>& hists)]"<<endl;
  vector<TH1*> hists_out;
  for(const auto& tu:hists){
    TH1* hist0=get<0>(tu); TH1* hist1=get<1>(tu); 
    if(hist1) hists_out.push_back(hist1);
    if(hist0) hists_out.push_back(hist0);
  }
  return hists_out;
}
tuple<double,double> Plotter::GetMinMax(const vector<TH1*>& hists){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetMinMax(vector<TH1*> hists)]"<<endl;
  double maximum=-999999999;
  double minimum=999999999;
  for(auto hist:hists){
    if(hist){
      if(strstr(hist->ClassName(),"THStack")) hist=GetTH1(hist);
      if(maximum<hist->GetMaximum()) maximum=hist->GetMaximum();
      if(minimum>hist->GetMinimum()) minimum=hist->GetMinimum();
    }
  }
  return make_tuple(minimum,maximum);
}
void Plotter::RebinXminXmax(TH1* hist,int rebin,double xmin,double xmax){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::RebinXminXmax(TH1* hist,int rebin,double xmin,double xmax)]"<<endl;
  if(hist){
    if(strstr(hist->ClassName(),"THStack")){
      for(const auto& obj:*((THStack*)hist)->GetHists())
	RebinXminXmax((TH1*)obj,rebin,xmin,xmax);
    }else{
      if(rebin) hist->Rebin(rebin);
      if(xmin||xmax) hist->GetXaxis()->SetRangeUser(xmin,xmax);
    }
  }
}
void Plotter::Normalize(vector<TH1*> hists,double val=1.){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::Normalize(vector<TH1*> hists,double val=1.)]"<<endl;
  for(auto hist:hists){
    if(strstr(hist->ClassName(),"THStack")){
      TH1* hsim=GetTH1(hist);
      double scale=val/hsim->Integral();
      for(auto obj:*((THStack*)hist)->GetHists()){
	TH1* hsim_sub=(TH1*)obj;
	hsim_sub->Scale(scale);
      }
    }else{
      hist->Scale(val/hist->Integral());
    }
  }
}
TLegend* Plotter::GetLegend(const vector<TH1*>& hists,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetLegend(const vector<TH1*>& hists,TString option)]"<<endl;
  vector<TH1*> hists_th1;
  for(const auto& hist:hists){
    if(strstr(hist->ClassName(),"THStack")){
      TIter next(((THStack*)hist)->GetHists(),kIterBackward);
      while(TObject *obj = next()){
	hists_th1.push_back((TH1*)obj);
      }
    }else hists_th1.push_back(hist);
  }
  int maxlen=0;
  for(const auto& hist:hists_th1)
    if(strlen(hist->GetName())>maxlen) maxlen=strlen(hist->GetName());

  if(maxlen<10) maxlen=10;
  
  vector<double> coordinates;
  double char_unit=0.013,entry_unit=0.07;
  if(option.Contains("leftleg")) coordinates={0.11,0.89,0.11+maxlen*char_unit,0.89-hists_th1.size()*entry_unit};
  else if(option.Contains("middlebottomleg")) coordinates={0.5-0.5*maxlen*char_unit,0.05,0.5+0.5*maxlen*char_unit,0.05+hists_th1.size()*entry_unit};
  else if(option.Contains("bottomleg")) coordinates={0.89,0.05,0.89-maxlen*char_unit,0.05+hists_th1.size()*entry_unit};
  else coordinates={0.89,0.89,0.89-maxlen*char_unit,0.89-hists_th1.size()*entry_unit};
  TLegend* legend=new TLegend(coordinates[0],coordinates[1],coordinates[2],coordinates[3]);

  for(const auto& hist:hists_th1) legend->AddEntry(hist,hist->GetName());
  legend->SetBorderSize(0);
  return legend;
}
TLegend* Plotter::GetLegend(const vector<tuple<TH1*,TH1*>>& hists,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetLegend(const vector<tuple<TH1*,TH1*>>& hists,TString option)]"<<endl;
  vector<TH1*> hists_final;
  for(const auto& histtuple : hists){
    hists_final.push_back(get<0>(histtuple));
  }
  return GetLegend(hists_final,option);
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Plot /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Plotter::SetupPlots(TString filename){
  plotdir=Dirname(filename);
  plotfile=Basename(filename);
  gSystem->Exec("mkdir -p "+plotdir);
  ifstream f(filename.Data());
  if(f.fail()){
    //UpdatePlots();
  }else{
    TString str;
    while(str.ReadLine(f)){
      Plot plot(str);
      plots[plot.name]=plot;
    }
  }
}
set<TString> Plotter::GetHistKeys(TList* keys,TRegexp regexp=".*"){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHistKeys(TList* keys,TRegexp regexp=\".*\")]"<<endl;
  set<TString> histkeys;
  for(const auto& obj:*keys){
    TKey* key=(TKey*)obj;
    if(strcmp(key->GetClassName(),"TDirectoryFile")==0){
      set<TString> this_histkeys=GetHistKeys(((TDirectoryFile*)key->ReadObj())->GetListOfKeys(),regexp);
      histkeys.insert(this_histkeys.begin(),this_histkeys.end());
    }else{
      TString path=key->GetMotherDir()->GetPath();
      path=path(path.Index(":")+2,path.Length());
      if(path!="") path+="/";
      path+=key->GetName();
      if(path.Contains(regexp)) histkeys.insert(path);
    }
  }
  return histkeys;
}
set<TString> Plotter::GetHistKeys(const Sample& sample,TRegexp regexp=".*"){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetHistKeys(const Sample& sample,TRegexp regexp=\".*\")]"<<endl;
  TFile f(get<0>(sample.files[0]));
  TList* keys=f.GetListOfKeys();
  set<TString> histkeys=GetHistKeys(keys,regexp);
  f.Close();
  return histkeys;
}
set<TString> Plotter::ParseHistKeys(set<TString> histkeys,set<TString> fixes,set<TString> excludes={}){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::ParseHistKeys(set<TString> histkeys_raw,set<TString> fixes,set<TString> excludes]"<<endl;
  set<TString> histkeys_out;
  for(auto key:histkeys){
    bool next_flag=false;

    for(const auto& exclude:excludes){
      if(key.Contains(TRegexp(exclude))){
	next_flag=true;
	break;
      }
    }
    if(next_flag) continue;

    for(const auto& fix:fixes){
      TString dirname=Dirname(key);
      TString basename=Basename(key);
      if(basename.Contains(TRegexp(fix))){
	key=dirname+"/"+Replace(basename,TRegexp(fix),"");
	key=Replace(key,"^[.]/","");
      }
    }
    key.ReplaceAll("forward","AFB");
    key.ReplaceAll("backward","AFB");
    key.ReplaceAll("AFB_num","weightedAFB");
    key.ReplaceAll("AFB_den","weightedAFB");
    histkeys_out.insert(key);
  }
  return histkeys_out;
}
set<TString> Plotter::GetParsedHistKeys(set<TString> excludes={}){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetParsedHistKeys(set<TString> excludes)]"<<endl;
  set<TString> histkeys;
  histkeys=GetHistKeys(entries[0]);
  set<TString> sys_fixes,sample_fixes;

  for(const auto& [sysname,sys]:systematics)
    for(const auto& suf:sys.suffixes)
      sys_fixes.insert(suf+"$");
  histkeys=ParseHistKeys(histkeys,sys_fixes,excludes);

  for(const auto& [samplename,sample]:samples)
    for(const auto& [filename,weight,prefix,suffix]:sample.files){
      if(prefix!="") sample_fixes.insert("^"+prefix);
      if(suffix!="") sample_fixes.insert(suffix+"$");
    }
  histkeys=ParseHistKeys(histkeys,sample_fixes);

  return histkeys;
}
void Plotter::AddPlot(TString plotkey,TString plotoption){
  Plot plot;
  plot.name=plotkey;plot.histname=plotkey;
  plot.SetOption(plotoption);
  plots[plotkey]=plot;
  //SetPlotRebinXminXmaxAuto(key);
} 
void Plotter::AddPlots(TRegexp regexp,TString plotoption){
  set<TString> histkeys=GetParsedHistKeys();
  for(const auto& key:histkeys){
    if(key.Contains(regexp)) AddPlot(key,plotoption);
  }
}
void Plotter::RemovePlot(TString plotkey){
  plots.erase(plotkey);
}
void Plotter::RemovePlots(TRegexp regexp){
  set<TString> to_remove;
  for(const auto& [key,plot]:plots)
    if(key.Contains(regexp)) to_remove.insert(key);
  for(const auto& key:to_remove)
    RemovePlot(key);
}   
void Plotter::SetPlotRebinXminXmaxAuto(TString plotkey){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::SetPlotRebinXminXmaxAuto()]"<<endl;
  Plot& plot=plots[plotkey];
  TH1 *hh=GetHist(entries[0],plot.histname);
  double maxrelerr=0;
  int ifirst=-1;
  int ilast=hh->GetNbinsX();
  for(int i=hh->GetXaxis()->GetFirst();i<hh->GetXaxis()->GetLast()+1;i++){
    double val=hh->GetBinContent(i);
    if(val==0) continue;
    if(ifirst==-1) ifirst=i;
    ilast=i;
    double err=hh->GetBinError(i);
    double relerr=fabs(err/val);
    if(maxrelerr<relerr) maxrelerr=relerr;
  }
  plot.rebin=pow(maxrelerr/0.05,2)>10?10:pow(maxrelerr/0.05,2);
  if(ifirst!=1||ilast!=hh->GetNbinsX()){
    plot.xmin=hh->GetBinLowEdge(ifirst);
    plot.xmax=hh->GetBinLowEdge(ilast)+hh->GetBinWidth(ilast)*(1-0.0000001);
    plot.Print();
  }
}
void Plotter::SetPlotsRebinXminXmaxAuto(){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::SetPlotsRebinXminXmaxAuto()]"<<endl;
  for(auto& [plotname,plot]:plots) SetPlotRebinXminXmaxAuto(plotname);
}
void Plotter::SetPlotOption(TString plotname,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::SetPlotOption(TString plotname,TString option)]"<<endl;
  plots[plotname].SetOption(option);
}
void Plotter::SetPlotsOption(TRegexp plotname_,TString option){
  for(auto& [plotname,plot]:plots){
    if(plotname.Contains(plotname_)) SetPlotOption(plotname,option);
  }
}
void Plotter::RemovePlotOption(TString plotname,TString option){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::RemovePlotOption(TString plotname,TString option)]"<<endl;
  plots[plotname].RemoveOption(option);
}
void Plotter::RemovePlotsOption(TRegexp plotname_,TString option){
  for(auto& [plotname,plot]:plots){
    if(plotname.Contains(plotname_)) RemovePlotOption(plotname,option);
  }
}
void Plotter::SavePlotFile(){
  if(plotfile!=""&&plots.size()>0){
    ofstream f(plotdir+"/"+plotfile);
    for(const auto& [plotname,plot]:plots) plot.Print(f);
  }
  plotfile="";
}  

///////////////////////////////////////////////////////////////////////
///////////////////// MakeSysteamtic //////////////////////////////////
///////////////////////////////////////////////////////////////////////
Systematic Plotter::MakeSystematic(TString name,Systematic::Type type,int varibit,vector<TString> includes){
  Systematic this_sys=Systematic(name,type,varibit);
  if(this_sys.type==Systematic::Type::MULTI){
    this_sys.sysbit=0;
    for(const auto& [syskey,sys]:systematics)
      for(int j=0;j<includes.size();j++)
        if(syskey==includes[j]){
          this_sys.sysbit|=sys.sysbit;
          break;
        }
  }else{
    this_sys.sysbit=1<<systematics.size();
    this_sys.suffixes=includes;
  }
  if(DEBUG>0) this_sys.Print();
  return this_sys;
}
Systematic Plotter::MakeSystematic(TString name,Systematic::Type type,int varibit,TString includes){
  return MakeSystematic(name,type,varibit,Split(includes," "));
}


///////////////////////////////////////////////////////////////////////
///////////////////// etc /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void Plotter::Reset(){
  entries.clear();
  systematics.clear();
  SavePlotFile();
  plots.clear();
  plotdir="";
}


//////////////////////working/////////////////////////
  

#endif













/*
TCanvas* Plotter::GetCompareAFBAll(vector<TString> histnames,int sysbit=0,TString option=""){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetCompareAFBAll(vector<TString> histnames,int sysbit=0,TString option="")]"<<endl;
  TString canvasname=histnames[0];
  canvasname.ReplaceAll("_y0.0to0.4","");
  int nhist=histnames.size();
  TCanvas* c1=new TCanvas(canvasname,canvasname,800,800);
  c1->Divide(nhist,1);
  for(int i=0;i<nhist;i++){
    TCanvas* ctemp=GetCompare(histnames[i],sysbit,0,0.,0.,option+(i==0?" ":" 1:noleg 2:noleg"));
    TH1* hdata=GetAxisParent(ctemp->GetPad(1));
    TH1* hratio=GetAxisParent(ctemp->GetPad(2));
    TLegend *leg1,*leg2;
    double sf0=(0.85/nhist)/(0.1+0.85/nhist);
    double sf1=(0.85/nhist)/(0.05+0.85/nhist);
    ctemp->GetPad(1)->SetLeftMargin(i==0?1-sf0:0);
    ctemp->GetPad(2)->SetLeftMargin(i==0?1-sf0:0);
    ctemp->GetPad(1)->SetRightMargin(i==nhist-1?1-sf1:0);
    ctemp->GetPad(2)->SetRightMargin(i==nhist-1?1-sf1:0);

    hdata->GetYaxis()->SetRangeUser(-0.19,0.33);
    hdata->GetXaxis()->SetNdivisions(503);

    TString ratiotitle=hratio->GetXaxis()->GetTitle();
*/
//    ratiotitle=ratiotitle("_y.*/");
/*
    ratiotitle=ratiotitle(1,ratiotitle.Length()-2);
    hratio->GetXaxis()->SetTitle(ratiotitle);
    hratio->GetXaxis()->CenterTitle();
    hratio->GetXaxis()->SetNdivisions(503);
    hratio->GetXaxis()->SetLabelSize(0.15);
    hratio->GetXaxis()->SetLabelOffset(-0.05);
    hratio->GetXaxis()->SetTitleOffset(0.5);
    hratio->GetXaxis()->SetTitleSize(0.15);
    if(i==0){
      leg1=(TLegend*)ctemp->GetPad(1)->GetPrimitive("TPave");
      leg1->SetX1(1-sf0+0.02);
      leg1->SetX2(0.9);
      leg1->SetTextSize(0.13);
      leg2=(TLegend*)ctemp->GetPad(2)->GetPrimitive("TPave");
      if(leg2){
	((TLegendEntry*)leg2->GetListOfPrimitives()->At(0))->SetLabel("Stat.");
	leg2->SetX1(1-sf0+0.02);
	leg2->SetX2(0.9);
	leg2->SetTextSize(0.13);
      }
      hdata->GetYaxis()->SetLabelSize(0.12);
      hdata->GetYaxis()->SetLabelOffset(0.02);
      hdata->GetYaxis()->SetTitle("A_{FB}");
      hdata->GetYaxis()->SetTitleSize(0.15);
      hdata->GetYaxis()->SetTitleOffset(1.3);

      hratio->GetYaxis()->SetLabelSize(0.1);
      hratio->GetYaxis()->SetLabelOffset(0.02);
      hratio->GetYaxis()->SetTitleOffset(1.8);
      hratio->GetYaxis()->SetTitleSize(0.12);

      hratio->GetXaxis()->SetLabelSize(hratio->GetXaxis()->GetLabelSize()*sf0);
      hratio->GetXaxis()->SetLabelOffset(0.001);
      hratio->GetXaxis()->SetTitleSize(hratio->GetXaxis()->GetTitleSize()*sf0);
      hratio->GetXaxis()->SetTitleOffset(hratio->GetXaxis()->GetTitleOffset()/sf0);
    }else if(i==nhist-1){
      hratio->GetXaxis()->SetLabelSize(hratio->GetXaxis()->GetLabelSize()*sf1);
      hratio->GetXaxis()->SetLabelOffset(0.025);
      hratio->GetXaxis()->SetTitleSize(hratio->GetXaxis()->GetTitleSize()*sf1);
      hratio->GetXaxis()->SetTitleOffset(hratio->GetXaxis()->GetTitleOffset()/sf1);
      hratio->GetXaxis()->SetLabelOffset(-0.02);
    }
    c1->cd(i+1);
    gPad->SetPad((i==0?0:0.1)+0.85*i/nhist,0.0,(i==nhist-1?0.15:0.1)+0.85*(i+1)/nhist,1);
    ctemp->GetPad(1)->SetGridx();
    ctemp->GetPad(1)->SetGridy();
    ctemp->DrawClonePad();
    delete ctemp;
  }
  c1->cd(0);
  TPad* titlepad=new TPad("titlepad","titlepad",0,0.94,1,1);
  titlepad->Draw();
  titlepad->cd();
  TPaveText* pavetitle=new TPaveText(0.1,0.1,0.9,0.9);
  pavetitle->AddText(c1->GetTitle());
  pavetitle->Draw();
  return c1;
}

TCanvas* Plotter::GetCompareAFBAll(TRegexp regexp,int sysbit=0,TString option=""){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::GetCompareAFBAll(TRegexp regexp,int sysbit=0,TString option="")]"<<endl;
  vector<TString> histnames;
  for(auto it=plots.begin();it!=plots.end();it++){
    if(it->first.Contains(regexp)){
      histnames.push_back(it->second.name);
    }
  }
  return GetCompareAFBAll(histnames,sysbit,option);
}


void Plotter::SavePlots(TString outputdir="plot",int njob=1,int ijob=0){
  if(DEBUG>3) std::cout<<"###DEBUG### [Plotter::SavePlots(TString outputdir="plot",int njob=1,int ijob=0)]"<<endl;
  int oldlevel=gErrorIgnoreLevel;
  if(!DEBUG) gErrorIgnoreLevel=kWarning;

  set<TString> dirs;
  int smax=systematics.size();
  TCanvas* c=NULL;
  int nplot=(plots.size()+1)/njob;
  for(auto ip=next(plots.begin(),ijob*nplot);ip!=next(plots.begin(),(ijob+1)*nplot)&&ip!=plots.end();ip++){
    Plot *plot=&ip->second;
    TString histname=plot->name;
    TString dir=outputdir+"/"+histname(0,histname.Last('/'));
    if(dirs.find(dir)==dirs.end()){
      std::cout<<"mkdir -p "+dir<<endl;
      system("mkdir -p "+dir);
      dirs.insert(dir);
    }
    if(DEBUG>1) std::cout<<"###INFO### [SavePlots] save "<<outputdir+"/"+histname+".png"<<endl;
    c=GetCompare(histname,0,plot->rebin,plot->xmin,plot->xmax,plot->option);
    c->SaveAs(outputdir+"/"+histname+".png");
    THStack* hstack=(THStack*)c->GetPad(1)->GetPrimitive("hstack");
    if(hstack) hstack->GetHists()->Clear();
    delete c;
    for(int is=0;is<smax;is++){
      TString this_dir=dir+"/"+systematics[is].name;
      if(dirs.find(this_dir)==dirs.end()){
	if(DEBUG) std::cout<<"mkdir -p "+this_dir<<endl;
	system("mkdir -p "+this_dir);
	dirs.insert(this_dir);
      }
      if(DEBUG>1) std::cout<<"###INFO### [SavePlots] save "<<this_dir+"/"+histname(histname.Last('/'),histname.Length())+".png"<<endl;
      c=GetCompare(histname,systematics[is].sysbit,plot->rebin,plot->xmin,plot->xmax,plot->option);
      if(c){
	c->SaveAs(this_dir+"/"+histname(histname.Last('/'),histname.Length())+".png");
	THStack* hstack=(THStack*)c->GetPad(1)->GetPrimitive("hstack");
	if(hstack) hstack->GetHists()->Clear();
	delete c;
      }
      if(systematics[is].suffixes.size()==1){
	if(DEBUG>1) std::cout<<"###INFO### [SavePlots] save "<<this_dir+"/"+histname(histname.Last('/'),histname.Length())+"_raw.png"<<endl;
	c=GetCompare(histname+(systematics[is].vary_data?systematics[is].suffixes[0]:""),histname+(systematics[is].vary_signal?systematics[is].suffixes[0]:""),histname+(systematics[is].vary_bg?systematics[is].suffixes[0]:""),0,plot->rebin,plot->xmin,plot->xmax,plot->option);
	if(c){
	  c->SaveAs(this_dir+"/"+histname(histname.Last('/'),histname.Length())+"_raw.png");
	  THStack* hstack=(THStack*)c->GetPad(1)->GetPrimitive("hstack");
	  if(hstack) hstack->GetHists()->Clear();
	  delete c;
	}
      }
    }
  }  
  gErrorIgnoreLevel=oldlevel;
}
*/
