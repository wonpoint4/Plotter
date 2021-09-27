#ifndef __PLOTTER_CC__
#define __PLOTTER_CC__
#include <set>
#include <TStyle.h>
#include <TGaxis.h>
#include <TH3.h>
#include <TROOT.h>
#include <TLatex.h>
#include <TGraph.h>
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
  map<TString,set<TString>> histkeys_cache;
  TDirectory *pdir=NULL;
  TString plotfile;
  TString plotdir="fig";

  //Setup
  void AddFile(TString key,TString file);
  void ScanFiles(TString path);
  virtual void SetupEntries(TString mode);
  void Reset();
  const Sample& GetEntry(TString entrytitle) const;
  void AddEntry(TString key);
  void AddEntry(const Sample& sample);

  //Print
  void PrintFiles(bool detail=false,TRegexp regexp=".*");
  void PrintSamples(bool detail=false,TRegexp regexp=".*");
  void PrintEntries(bool detail=false,TRegexp regexp=".*");
  void PrintHistKeys(const Sample& sample,TString regexp=".*");
  void PrintHistKeys(int ientry=0,TString regexp=".*");
  void PrintPlots(TRegexp reg=".*");
  void PrintSystematics();

  //Hist
  virtual TH1* GetHist(TString samplekey,TString plotkey,TString additional_option="");
  virtual TH1* GetHist(int ientry,TString plotkey,TString additional_option="");
  virtual TH1* GetHist(const Sample& sample,Plot p,TString additional_option="");
  virtual TH1* GetHistFromSample(const Sample& sample,const Plot& p,TString additional_option="");
  virtual TH1* GetHistFromFile(TString filename,TString histname);
  virtual void GetHistActionForAdditionalClass(TObject*& obj,Plot p);
  vector<TH1*> GetHistSys(const Sample& sample,const Plot& plot);
  vector<vector<TH1*>> GetHists(Plot& plot);

  //Systematic
  TH1* GetEnvelope(TH1* central,const vector<TH1*>& variations);
  TH1* GetEnvelope(TH1* central,TH1* variation1,TH1* variation2=NULL,TH1* variation3=NULL,TH1* variation4=NULL,TH1* variation5=NULL,TH1* variation6=NULL,TH1* variation7=NULL,TH1* variation8=NULL,TH1* variation9=NULL);
  TH1* GetHessianError(TH1* central,const vector<TH1*>& variations);
  TH1* GetRMSError(TH1* central,const vector<TH1*>& variations);
  int AddError(TH1* hist,TH1* sys);
  void AddSystematic(TString key,TString title,Systematic::Type type,int varibit,vector<TString> includes);
  void AddSystematic(TString key,TString title,Systematic::Type type,int varibit,TString includes);

  //Canvas
  void DrawCompare(Plot p);
  vector<vector<TH1*>> GetRatioHists(Plot p);
  void DrawRatio(Plot p);
  void DrawDiff(Plot p);
  void DrawSig(Plot p);
  void DrawCompareAndRatio(Plot p);
  void DrawCompareAndDiff(Plot p);
  void DrawCompareAndSig(Plot p);
  void DrawDoubleRatio(Plot& plot);
  virtual TCanvas* DrawPlot(Plot plotkey,TString additional_option="");
  virtual TCanvas* DrawPlot(TString plotkey,TString additional_option="");
  vector<TCanvas*> DrawPlots(TRegexp plotkey,TString additional_option="");
  void SavePlots(TRegexp plotkey);
  void SavePlot(TString plotkey,TString option="",bool delete_canvas=true);
  void SaveCanvas(TCanvas *c,TString path,bool delete_canvas=true);
  void SavePlotAll();

  //Canvas tools
  TString GetHistNameWithPrefixAndSuffix(TString histname,TString prefix,TString suffix);
  static TH1* GetAxisParent(TVirtualPad* pad=NULL);
  static TLegend* GetLegend(TVirtualPad* pad=NULL);
  static void SetLegendEntryLabel(TLegend* legend,int i,TString label);
  TH1* GetTH1(TH1* hstack,bool deleteorigin=false) const;
  bool CheckHists(vector<TH1*> hists);
  void DrawHistograms(Plot p);
  static vector<TH1*> VectorTH1(const vector<vector<TH1*>>& hists);
  pair<double,double> GetMinMax(const vector<TH1*>& hists,TString option="") const;
  pair<double,double> GetMinMax(const vector<vector<TH1*>>& hists,TString option="") const;
  void RebinXminXmax(TH1* hist,Plot p);
  double Normalize(TH1* hists,double val=1.,TString option="");
  vector<double> Normalize(vector<TH1*> hists,double val=1.,TString option="");
  vector<vector<double>> Normalize(vector<vector<TH1*>> hists,double val=1.,TString option="");
  void WidthWeight(TH1* hist);
  void WidthWeight(vector<TH1*> hists);
  bool IsEntry(const Sample& sample);
  static TLegend* DrawLegend(const Plot& p);
  
  //Plot
  set<TString> GetHistKeys(TDirectory* dir,TRegexp regexp=".*");
  set<TString> GetHistKeys(TString filename,TString regexp=".*");
  set<TString> GetHistKeys(const Sample& sample,TString regexp=".*");
  virtual Plot MakePlot(TString plotkey,TString plotoption="");

  //etc
  double GetChi2(TH1* h1,TH1* h2) const;
  int GetAutoColor() const;
  static vector<int> Range(int n);
  static vector<int> Range(int s,int n,int h=1);
  static vector<TString> FormRange(TString form,vector<int> range);
  
  //working

  //TCanvas* GetCompareAFBAll(vector<TString> histnames,int sysbit=0,TString option="");
  //TCanvas* GetCompareAFBAll(TRegexp regexp,int sysbit=0,TString option="");
};

Plotter::Plotter(){
  TH1::SetDefaultSumw2(kTRUE);
  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(700);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadColor(0);
  gStyle->SetFrameFillStyle(0);
  TGaxis::SetExponentOffset(-0.06,0.02);
  pdir=new TDirectory("plotdir","plotdir");
}
Plotter::~Plotter(){
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Setup ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Plotter::AddFile(TString key,TString file){
  Sample sample=Sample(file,Sample::Type::FILE);
  if(samples.find(key)==samples.end()){
    TFile f(file);
    if(f.IsZombie()) PError("[Plotter::AddFile] Wrong file "+file);
    else{
      char buf[64];
      int nbytes=0,objlen=0,keylen=0;
      f.GetRecordHeader(buf,100,64,nbytes,objlen,keylen);
      if(nbytes==1||nbytes>f.GetSize()){
	PWarning("[Plotter::AddFile] potentially corrupted file... recover using hadd");
	system("TEMPDIR=$(mktemp -d);hadd $TEMPDIR/temp.root "+file+";cp $TEMPDIR/temp.root "+file+";rm -r $TEMPDIR");
      }
      samples[key]=sample;
    }
  }else if(samples[key].type!=Sample::Type::FILE||samples[key].title!=file) PError("[Plotter::AddFile] sample "+key+" already exists");
}
void Plotter::ScanFiles(TString path){
  vector<TString> files=Split(gSystem->GetFromPipe("find "+path+" -type f -name '*.root'"),"\n");
  if(!path.EndsWith("/")) path+="/";
  for(const auto& file:files){
    TString key=Replace(file,path,"");
    key=Replace(key,".root$","");
    AddFile(key,file);
  }
}
void Plotter::SetupEntries(TString mode){
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[Plotter::SetupEntries] mode = '"<<mode<<"'"<<endl;
  entries.clear();
  vector<TString> entry_keys=Split(mode," ");
  for(auto entry_key:entry_keys) AddEntry(entry_key);
}
void Plotter::Reset(){
  entries.clear();
  systematics.clear();
  plots.clear();
  plotdir="fig";
}
const Sample& Plotter::GetEntry(TString entrytitle) const {
  for(const auto& entry:entries){
    if(entry.title==entrytitle) return entry;
  }
  PError("[Plotter::GetEntry(TString entrytitle)] no entry with title "+entrytitle);
  return entries.at(0);
}
void Plotter::AddEntry(TString key){
  TPRegexp("([+-])").Substitute(key," $1","g");
  Sample entry;
  if(key.BeginsWith("^")){
    entry=Sample("simulation",Sample::Type::STACK,Style(kRed,-1,3001,"e2"),Style(kCyan,-1,3001,"e2"));
    key=key(1,key.Length()-1);
  }else entry=Sample("simulation",Sample::Type::SUM);
  vector<TString> sample_keys=Split(key," ");
  for(int i=0;i<(int)sample_keys.size();i++){
    TObjArray *array=TPRegexp("^([+-]?[0-9.]*)[*]?(.*)$").MatchS(sample_keys[i]);
    TString sweight=((TObjString*)array->At(1))->GetString();
    if(sweight=="+"||sweight=="-") sweight+="1";
    else if(sweight=="") sweight="+1";
    double weight=sweight.Atof();
    TString sample_key=((TObjString*)array->At(2))->GetString();
    delete array;
    if(samples.find(sample_key)!=samples.end()){
      Sample sample=samples[sample_key];
      if(sample_keys.size()==1&&entry.type!=Sample::Type::STACK){
	entry=sample*weight;
      }else{
	if(i==0){
	  //entry.style=sample.style;
	  //entry.style_alt=sample.style_alt;
	}
	entry+=weight*sample;
      }
    }else PError("[Plotter::AddEntry] no sample with key "+sample_key);
  }
  AddEntry(entry);
}
void Plotter::AddEntry(const Sample& sample){
  if(sample.IsFile()){
    Sample s(sample.title,Sample::Type::UNDEF,GetAutoColor());
    s.weight=sample.weight;
    s.replace=sample.replace;
    s+=sample;
    entries.push_back(s);
  }else entries.push_back(sample);
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Print ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Plotter::PrintFiles(bool detail,TRegexp regexp){
  std::cout<<"--------------------------"<<endl;
  for(const auto& [key,sample]:samples){
    if(sample.type!=Sample::Type::FILE) continue;
    if(key.Contains(regexp)){
      std::cout<<"@Key: "<<key<<" "<<endl;
      if(detail) sample.Print(detail,"  ");
      std::cout<<"--------------------------"<<endl;
    }
  }
}
void Plotter::PrintSamples(bool detail,TRegexp regexp){
  std::cout<<"--------------------------"<<endl;
  for(const auto& [key,sample]:samples){
    if(sample.type==Sample::Type::FILE) continue;
    if(key.Contains(regexp)){
      std::cout<<"@Key: "<<key<<" "<<endl;
      sample.Print(detail,"  ");
      std::cout<<"--------------------------"<<endl;
    }
  }
}
void Plotter::PrintEntries(bool detail,TRegexp regexp){
  std::cout<<"--------------------------"<<endl;
  for(const auto& entry:entries){
    if(entry.title.Contains(regexp)){
      entry.Print(detail);
      std::cout<<"--------------------------"<<endl;
    }
  }
}
void Plotter::PrintHistKeys(const Sample& sample,TString regexp){
  set<TString> histkeys=GetHistKeys(sample,regexp);
  for(const auto& key:histkeys) std::cout<<key<<endl;
}
void Plotter::PrintHistKeys(int ientry,TString regexp){
  set<TString> histkeys=GetHistKeys(entries[ientry],regexp);
  for(const auto& key:histkeys) std::cout<<key<<endl;
}
void Plotter::PrintPlots(TRegexp reg){
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
TH1* Plotter::GetHist(TString samplekey,TString plotkey,TString additional_option){
  return GetHist(samples[samplekey],MakePlot(plotkey,additional_option));
}
TH1* Plotter::GetHist(int ientry,TString plotkey,TString additional_option){
  if(entries.size()<=ientry){
    PError("[Plotter::GetHist] entries.size()<=ientry");
    return NULL;
  }
  return GetHist(entries[ientry],MakePlot(plotkey,additional_option));
} 
TH1* Plotter::GetHist(const Sample& sample,Plot p,TString additional_option){
  PInfo("[Plotter::GetHist] "+sample.title+" "+p.histname+" "+p.option+" "+additional_option);_depth++;
  TH1* hist=GetHistFromSample(sample,p,additional_option);
  if(hist){
    hist->SetTitle(p.title);
    RebinXminXmax(hist,p);
    if(p.option.Contains("widthweight")) WidthWeight(hist);
  }
  _depth--;
  return hist;
}
TH1* Plotter::GetHistFromSample(const Sample& sample,const Plot& pp,TString additional_option){
  Plot p=pp;
  PInfo("[Plotter::GetHistFromSample] "+sample.title+" "+p.histname+" "+p.option+" "+additional_option);_depth++;
  //p.Print();
  p.SetOption(additional_option);
  TH1* hist=NULL;
  if(sample.type==Sample::Type::STACK){
    for(auto [reg,newstr]:sample.replace) TPRegexp(reg).Substitute(p.histname,newstr);
    for(int i=sample.subs.size()-1;i>=0;i--){
      TH1* this_hist=GetHistFromSample(sample.subs[i],p);
      if(this_hist){
	this_hist->Scale(sample.weight);
	if(!hist){
	  hist=(TH1*)new THStack(sample.title,sample.title);
	}
	((THStack*)hist)->Add(this_hist,"HIST");
      }
    }
  }else if(sample.type==Sample::Type::SUM){
    for(auto [reg,newstr]:sample.replace) TPRegexp(reg).Substitute(p.histname,newstr);
    for(const auto& sub:sample.subs){
      TH1* this_hist=GetHistFromSample(sub,p);
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
    if(hist) hist->Scale(sample.weight);
  }else if(p.option.Contains("absy")){
    if(p.project.Contains("y")) PError("[Plotter::GetHistFromSample] Not supported (absy + project:y)");
    else if(p.Ymin==0&&p.Ymax==0) PError("[Plotter::GetHistFromSample] absy needs Ymin and Ymax");
    else if(p.Ymin<0||p.Ymax<0) PError("[Plotter::GetHistFromSample] absy needs positive Ymin and Ymax");
    else{
      Plot plus_plot=p-"absy";
      TH1* plus_hist=GetHistFromSample(sample,plus_plot);
      Plot minus_plot=p-"absy";
      double Ymax_temp=-fabs(minus_plot.Ymin);
      minus_plot.Ymin=-fabs(minus_plot.Ymax);
      minus_plot.Ymax=Ymax_temp;
      TH1* minus_hist=GetHistFromSample(sample,minus_plot);
      for(auto& this_hist:{plus_hist,minus_hist}){
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
    }
  }else{
    for(auto [reg,newstr]:sample.replace) TPRegexp(reg).Substitute(p.histname,newstr);
    TString histname=p.histname;
    if((1<<sample.type&p.varibit)&&p.replace!=make_pair(TString(),TString())) TPRegexp(p.replace.first).Substitute(histname,p.replace.second);
    for(const auto& file:sample.subs){
      TH1* this_hist=GetHistFromFile(file.title,histname);
      if(this_hist){
	if(strstr(this_hist->ClassName(),"TH1")!=NULL){
	}else if(strstr(this_hist->ClassName(),"TH2")!=NULL){
	  TH2* hist2d=(TH2*)this_hist;
	  int ixmin=0,iymin=0;
	  int ixmax=-1,iymax=-1;
	  if(p.Xmin||p.Xmax){
	    ixmin=hist2d->GetXaxis()->FindBin(p.Xmin);
	    ixmax=hist2d->GetXaxis()->FindBin(p.Xmax-0.00001);
	    if(fabs(p.Xmin-hist2d->GetXaxis()->GetBinLowEdge(ixmin))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Xmin=%f is not exact edge... use %f",p.Xmin,hist2d->GetXaxis()->GetBinLowEdge(ixmin)));
	    if(fabs(p.Xmax-hist2d->GetXaxis()->GetBinUpEdge(ixmax))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Xmax=%f is not exact edge... use %f",p.Xmax,hist2d->GetXaxis()->GetBinUpEdge(ixmin)));
	  }
	  if(p.Ymin||p.Ymax){
	    iymin=hist2d->GetYaxis()->FindBin(p.Ymin);
	    iymax=hist2d->GetYaxis()->FindBin(p.Ymax-0.00001);
	    if(fabs(p.Ymin-hist2d->GetYaxis()->GetBinLowEdge(iymin))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Ymin=%f is not exact edge... use %f",p.Ymin,hist2d->GetYaxis()->GetBinLowEdge(iymin)));
	    if(fabs(p.Ymax-hist2d->GetYaxis()->GetBinUpEdge(iymax))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Ymax=%f is not exact edge... use %f",p.Ymax,hist2d->GetYaxis()->GetBinUpEdge(iymax)));
	  }
	  if(p.project=="") this_hist=(TH1*)hist2d->ProjectionY("_py",ixmin,ixmax);
	  else if(p.project=="x") this_hist=(TH1*)hist2d->ProjectionX("_px",iymin,iymax);
	  else if(p.project=="y") this_hist=(TH1*)hist2d->ProjectionY("_py",ixmin,ixmax);
	  else PError("[Plotter::GetHistFromSample] wrong projection or classname");
	  delete hist2d;
	}else if(strstr(this_hist->ClassName(),"TH3")!=NULL){
	  TH3* hist3d=(TH3*)this_hist;
	  int ixmin=0,iymin=0,izmin=0;
	  int ixmax=-1,iymax=-1,izmax=-1;
	  if(p.Xmin||p.Xmax){
	    ixmin=hist3d->GetXaxis()->FindBin(p.Xmin);
	    ixmax=hist3d->GetXaxis()->FindBin(p.Xmax-0.00001);
	    if(fabs(p.Xmin-hist3d->GetXaxis()->GetBinLowEdge(ixmin))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Xmin=%f is not exact edge... use %f",p.Xmin,hist3d->GetXaxis()->GetBinLowEdge(ixmin)));
	    if(fabs(p.Xmax-hist3d->GetXaxis()->GetBinUpEdge(ixmax))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Xmax=%f is not exact edge... use %f",p.Xmax,hist3d->GetXaxis()->GetBinUpEdge(ixmin)));
	  }
	  if(p.Ymin||p.Ymax){
	    iymin=hist3d->GetYaxis()->FindBin(p.Ymin);
	    iymax=hist3d->GetYaxis()->FindBin(p.Ymax-0.00001);
	    if(fabs(p.Ymin-hist3d->GetYaxis()->GetBinLowEdge(iymin))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Ymin=%f is not exact edge... use %f",p.Ymin,hist3d->GetYaxis()->GetBinLowEdge(iymin)));
	    if(fabs(p.Ymax-hist3d->GetYaxis()->GetBinUpEdge(iymax))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Ymax=%f is not exact edge... use %f",p.Ymax,hist3d->GetYaxis()->GetBinUpEdge(iymax)));
	  }
	  if(p.Zmin||p.Zmax){
	    izmin=hist3d->GetZaxis()->FindBin(p.Zmin);
	    izmax=hist3d->GetZaxis()->FindBin(p.Zmax-0.00001);
	    if(fabs(p.Zmin-hist3d->GetZaxis()->GetBinLowEdge(izmin))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Zmin=%f is not exact edge... use %f",p.Zmin,hist3d->GetZaxis()->GetBinLowEdge(izmin)));
	    if(fabs(p.Zmax-hist3d->GetZaxis()->GetBinUpEdge(izmax))>0.00001)
	      PWarning(Form("[Plotter::GetHistFromSample] Zmax=%f is not exact edge... use %f",p.Zmax,hist3d->GetZaxis()->GetBinUpEdge(izmax)));
	  }
	  if(!p.option.Contains("noproject")){
	    if(p.project=="") this_hist=(TH1*)hist3d->ProjectionZ("_pz",ixmin,ixmax,iymin,iymax);
	    else if(p.project=="x") this_hist=(TH1*)hist3d->ProjectionX("_px",iymin,iymax,izmin,izmax);
	    else if(p.project=="y") this_hist=(TH1*)hist3d->ProjectionY("_py",ixmin,ixmax,izmin,izmax);
	    else if(p.project=="z") this_hist=(TH1*)hist3d->ProjectionZ("_pz",ixmin,ixmax,iymin,iymax);
	    else PError("[Plotter::GetHistFromSample] wrong projection or classname");
	    delete hist3d;
	  }
	}else{
	  TObject* obj=(TObject*)this_hist;
	  GetHistActionForAdditionalClass(obj,p);
	  this_hist=(TH1*)obj;
	}
	this_hist->SetName(sample.title);
	if(!hist){
	  hist=(TH1*)this_hist->Clone();
	  hist->SetDirectory(pdir);
	  hist->Reset();
	}
	hist->Add(this_hist,sample.weight);
	delete this_hist;
      }
    }
    if(!hist&&(1<<sample.type&p.varibit)&&p.replace!=make_pair(TString(),TString())){
      PWarning("[Plotter::GetHistFromSample] no "+histname+" "+sample.title+", use "+p.histname);
      hist=GetHistFromSample(sample,pp+additional_option-"replace"-"varibit");
    }
  }
  if(hist){
    sample.ApplyStyle(hist);
  }
  _depth--;
  return hist;
}
TH1* Plotter::GetHistFromFile(TString filename,TString histname){
  TH1* hist=NULL;
  if(histname.MaybeRegexp()){
    for(const auto& key:GetHistKeys(filename,"^"+histname+"$")){
      TH1* this_hist=GetHistFromFile(filename,key);
      if(this_hist){
	if(hist){
	  hist->Add(this_hist);
	  delete this_hist;
	}else hist=this_hist;
      }
    }
  }else{
    TFile *f=TFile::Open(filename);
    if(f->IsZombie()) PError("It is a zombie file");
    if(!f->IsOpen()) PError("Cannot open the file");
    hist=(TH1*)f->Get(histname);
    if(hist){
      if(hist->InheritsFrom("TH1")){
	hist->SetDirectory(pdir);
	if(!hist->GetSumw2N()) hist->Sumw2();
	PInfo("[Plotter::GetHistFromFile] get "+histname+" from "+filename);
      }else hist=NULL;
    }
    f->Close();
    delete f;
  }
  if(!hist)
    PDebug("[Plotter::GetHistFromFile] no "+histname+" in "+filename);
  return hist;
}
void Plotter::GetHistActionForAdditionalClass(TObject*& obj,Plot p){
  PError((TString)"[Plotter::GetHistActionForAdditionalClass(TObject*& obj,Plot p)] Unsupported class "+obj->ClassName());
}
vector<TH1*> Plotter::GetHistSys(const Sample& sample,const Plot& p){
  PInfo("[Plotter::GetHistSys] "+sample.title+" "+p.name+" "+p.histname+" "+p.sysname);_depth++;

  vector<TH1*> hists;
  hists.push_back(GetHist(sample,p));

  if(p.sysname!=""&&hists.at(0)){
    if(systematics.find(p.sysname)!=systematics.end()){
      const Systematic& systematic=systematics[p.sysname];
      if(systematic.type==Systematic::Type::MULTI){
	vector<TH1*> syss;
	for(const auto& this_key:systematic.keys){
	  if(systematics.find(this_key)==systematics.end()){
	    PError("[Plotter::GetHistSys] no systematic with key "+this_key);
	  }else{
	    vector<TH1*> this_histsys=GetHistSys(sample,p+("sysname:"+this_key)-"sysdetail"+"notaddsys");
	    if(this_histsys.size()==2){
	      this_histsys.at(1)->SetName(this_key);
	      this_histsys.at(1)->SetTitle(systematics[this_key].title);
	      syss.push_back(this_histsys.at(1));
	    }
	    else PError("[Plotter::GetHistSys] this_histsys.size() != 2");
	  }
	}
	if(syss.size()>0){
	  if(p.option.Contains("sysdetail")){
	    for(int i=0;i<(int)syss.size();i++){
	      if(i==0) AddError(syss.at(i),hists.at(0));
	      else AddError(syss.at(i),syss.at(i-1));
	      hists.push_back(syss.at(i));
	    }
	  }else{
	    TH1* totalsys=syss.at(0);
	    for(int i=1;i<(int)syss.size();i++){
	      AddError(totalsys,syss.at(i));
	      delete syss.at(i);
	    }
	    totalsys->SetName(p.sysname);
	    totalsys->SetTitle(systematic.title);
	    if(!p.option.Contains("notaddsys")) AddError(totalsys,hists.at(0));
	    hists.push_back(totalsys);
	  }
	}
      }else{
	PDebug("[Plotter::GetHistSys] sysname='"+p.sysname+"' systype='"+systematic.GetTypeString()+"'");
	vector<TH1*> variations;
	for(const auto& replace:systematic.replaces){
	  TH1* this_hist=GetHist(sample,p,Form("replace:%s->%s varibit:%d",replace.first.Data(),replace.second.Data(),systematic.varibit));
	  this_hist=GetTH1(this_hist,true);
	  if(this_hist){
	    this_hist->SetName(this_hist->GetName()+replace.second);
	    this_hist->SetTitle(this_hist->GetTitle()+replace.second);
	  }
	  variations.push_back(this_hist);
	}
	PDebug("[Plotter::GetHistSys] '"+p.sysname+"': "+variations.size()+" variations");
	if(p.option.Contains("sysdetail")){
	  for(auto h:variations){
	    if(h){
	      h->SetOption("hist");
	      h->SetFillColor(0);
	    }
	    hists.push_back(h);
	  }
	}else{
	  TH1* histsys=NULL;
	  if(systematic.type==Systematic::Type::ENVELOPE){
	    histsys=GetEnvelope(hists.at(0),variations);
	  }else if(systematic.type==Systematic::Type::GAUSSIAN){
	    histsys=GetRMSError(hists.at(0),variations);
	  }else if(systematic.type==Systematic::Type::HESSIAN){
	    histsys=GetHessianError(hists.at(0),variations);
	  }else{
	    PError((TString)"[Plotter::GetHistSys] Wrong Systematic::Type "+Form("%d",systematic.type));
	  }
	  for(unsigned int j=0;j<variations.size();j++){
	    delete variations.at(j);
	  }
	  if(histsys){
	    sample.ApplyStyle(histsys,true);
	    if(!p.option.Contains("notaddsys")) AddError(histsys,hists.at(0));
	    hists.push_back(histsys);
	  }
	}
      }
    }
    //sample.ApplyStyle(total,true);
  }
  _depth--;
  return hists;
}
vector<vector<TH1*>> Plotter::GetHists(Plot& p){
  vector<vector<TH1*>> hists;
  for(const auto& entry:entries) hists.push_back(GetHistSys(entry,p));
  if(p.option.Contains("norm")&&hists.size()>1){
    TH1* h=GetTH1(hists.at(0).at(0));
    if(h){
      TString integraloption="";
      if(p.option.Contains("widthweight")) integraloption="width";
      double val=h->Integral(integraloption);
      delete h;
      vector<vector<double>> scales=Normalize(hists,val,integraloption);
      if(scales.size()>1){
	TString newytitle="'"+p.ytitle+Form(" (Norm. SF=%.3f",scales.at(1).at(0));
	for(unsigned int i=2;i<scales.size();i++) newytitle+=Form(",%.3f",scales.at(i).at(0));
	newytitle+=")'";
	p.SetOption("1:ytitle:"+newytitle);
      }
    }
  }
  return hists;
}  


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Uncertainty //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TH1* Plotter::GetEnvelope(TH1* central,const vector<TH1*>& variations){
  PAll("[Plotter::GetEnvelope]");
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
TH1* Plotter::GetEnvelope(TH1* central,TH1* variation1,TH1* variation2,TH1* variation3,TH1* variation4,TH1* variation5,TH1* variation6,TH1* variation7,TH1* variation8,TH1* variation9){
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
  PAll("[Plotter::GetHessianError]");
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
  PAll("[Plotter::GetRMSError]");
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
  PAll("[Plotter::AddError]");
  if(!hist){
    PError("[Plotter::AddError] null hist");
    return -1;
  }
  if(!sys){
    PError("[Plotter::AddError] null sys");
    return -1;
  }
  if(sys->InheritsFrom("THStack")){
    sys=GetTH1(sys);
    int ret=AddError(hist,sys);
    delete sys;
    return ret;
  }
  for(int i=1;i<hist->GetNbinsX()+1;i++){
    if(fabs(hist->GetBinContent(i)-sys->GetBinContent(i))*1000000>fabs(hist->GetBinContent(i))){
      PError("[Plotter::AddError] systematic hist is wrong");
      std::cout.precision(20);
      std::cout<<i<<" "<<hist->GetBinContent(i)<<" "<<sys->GetBinContent(i)<<" "<<fabs(hist->GetBinContent(i)-sys->GetBinContent(i))<<endl;
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
void Plotter::DrawHistograms(Plot p){
  for(const vector<TH1*>& hists:p.hists){
    if(hists.size()&&hists.at(0)){
      if(hists.at(0)->InheritsFrom("THStack")){
	if(GetAxisParent()){
	  hists.at(0)->Draw("same");
	  TH1* hist_sum=GetTH1(hists.at(0));
	  hist_sum->Draw(TString("same ")+hists.at(0)->GetOption());
	}else{
	  TH1* hist_sum=GetTH1(hists.at(0));
	  hist_sum->Draw(hists.at(0)->GetOption());
	  if(((THStack*)hists.at(0))->GetHists()->GetSize()){
	    TH1* first=(TH1*)((THStack*)hists.at(0))->GetHists()->At(0);
	    hist_sum->GetXaxis()->SetRange(first->GetXaxis()->GetFirst(),first->GetXaxis()->GetLast());
	  }
	  hists.at(0)->Draw("same");
	}
	for(int i=hists.size()-1;i>0;i--){
	  hists.at(i)->Draw(TString("same ")+hists.at(i)->GetOption());
	}
      }
    }
  }

  for(const vector<TH1*>& hists:p.hists){
    if(hists.size()&&hists.at(0)){
      if(!hists.at(0)->InheritsFrom("THStack")&&hists.at(0)->GetFillColor()>0){
	for(int i=hists.size()-1;i>=0;i--){
	  if(GetAxisParent()) hists.at(i)->Draw(TString("same ")+hists.at(i)->GetOption());
	  else{
	    hists.at(i)->Draw(hists.at(i)->GetOption());
	    hists.at(i)->SetStats(0);
	  }
	}
      }
    }
  }
  
  for(const vector<TH1*>& hists:p.hists){
    if(hists.size()&&hists.at(0)){
      if(!hists.at(0)->InheritsFrom("THStack")&&hists.at(0)->GetFillColor()==0){
	for(int i=hists.size()-1;i>=0;i--){
	  if(GetAxisParent()) hists.at(i)->Draw(TString("same ")+hists.at(i)->GetOption());
	  else{
	    hists.at(i)->Draw(hists.at(i)->GetOption());
	    hists.at(i)->SetStats(0);
	  }
	}
      }
    }
  }
}

void Plotter::DrawCompare(Plot p){
  PInfo("[Plotter::DrawCompare] "+p.option);_depth++;
  if(!gPad) gROOT->MakeDefCanvas();

  DrawHistograms(p);
  TH1* axisowner=GetAxisParent();
  axisowner->SetTickLength(0.015,"Y");
  double scale=1/TMath::Min(gPad->GetHNDC(),gPad->GetWNDC());
  axisowner->SetTitleSize(0.04*scale,"XYZ");
  axisowner->SetLabelSize(0.04*scale,"XYZ");
  if(p.option.Contains("xtitle++")) axisowner->SetTitleSize(axisowner->GetTitleSize("x")*1.5,"x");
  else if(p.option.Contains("xtitle+")) axisowner->SetTitleSize(axisowner->GetTitleSize("x")*1.2,"x");
  else if(p.option.Contains("xtitle--")) axisowner->SetTitleSize(axisowner->GetTitleSize("x")*0.6,"x");
  else if(p.option.Contains("xtitle-")) axisowner->SetTitleSize(axisowner->GetTitleSize("x")*0.8,"x");
  if(p.option.Contains("ytitle++")) axisowner->SetTitleSize(axisowner->GetTitleSize("y")*1.5,"y");
  else if(p.option.Contains("ytitle+")) axisowner->SetTitleSize(axisowner->GetTitleSize("y")*1.2,"y");
  else if(p.option.Contains("ytitle--")) axisowner->SetTitleSize(axisowner->GetTitleSize("y")*0.8,"y");
  else if(p.option.Contains("ytitle-")) axisowner->SetTitleSize(axisowner->GetTitleSize("y")*0.6,"y");
  axisowner->GetYaxis()->SetTitleOffset(1.8/scale);

  TLegend* legend=NULL;
  if(!p.option.Contains("noleg")) DrawLegend(p);

  if(p.option.Contains("logx")){
    gPad->SetLogx();
    axisowner->GetXaxis()->SetMoreLogLabels();
    if(axisowner->GetXaxis()->GetBinLowEdge(axisowner->GetXaxis()->GetFirst())==0){
      axisowner->GetXaxis()->SetRange(axisowner->GetXaxis()->GetFirst()+1,axisowner->GetXaxis()->GetLast());
    }
  }
  if(p.option.Contains("logy")){
    pair<double,double> minmax=GetMinMax(p.hists,"pos");
    double minimum=minmax.first,maximum=minmax.second;
    if(minimum<=0) minimum=maximum/1000;
    axisowner->GetYaxis()->SetRangeUser(minimum/20,maximum*20);
    gPad->SetLogy();
  }else{
    pair<double,double> minmax=GetMinMax(p.hists);
    double minimum=minmax.first,maximum=minmax.second;
    double range=fabs(maximum-minimum);
    double ymin=minimum/range<-0.01?minimum-0.1*range:0;
    double ymax=maximum+0.1*(maximum-ymin);
    axisowner->GetYaxis()->SetRangeUser(ymin,ymax);
  }
  if(p.ymin||p.ymax){
    axisowner->GetYaxis()->SetRangeUser(p.ymin,p.ymax);
    cout<<"setting"<<endl;
  }
  axisowner->GetXaxis()->SetTitle(p.xtitle);
  axisowner->GetYaxis()->SetTitle(p.ytitle); 
  gPad->SetFillStyle(0);
  gPad->RedrawAxis();
  gPad->Update();
  if(gPad->GetPrimitive("title")){
    TPaveText* pt=(TPaveText*)gPad->GetPrimitive("title");
    pt->SetTextSize(0.05);
    if(p.option.Contains("title++")) pt->SetTextSize(pt->GetTextSize()*1.4);
    else if(p.option.Contains("title+")) pt->SetTextSize(pt->GetTextSize()*1.2);
  }
  _depth--;
}
vector<vector<TH1*>> Plotter::GetRatioHists(Plot p){
  TH1* base=NULL;
  TString baseopt=p.option("base:[0-9]*");
  if(baseopt!="") base=GetTH1(p.hists[((TString)baseopt(5,100)).Atoi()][0]);
  else if(p.hists.size()>2) base=GetTH1(p.hists[0][0]);
  else base=GetTH1(p.hists[1][0]);
    
  vector<vector<TH1*>> hists_new;
  if(base){
    for(int i=0;i<base->GetNbinsX()+2;i++){
      base->SetBinError(i,0);
    }
    for(const vector<TH1*>& this_hists:p.hists){
      vector<TH1*> this_hists_new;
      for(const auto& hist:this_hists){
	TH1* hist_new=NULL;
	if(hist){
	  hist_new=GetTH1(hist);
	  hist_new->Divide(base);
	}
	this_hists_new.push_back(hist_new);
      }
      hists_new.push_back(this_hists_new);
    }  
    delete base;
  }
  return hists_new;
}  
void Plotter::DrawRatio(Plot p){
  PInfo("[Plotter::DrawRatio] "+p.option);_depth++;
  if(!gPad) gROOT->MakeDefCanvas();

  p.hists=GetRatioHists(p);
  DrawCompare(p-"norm");
  TH1* axisowner=GetAxisParent();
  if(axisowner){
    if(p.ytitle=="") axisowner->GetYaxis()->SetTitle("Ratio");
    if(p.ymin||p.ymax){
      axisowner->GetYaxis()->SetRangeUser(p.ymin,p.ymax);
    }else if(p.option.Contains("widewidey")){
      axisowner->GetYaxis()->SetRangeUser(0.01,1.99);
      axisowner->GetYaxis()->SetNdivisions(506);
    }else if(p.option.Contains("widey")){
      axisowner->GetYaxis()->SetRangeUser(0.501,1.499);
      axisowner->GetYaxis()->SetNdivisions(506);
    }else{
      axisowner->GetYaxis()->SetRangeUser(0.801,1.199);
      axisowner->GetYaxis()->SetNdivisions(504);
    }
  }
  _depth--;
}
void Plotter::DrawDiff(Plot p){
  PInfo("[Plotter::DrawDiff]"+p.option);
  if(!gPad) gROOT->MakeDefCanvas();

  TH1* base=NULL;
  TString baseopt=p.option("base:[0-9]*");
  if(baseopt!="") base=GetTH1(p.hists[((TString)baseopt(5,100)).Atoi()][0]);
  else if(p.hists.size()>2) base=GetTH1(p.hists[0][0]);
  else base=GetTH1(p.hists[1][0]);  
  for(int i=0;i<base->GetNbinsX()+2;i++){
    base->SetBinError(i,0);
  }

  vector<vector<TH1*>> hists_new;
  for(const vector<TH1*>& this_hists:p.hists){
    vector<TH1*> this_hists_new;
    for(const auto& hist:this_hists){
      TH1* hist_new=NULL;
      if(hist){
	hist_new=GetTH1(hist);
	hist_new->Add(base,-1);
      }
      this_hists_new.push_back(hist_new);
    }
    hists_new.push_back(this_hists_new);
  }  
  delete base;

  p.hists=hists_new;
  DrawCompare(p);

  TH1* axisparent=GetAxisParent();
  axisparent->GetYaxis()->SetTitle("Diff");
  axisparent->GetYaxis()->SetLabelSize(0.06);

  pair<double,double> minmax=GetMinMax(p.hists);
  double minimum=minmax.first,maximum=minmax.second;
  double range=fabs(maximum-minimum);
  axisparent->GetYaxis()->SetRangeUser(minimum/range<-0.01?minimum-0.1*range:0,maximum+0.1*range);
  _depth--;
}
void Plotter::DrawSig(Plot p){
  PInfo("[Plotter::DrawSig] "+p.option);_depth++;
  if(!gPad) gROOT->MakeDefCanvas();

  TH1* base=NULL;
  TString baseopt=p.option("base:[0-9]*");
  if(baseopt!="") base=GetTH1(p.hists[((TString)baseopt(5,100)).Atoi()].back());
  else if(p.hists.size()>2) base=GetTH1(p.hists[0].back());
  else base=GetTH1(p.hists[1].back());

  vector<vector<TH1*>> hists_new;
  for(const vector<TH1*>& this_hists:p.hists){
    TH1* hist=NULL;
    for(int i=this_hists.size()-1;i>=0;i--){
      hist=this_hists.at(i);
      if(hist){
	hist=GetTH1(hist);
	for(int i=0;i<hist->GetNbinsX()+2;i++){
	  double error=sqrt(pow(hist->GetBinError(i),2)+pow(base->GetBinError(i),2));
	  double content=(hist->GetBinContent(i)-base->GetBinContent(i))/error;
	  hist->SetBinContent(i,content);
	  hist->SetBinError(i,0);
	}
	break;
      }
    }
    hists_new.push_back({hist});
  }
  p.hists=hists_new;
  DrawCompare(p);
  TH1* axisparent=GetAxisParent();
  
  axisparent->GetYaxis()->SetTitle("Difference (#sigma)");
  axisparent->GetYaxis()->SetLabelSize(0.06);

  if(p.option.Contains("widey")) axisparent->GetYaxis()->SetRangeUser(-4.9,4.9);
  else axisparent->GetYaxis()->SetRangeUser(-2.9,2.9);
  _depth--;
}
void Plotter::DrawCompareAndRatio(Plot p){
  PInfo("[Plotter::DrawCompareAndRatio] "+p.option);_depth++;
  if(!gPad) gROOT->MakeDefCanvas();
  
  TVirtualPad* pad=gPad;
  pad->Divide(1,2);
  pad->cd(1);
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  gPad->SetTopMargin(pad->GetTopMargin()/0.65);
  Plot plot1=p.GetSubPlot(1);
  DrawCompare(plot1);
  TH1* axisparent=GetAxisParent();
  axisparent->GetXaxis()->SetLabelSize(0);
  axisparent->GetXaxis()->SetTitle("");
  if(gPad->GetPrimitive("title")){
    TPaveText* pt=(TPaveText*)gPad->GetPrimitive("title");
    pt->SetTextSize(0.075);
    pt->SetY1NDC(pt->GetY1NDC()-0.02);
    pt->SetY2NDC(pt->GetY2NDC()-0.02);
    if(p.option.Contains("title++")) pt->SetTextSize(pt->GetTextSize()*1.4);
    else if(p.option.Contains("title+")) pt->SetTextSize(pt->GetTextSize()*1.2);
  }
  gPad->Update();
  gPad->Modified();
  
  pad->cd(2);
  gPad->SetPad(0,0,1,0.365);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(pad->GetBottomMargin()/0.35);
  gPad->SetGridx();gPad->SetGridy();
  DrawRatio((p+"noleg").GetSubPlot(2));
  axisparent=GetAxisParent();
  if(axisparent){
    axisparent->SetTitle("");
    //axisparent->SetStats(0);
    //axisparent->GetYaxis()->SetTitle("Ratio");
  }
  gPad->Update();
  gPad->Modified();
  _depth--;
}
void Plotter::DrawCompareAndSig(Plot p){
  PInfo("[Plotter::DrawCompareAndSig] "+p.option);_depth++;
  if(!gPad) gROOT->MakeDefCanvas();
    
  TVirtualPad* pad=gPad;
  pad->Divide(1,2);
  pad->cd(1);
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  gPad->SetTopMargin(pad->GetTopMargin()/0.65);
  DrawCompare(p-"2:");
  if(pad->GetPad(1)->GetPrimitive("title")) ((TPaveText*)pad->GetPad(1)->GetPrimitive("title"))->SetTextSize(0.075);
  TH1* axisparent=GetAxisParent();
  axisparent->GetYaxis()->SetLabelSize(0.06);
  axisparent->GetYaxis()->SetTitleSize(0.06);
  axisparent->GetYaxis()->SetTitleOffset(1.2);
  axisparent->GetXaxis()->SetLabelSize(0);
  axisparent->GetXaxis()->SetTitle("");
  gPad->Update();
  gPad->Modified();

  pad->cd(2);
  DrawSig(p-"1:"+"noleg");
  gPad->SetPad(0,0,1,0.365);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.3);
  gPad->SetGridy();
  axisparent=GetAxisParent();
  if(axisparent){
    axisparent->SetTitle("");
    axisparent->SetStats(0);
    axisparent->GetYaxis()->SetLabelSize(0.1);
    axisparent->GetYaxis()->SetTitleSize(0.12);
    axisparent->GetYaxis()->SetTitleOffset(0.6);
    axisparent->GetXaxis()->SetTitleSize(0.12);
    axisparent->GetXaxis()->SetLabelSize(0.12);
  }
  gPad->Update();
  gPad->Modified();
  _depth--;
}
void Plotter::DrawCompareAndDiff(Plot p){
  PInfo("[Plotter::DrawCompareAndDiff] "+p.option);_depth++;
  if(!gPad) gROOT->MakeDefCanvas();
    
  TVirtualPad* pad=gPad;
  pad->Divide(1,2);
  pad->cd(1);
  DrawCompare(p-"2:");
  if(pad->GetPad(1)->GetPrimitive("title")) ((TPaveText*)pad->GetPad(1)->GetPrimitive("title"))->SetTextSize(0.075);
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  gPad->SetTopMargin(pad->GetTopMargin()/0.65);
  TH1* axisparent=GetAxisParent();
  axisparent->GetYaxis()->SetLabelSize(0.06);
  axisparent->GetYaxis()->SetTitleSize(0.06);
  axisparent->GetYaxis()->SetTitleOffset(1.2);
  axisparent->GetXaxis()->SetLabelSize(0);
  axisparent->GetXaxis()->SetTitle("");
  gPad->Update();
  gPad->Modified();

  pad->cd(2);
  DrawDiff(p-"1:"+"noleg");
  gPad->SetPad(0,0,1,0.365);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.3);
  gPad->SetGridx();gPad->SetGridy();

  axisparent=GetAxisParent();
  if(axisparent){
    axisparent->SetTitle("");
    axisparent->SetStats(0);
    axisparent->GetYaxis()->SetLabelSize(0.1);
    axisparent->GetYaxis()->SetTitleSize(0.12);
    axisparent->GetYaxis()->SetTitleOffset(0.6);
    axisparent->GetYaxis()->SetNdivisions(305);
    axisparent->GetXaxis()->SetTitle(axisparent->GetTitle());
    axisparent->GetXaxis()->SetTitleSize(0.12);
    axisparent->GetXaxis()->SetLabelSize(0.12);
  }
  gPad->Update();
  gPad->Modified();
  _depth--;
}
void Plotter::DrawDoubleRatio(Plot& p){
  if(!gPad) gROOT->MakeDefCanvas();

  vector<vector<vector<TH1*>>> vec_hists;
  for(auto& sub:p.subplots){
    vec_hists.push_back(GetRatioHists(p));
  }
  int baseindex=0;
  if(p.subplots.size()==2){
    baseindex=1;
    TString baseopt=p.option("base:[0-9]*");
    if(baseopt!="") baseindex=((TString)baseopt(5,100)).Atoi();
    for(int i=0;i<vec_hists[baseindex].size();i++){
      TH1* hist=vec_hists[baseindex][i][0];
      bool flag=false;
      for(int j=1;j<hist->GetNbinsX()+1;j++) if(hist->GetBinContent(j)!=1.) flag=true;
      if(!flag){
	for(int j=0;j<vec_hists.size();j++){
	  vec_hists[j].erase(vec_hists[j].begin()+i);
	}
      }
    }
    vector<vector<TH1*>> hists_new;
    for(int i=0;i<vec_hists[baseindex].size();i++){
      TH1* base=(TH1*)vec_hists[baseindex][i][0]->Clone();
      for(int j=0;j<base->GetNbinsX()+2;j++) base->SetBinError(j,0);
      for(int j=0;j<vec_hists.size();j++){
	vector<TH1*> this_hists_new;
	for(int k=0;k<vec_hists[j][i].size();k++){
	  if(vec_hists[j][i][k]) vec_hists[j][i][k]->Divide(base);
	  this_hists_new.push_back(vec_hists[j][i][k]);
	}
	hists_new.push_back(this_hists_new);
      }
      delete base;
    }
    p.hists=hists_new;
    DrawCompare(p-"norm");
    TH1* axisparent=GetAxisParent();
    axisparent->SetTitle(p.title);
    gPad->SetGridy();
    axisparent->GetYaxis()->SetTitle("DoubleRatio");
    if(p.option.Contains("widewidey")){
      axisparent->GetYaxis()->SetRangeUser(0.01,1.99);
      axisparent->GetYaxis()->SetNdivisions(506);
    }else if(p.option.Contains("widey")){
      axisparent->GetYaxis()->SetRangeUser(0.501,1.499);
      axisparent->GetYaxis()->SetNdivisions(506);
    }else{
      axisparent->GetYaxis()->SetRangeUser(0.801,1.199);
      axisparent->GetYaxis()->SetNdivisions(504);
    }
  }else{
    PError("not supported");
    exit(1);
  }
}

TCanvas* Plotter::DrawPlot(Plot p,TString additional_option){
  PInfo("[Plotter::DrawPlot] "+p.name+" "+p.histname+" "+p.option+" "+additional_option);_depth++;
  if(!pdir) pdir=new TDirectory("plotdir","plotdir");
  p.SetOption(additional_option);
  TCanvas* c=gROOT->MakeDefCanvas();
  p.hists=GetHists(p);
  vector<TH1*> hists=VectorTH1(p.hists);
  if(hists.size()<entries.size()){
    PError("[Plotter::DrawPlot] #hists < #entries ");
    _depth--;
    return NULL;
  }
  if(p.type==Plot::Type::Compare) DrawCompare(p);
  else if(p.type==Plot::Type::Ratio) DrawRatio(p);
  else if(p.type==Plot::Type::Diff) DrawDiff(p);
  else if(p.type==Plot::Type::Sig) DrawSig(p);
  else if(p.type==Plot::Type::CompareAndRatio) DrawCompareAndRatio(p);
  else if(p.type==Plot::Type::CompareAndDiff) DrawCompareAndDiff(p);
  else if(p.type==Plot::Type::CompareAndSig) DrawCompareAndSig(p);
  else if(p.type==Plot::Type::DoubleRatio) DrawDoubleRatio(p);
  if(p.option.Contains("preliminary")){
    c->cd(1);
    TH1* axisparent=GetAxisParent();
    if(axisparent) axisparent->SetTitle("");
    c->cd();
    axisparent=GetAxisParent();
    if(axisparent) axisparent->SetTitle("");
    TLatex latex;
    latex.SetTextSize(0.035);
    latex.SetNDC();
    latex.DrawLatex(0.16,0.91,"CMS #bf{#it{Preliminary}}");
    if(p.histname.Contains("2016a/")||p.era=="2016preVFP"){
      latex.DrawLatex(0.69,0.91,"19.5 fb^{-1} (13 TeV)");
    }else if(p.histname.Contains("2016b/")||p.era=="2016postVFP"){
      latex.DrawLatex(0.69,0.91,"16.8 fb^{-1} (13 TeV)");
    }else if(p.histname.Contains("2016[ab]/")||p.era=="2016"){
      latex.DrawLatex(0.69,0.91,"36.3 fb^{-1} (13 TeV)");
    }else if(p.histname.Contains("2017/")||p.era=="2017"){
      latex.DrawLatex(0.69,0.91,"41.5 fb^{-1} (13 TeV)");
    }else if(p.histname.Contains("2018/")||p.era=="2018"){
      latex.DrawLatex(0.69,0.91,"59.8 fb^{-1} (13 TeV)");
    }else if(p.histname.Contains("201[6-8]/")||p.histname.Contains("201[678ab]+/")||p.histname.Contains("201[6-8][ab]?/")||p.histname.Contains("201[678][ab]?/")||p.era=="Run2"){
      latex.DrawLatex(0.69,0.91,"137 fb^{-1} (13 TeV)");      
    }
    latex.SetTextSize(0.035);
    latex.SetTextColor(2);
    latex.DrawLatex(0.38,0.91,"#it{Working in progress}");
  }
  c->Update();
  c->Modified();
  _depth--;
  return c;
}
TCanvas* Plotter::DrawPlot(TString plotkey,TString additional_option){
  return DrawPlot(MakePlot(plotkey,additional_option));
}
vector<TCanvas*> Plotter::DrawPlots(TRegexp plotkey,TString additional_option){
  vector<TCanvas*> canvases;
  for(const auto& [key,plot]:plots)
    if(key.Contains(plotkey)) canvases.push_back(DrawPlot(key,additional_option));
  return canvases;
}
void Plotter::SavePlot(TString plotkey,TString option,bool delete_canvas){
  PDebug("[Plotter::SavePlot(TString plotkey)]");
  TString format="png";
  if(option.Contains("pdf")) format="pdf";
  TCanvas* c=DrawPlot(plotkey,option);
  SaveCanvas(c,plotkey+"."+format,delete_canvas);
}
void Plotter::SaveCanvas(TCanvas *c,TString path,bool delete_canvas){
  if(c){
    gSystem->Exec("mkdir -p "+plotdir+"/"+Dirname(path));
    c->SaveAs(plotdir+"/"+path);
    if(delete_canvas){
      delete c;
      if(pdir) pdir->Delete();
    }
  }  
}
void Plotter::SavePlots(TRegexp plotkey){
  for(const auto& [key,plot]:plots)
    if(key.Contains(plotkey)) SavePlot(key);
}  
void Plotter::SavePlotAll(){
  PAll("[Plotter::SavePlotAll(TString plotkey)]");
  for(const auto& [key,plot]:plots) SavePlot(key);
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Canvas Tools /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TString Plotter::GetHistNameWithPrefixAndSuffix(TString histname,TString prefix,TString suffix){
  PError("[Plotter::GetHistNameWithPrefixAndSuffix] "+histname+" "+prefix+" "+suffix);
  TString this_histname=histname(0,histname.Last('/')+1)+prefix+histname(histname.Last('/')+1,histname.Length())+suffix;
  return this_histname;
}
TH1* Plotter::GetAxisParent(TVirtualPad* pad){
  PAll("[Plotter::GetAxisParent(TVirtualPad* pad)]");
  if(!pad) pad=gPad;
  if(pad){
    TList* list=pad->GetListOfPrimitives();
    for(int i=0;i<list->GetSize();i++){
      if(list->At(i)->InheritsFrom("THStack")) return ((THStack*)list->At(i))->GetHistogram();
      if(list->At(i)->InheritsFrom("TH1")) return (TH1*)list->At(i);
      if(list->At(i)->InheritsFrom("TGraph")) return ((TGraph*)list->At(i))->GetHistogram();
    }
  }
  return NULL;
}
TLegend* Plotter::GetLegend(TVirtualPad* pad){
  PAll("[Plotter::GetLegend(TVirtualPad* pad)]");
  if(!pad) pad=gPad;
  if(pad){
    TList* list=pad->GetListOfPrimitives();
    for(int i=0;i<list->GetSize();i++){
      if(list->At(i)->InheritsFrom("TLegend")) return (TLegend*)list->At(i);
    }
  }
  return NULL;
}
void Plotter::SetLegendEntryLabel(TLegend* legend,int i,TString label){
  if(!legend){
    cout<<"[Plotter::SetLegendEntryLabel] null legend"<<endl;
    return;
  }
  TList* list=legend->GetListOfPrimitives();
  if(i>=list->GetSize()){
    cout<<"[Plotter::SetLegendEntryLabel] i="+TString(i)+" entries="+TString(list->GetSize())<<endl;
    return;
  }   
  TLegendEntry* entry=(TLegendEntry*)list->At(i);
  entry->SetLabel(label);
}
TH1* Plotter::GetTH1(TH1* hstack,bool deleteorigin) const{
  PAll("[Plotter::GetTH1(TH1* hstack)]");
  TH1* hist=NULL;
  if(hstack){
    if(strstr(hstack->ClassName(),"THStack")!=NULL){
      hist=(TH1*)((THStack*)hstack)->GetHists()->Last()->Clone();
      hist->SetDirectory(pdir);
      hist->SetStats(0);
      hist->Reset();
      for(const auto& entry:entries){
	if(entry.title==hstack->GetName()){
	  entry.ApplyStyle(hist);
	  hist->SetTitle(hstack->GetTitle());
	  break;
	}
      }
      for(const auto& this_hist:*((THStack*)hstack)->GetHists()){
	hist->Add((TH1*)this_hist);
      }
    }else{
      hist=(TH1*)hstack->Clone();
      hist->SetDirectory(pdir);
    }
    if(deleteorigin) delete hstack;
  }
  return hist;
}
bool Plotter::CheckHists(vector<TH1*> hists){
  PAll("[Plotter::CheckHists(vector<TH1*> hists)]");
  bool flag_data=false,flag_signal=false;
  for(unsigned int i=0;i<entries.size();i++){
    if(!hists.at(i)) continue;
    if(entries[i].type==Sample::Type::DATA) flag_data=true;
    if(entries[i].type==Sample::Type::SIGNAL) flag_signal=true;
  }
  if(flag_data&&flag_signal) return true;
  else return false;
}
vector<TH1*> Plotter::VectorTH1(const vector<vector<TH1*>>& hists){
  PAll("[Plotter::VectorTH1(const vector<vector<TH1*>>& hists)]");
  vector<TH1*> hists_out;
  for(const vector<TH1*>& this_hists:hists){
    for(const auto& hist:this_hists){
      if(hist) hists_out.push_back(hist);
    }
  }
  return hists_out;
}
pair<double,double> Plotter::GetMinMax(const vector<TH1*>& hists,TString option) const{
  PAll("[Plotter::GetMinMax(const vector<TH1*>& hists)]");
  double maximum=-999999999;
  double minimum=999999999;
  for(auto hist:hists){
    if(hist){
      if(hist->InheritsFrom("THStack")) hist=GetTH1(hist);
      double this_maximum=hist->GetBinContent(hist->GetMaximumBin());
      double this_minimum=hist->GetBinContent(hist->GetMinimumBin());
      if(!option.Contains("pos")){
	if(maximum<this_maximum) maximum=this_maximum;
	if(minimum>this_minimum) minimum=this_minimum;
      }else{
	if(maximum<this_maximum && this_maximum>0) maximum=this_maximum;
	if(minimum>this_minimum && this_minimum>0) minimum=this_minimum;
      }	
    }
  }
  return make_pair(minimum,maximum);
}
pair<double,double> Plotter::GetMinMax(const vector<vector<TH1*>>& hists,TString option) const{
  return GetMinMax(VectorTH1(hists),option);
}
void Plotter::RebinXminXmax(TH1* hist,Plot p){
  PAll("[Plotter::RebinXminXmax]");
  if(hist){
    if(hist->InheritsFrom("THStack")){
      for(const auto& obj:*((THStack*)hist)->GetHists())
	RebinXminXmax((TH1*)obj,p);
    }else if(hist->InheritsFrom("TH4D")){
      if(p.rebin!=""){
	PError((TString)"[Plotter::RebinXminXmax] Unsupported class for rebin "+hist->ClassName());
      }
      if(p.xmin!=0||p.xmax!=0)
	hist->GetXaxis()->SetRangeUser(p.xmin,p.xmax);
    }else if(hist->InheritsFrom("TH3")){
      if(p.rebin!=""){
	PError((TString)"[Plotter::RebinXminXmax] Unsupported class for rebin "+hist->ClassName());
      }
      if(p.xmin!=0||p.xmax!=0)
	hist->GetXaxis()->SetRangeUser(p.xmin,p.xmax);
    }else if(strstr(hist->ClassName(),"TH1")){
      if(p.rebin!=""){
	if(p.rebin.IsDec()){
	  double this_xmin=hist->GetBinLowEdge(hist->GetXaxis()->GetFirst());
	  double this_xmax=hist->GetBinLowEdge(hist->GetXaxis()->GetLast()+1);
	  hist->Rebin(p.rebin.Atoi());
	  hist->GetXaxis()->SetRangeUser(this_xmin,this_xmax);
	}else{
	  vector<double> bins=VectorDouble(p.rebin);
	  TH1* newhist=hist->Rebin(bins.size()-1,hist->GetName(),&bins[0]);
	  newhist->Copy(*hist);
	}
      }
      if(p.xmin==0&&p.xmax==0){
	if(hist->GetXaxis()->GetFirst()==1&&hist->GetXaxis()->GetLast()==hist->GetNbinsX()){
	  int start=0,end=0;
	  for(int i=0;i<hist->GetNbinsX()+2;i++){
	    if(hist->GetBinContent(i)&&hist->GetBinError(i)){
	      start=i;
	      break;
	    }
	  }
	  for(int i=hist->GetNbinsX()+1;i>=0;i--){
	    if(hist->GetBinContent(i)&&hist->GetBinError(i)){
	      end=i;
	      break;
	    }
	  }
	  if(start<end) hist->GetXaxis()->SetRange(start,end);
	}
      }else hist->GetXaxis()->SetRangeUser(p.xmin,p.xmax);
    }else{
      PError((TString)"[Plotter::RebinXminXmax] Unsupported class "+hist->ClassName());
    }
  }
}

double Plotter::Normalize(TH1* hist,double val,TString option){
  PAll("[Plotter::Normalize(TH1* hist,double val=1.,TString option="")]");
  double scale=1.;
  if(hist){
    if(strstr(hist->ClassName(),"THStack")){
      TH1* hsim=GetTH1(hist);
      scale=val/hsim->Integral(option);
      for(auto obj:*((THStack*)hist)->GetHists()){
	TH1* hsim_sub=(TH1*)obj;
	hsim_sub->Scale(scale);
      }
    }else{
      scale=val/hist->Integral(option);
      hist->Scale(scale);
    }
  }
  //cout<<"[Plotter::Normalize] "<<hist->GetName()<<" "<<scale<<endl;
  return scale;
}
vector<double> Plotter::Normalize(vector<TH1*> hists,double val,TString option){
  vector<double> scales;
  for(auto& hist:hists) scales.push_back(Normalize(hist,val,option));
  return scales;
}
vector<vector<double>> Plotter::Normalize(vector<vector<TH1*>> histss,double val,TString option){
  vector<vector<double>> scales;
  for(auto& hists:histss)
    scales.push_back(Normalize(hists,val,option));
  return scales;
}
void Plotter::WidthWeight(TH1* hist){
  PAll("[Plotter::WidthWeight(TH1* hist)]");
  if(hist){
    if(strstr(hist->ClassName(),"THStack")){
      TH1* hsim=GetTH1(hist);
      for(auto obj:*((THStack*)hist)->GetHists()){
	TH1* hsim_sub=(TH1*)obj;
	WidthWeight(hsim_sub);
      }
    }else{
      for(int i=1;i<hist->GetNbinsX()+1;i++){
	double width=hist->GetBinWidth(i);
	hist->SetBinContent(i,hist->GetBinContent(i)/width);
	hist->SetBinError(i,hist->GetBinError(i)/width);
      }
    }
  }
}
void Plotter::WidthWeight(vector<TH1*> hists){
  for(auto& hist:hists) WidthWeight(hist);
}
bool Plotter::IsEntry(const Sample& sample){
  PAll("[Plotter::IsEntry(const Sample& sample)]");
  for(const auto& entry:entries){
    if(&entry==&sample) return true;
  }
  return false;
}

TLegend* Plotter::DrawLegend(const Plot& p){
  PAll("[Plotter::DrawLegend(const Plot& p)]");
  vector<TH1*> hists;
  for(const auto& this_hists:p.hists){
    TH1* hist=this_hists[0];
    if(hist){
      if(strstr(hist->ClassName(),"THStack")){
	TIter next(((THStack*)hist)->GetHists(),kIterBackward);
	while(TObject *obj = next()){
	  hists.push_back((TH1*)obj);
	}
      }else hists.push_back(hist);
    }
  }
  TVirtualPad* pad=gPad;
  TCanvas* c=pad->GetCanvas();
  double maxwidth=0;
  double maxheight=0;
  c->cd();
  for(const auto& hist:hists){
    TLatex latex(-1,-1,hist->GetName());
    latex.SetTextSize(0.04);
    UInt_t w,h;
    latex.GetBoundingBox(w,h);
    if(1.0*w/c->GetWw()>maxwidth) maxwidth=1.0*w/c->GetWw();
    if(1.0*h/c->GetWh()>maxheight) maxheight=1.0*h/c->GetWh();
  }

  if(maxwidth<0.1) maxwidth=0.1;
  if(maxwidth>0.5) maxwidth=0.5;
  int entry_size=hists.size();
  if(entry_size<3) entry_size=3;
  if(entry_size>7) entry_size=7;
  
  vector<double> coordinates;
  double width=(maxwidth+0.03)/pad->GetWNDC();
  double height=(maxheight*entry_size)/pad->GetHNDC();
  if(p.option.Contains("topleftleg")||p.option.Contains("TLleg")) coordinates={pad->GetLeftMargin()+0.01,1-pad->GetTopMargin()-0.01,pad->GetLeftMargin()+0.01+width,1-pad->GetTopMargin()-0.01-height};
  else if(p.option.Contains("topmiddleleg")||p.option.Contains("TMleg")) coordinates={0.5-0.5*width,1-pad->GetTopMargin()-0.01,0.5+0.5*width,1-pad->GetTopMargin()-0.01-height};
  else if(p.option.Contains("bottomleftleg")||p.option.Contains("BLleg")) coordinates={pad->GetLeftMargin()+0.01,pad->GetBottomMargin()+0.01,pad->GetLeftMargin()+0.01+width,pad->GetBottomMargin()+0.01+height};
  else if(p.option.Contains("bottommiddleleg")||p.option.Contains("BMleg")) coordinates={0.5-0.5*width,pad->GetBottomMargin()+0.01,0.5+0.5*width,pad->GetBottomMargin()+0.01+height};
  else if(p.option.Contains("bottomrightleg")||p.option.Contains("BRleg")) coordinates={1-pad->GetRightMargin()-0.01,pad->GetBottomMargin()+0.01,1-pad->GetRightMargin()-width,pad->GetBottomMargin()+0.01+height};
  else coordinates={1-pad->GetRightMargin()-0.01,1-pad->GetTopMargin()-0.01,1-pad->GetRightMargin()-0.01-width,1-pad->GetTopMargin()-0.01-height};

  TLegend* legend=new TLegend(coordinates[0],coordinates[1],coordinates[2],coordinates[3]);
  for(const auto& hist:hists) legend->AddEntry(hist,hist->GetName());
  legend->SetBorderSize(0);
  pad->cd();
  legend->Draw();
  return legend;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Plot /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
set<TString> Plotter::GetHistKeys(TDirectory* dir,TRegexp regexp){
  PAll("[Plotter::GetHistKeys(TDirectory* dir,TRegexp regexp=\".*\")]");
  set<TString> histkeys;
  for(const auto& obj:*(dir->GetListOfKeys())){
    TKey* key=(TKey*)obj;
    if(strcmp(key->GetClassName(),"TDirectoryFile")==0){
      set<TString> this_histkeys=GetHistKeys((TDirectory*)key->ReadObj(),regexp);
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
set<TString> Plotter::GetHistKeys(TString filename,TString regexp){
  PAll("[Plotter::GetHistKeys(TString filename=\""+filename+",TString regexp=\""+regexp+"\")]");
  if(histkeys_cache.find(filename)==histkeys_cache.end()){
    TFile *f=new TFile(filename);
    histkeys_cache[filename]=GetHistKeys((TDirectory*)f,".*");
    f->Close();delete f;
  }
  if(regexp==".*") return histkeys_cache[filename];
  else{
    set<TString> histkeys;
    for(const TString& key:histkeys_cache[filename])
      if(key.Contains(TRegexp(regexp))) histkeys.insert(key);
    return histkeys;
  }
}
set<TString> Plotter::GetHistKeys(const Sample& sample,TString regexp){
  PAll("[Plotter::GetHistKeys]");
  set<TString> histkeys;
  if(sample.type==Sample::Type::FILE){
    set<TString> this_histkeys=GetHistKeys(sample.title,regexp);
    histkeys.insert(this_histkeys.begin(),this_histkeys.end());
  }else if(sample.subs.size()){
    for(const auto& sub:sample.subs){
      set<TString> this_histkeys=GetHistKeys(sub,regexp);
      histkeys.insert(this_histkeys.begin(),this_histkeys.end());
    }
  }
  return histkeys;
}
Plot Plotter::MakePlot(TString name,TString option){
  Plot p;
  p.name=name;
  p.title=name;
  p.histname=name;
  if(entries.size()==1) p.type=Plot::Type::Compare;
  p.SetOption(option);
  /*
  if(p.type==Plot::Type::DoubleRatio){
    for(auto const& [k,p]:plots){
      if(k.Contains(TRegexp(plotkey+"$"))) p.subplots.push_back(p);
    }
    if(p.subplots.size()!=2){
      PError(Form("[Plotter::MakePlot] DoubleRatio should have 2 subplots (%zu!=2)",p.subplots.size()));
      p.Print();
      exit(1);
    }
  }
  */
  return p;
} 

///////////////////////////////////////////////////////////////////////
///////////////////// Systeamtic /////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void Plotter::AddSystematic(TString key,TString title,Systematic::Type type,int varibit,vector<TString> includes){
  auto it=systematics.find(key);
  if(it==systematics.end()) systematics[key]=Systematic(title,type,varibit,includes);
  else PError("systematic "+key+" already exists");
}
void Plotter::AddSystematic(TString key,TString title,Systematic::Type type,int varibit,TString includes){
  return AddSystematic(key,title,type,varibit,Split(includes," "));
}


///////////////////////////////////////////////////////////////////////
///////////////////// etc /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int Plotter::GetAutoColor() const {
  set<int> colors={1,2,3,4,5,6,7,8,9};
  for(const auto& color:colors){
    bool exist=false;
    for(const auto& entry:entries){
      if(entry.style.linecolor==color){
	exist=true;
	break;
      }
    }
    if(!exist){
      return color;
    }
  }
  return 0;
}
double Plotter::GetChi2(TH1* h1,TH1* h2) const {
  double chi2=0;
  for(int i=h1->GetXaxis()->GetFirst();i<h1->GetXaxis()->GetLast()+1;i++){
    double x1=h1->GetBinContent(i);
    double ex1=h1->GetBinError(i);
    double x2=h2?h2->GetBinContent(i):0.;
    double ex2=h2?h2->GetBinError(i):0.;
    chi2+=pow(x1-x2,2)/(pow(ex1,2)+pow(ex2,2));
  }
  chi2/=h1->GetXaxis()->GetLast()-h1->GetXaxis()->GetFirst()+1;
  return chi2;
}
vector<int> Plotter::Range(int n){
  return Range(0,n,1);
}
vector<int> Plotter::Range(int s,int n,int h){
  vector<int> out;
  for(int i=s;i<n;i+=h) out.push_back(i);
  return out;
}
vector<TString> Plotter::FormRange(TString form,vector<int> range){
  vector<TString> out;
  for(auto d:range) out.push_back(Form(form,d));
  return out;
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
    if(DEBUG>1) std::cout<<"###INFO#### [SavePlots] save "<<outputdir+"/"+histname+".png"<<endl;
    c=GetCompare(histname,0,p->rebin,p->xmin,p->xmax,p->option);
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
      if(DEBUG>1) std::cout<<"###INFO#### [SavePlots] save "<<this_dir+"/"+histname(histname.Last('/'),histname.Length())+".png"<<endl;
      c=GetCompare(histname,systematics[is].sysbit,p->rebin,p->xmin,p->xmax,p->option);
      if(c){
	c->SaveAs(this_dir+"/"+histname(histname.Last('/'),histname.Length())+".png");
	THStack* hstack=(THStack*)c->GetPad(1)->GetPrimitive("hstack");
	if(hstack) hstack->GetHists()->Clear();
	delete c;
      }
      if(systematics[is].suffixes.size()==1){
	if(DEBUG>1) std::cout<<"###INFO#### [SavePlots] save "<<this_dir+"/"+histname(histname.Last('/'),histname.Length())+"_raw.png"<<endl;
	c=GetCompare(histname+(systematics[is].vary_data?systematics[is].suffixes[0]:""),histname+(systematics[is].vary_signal?systematics[is].suffixes[0]:""),histname+(systematics[is].vary_bg?systematics[is].suffixes[0]:""),0,p->rebin,p->xmin,p->xmax,p->option);
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
