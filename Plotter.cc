#ifndef __PLOTTER_CC__
#define __PLOTTER_CC__
#include <set>
#include <TStyle.h>
#include <TGaxis.h>
#include <TH3.h>
#include <TROOT.h>
#include <TLatex.h>
#include <TGraph.h>
#include <TMath.h>
#include"Global.h"
#include"Sample.cc"
#include"Hists.cc"
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
  TString mode;
  TString scandir;
  TString plotfile;
  TString plotdir="fig";

  //Setup
  void AddFile(TString key,TString file);
  void ScanFiles(TString path);
  virtual int Setup(TString mode_,TString scandir);
  virtual TString GetSetupStringForCondor();
  virtual void SetupEntries(TString mode);
  static void SetupStyle();
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
  vector<double> GetAxisVector(const TAxis* axis) const;
  virtual TH1* GetHist(TString samplekey,TString plotkey,TString additional_option="");
  virtual TH1* GetHist(int ientry,TString plotkey,TString additional_option="");
  virtual TH1* GetHist(const Sample& sample,Plot p,TString additional_option="");
  virtual TH1* GetHistFromSample(const Sample& sample,const Plot& p,TString additional_option="");
  virtual TH1* GetHistFromFile(TString filename,TString histname);
  virtual void GetHistActionForAdditionalClass(TObject*& obj,Plot p);
  Hists GetHistVariations(TString samplekey,TString plotkey,TString additional_option="");
  Hists GetHistVariations(int ientry,TString plotkey,TString additional_option="");
  Hists GetHistVariations(const Sample& sample,const Plot& plot);
  vector<Hists> GetHists(Plot& plot);
  void Blind(Plot& p);

  //Systematic
  static TString GetTagFromVariation(TString variation);
  set<TString> GetSystematicVariations(TString sysname);
  static TString Variation2Suffix(TString variation);
  map<TString,TString> GetSystematicSuffixes(TString sysname);
  TH1* GetEnvelope(TH1* central,const vector<const TH1*>& variations);
  TH1* GetEnvelope(TH1* central,TH1* variation1,TH1* variation2=NULL,TH1* variation3=NULL,TH1* variation4=NULL,TH1* variation5=NULL,TH1* variation6=NULL,TH1* variation7=NULL,TH1* variation8=NULL,TH1* variation9=NULL);
  TH1* GetHessianError(TH1* central,const vector<const TH1*>& variations);
  TH1* GetRMSError(TH1* central,const vector<const TH1*>& variations);
  TH1* GetCorrelatedError(TH1* central,const vector<const TH1*>& variations);
  TH1* GetHistSys(TH1* hist0,const vector<const TH1*>& variations,Systematic::Type type);
  TH1* GetHistSys(Hists& hists,TString sysname);
  void EvalHistSys(Hists& hists,TString sysname,TString option);
  void EvalHistSyss(Plot& p);
  TMatrixD GetCov(Hists& hists,TString sysname);
  TMatrixD GetCov(Hists& hists0,Hists& hists1,TString sysname);
  TMatrixD GetCov(TH1* central,const vector<const TH1*>& variations,Systematic::Type type);
  static double Trace(TMatrixD mat);
  int AddError(TH1* hist,TH1* sys) const;
  void AddSystematic(TString key,TString title,Systematic::Type type,vector<TString> includes,TString tag="");
  void AddSystematic(TString key,TString title,Systematic::Type type,TString includes,TString tag="");

  //Canvas
  void DrawCompare(Plot p);
  vector<Hists> GetRatioHists(Plot p);
  void DrawRatio(Plot p);
  void DrawDiff(Plot p);
  void DrawSig(Plot p);
  void DrawCompareAndRatio(Plot p);
  void DrawCompareAndDiff(Plot p);
  void DrawCompareAndSig(Plot p);
  void DrawDoubleRatio(Plot& plot);
  virtual TCanvas* DrawPlot(Plot plotkey,TString additional_option="");
  virtual TCanvas* DrawPlot(TString plotkey,TString additional_option="");
  virtual TCanvas* DrawPlot(vector<TH1*> hists,TString additional_option="");
  vector<TCanvas*> DrawPlots(TRegexp plotkey,TString additional_option="");
  void SavePlots(TRegexp plotkey);
  void SavePlot(TString plotkey,TString option="",bool delete_canvas=true);
  void SavePlotCondor(TString plotkey,TString option="");
  void SaveCanvas(TCanvas *c,TString path,bool delete_canvas=true);
  void SavePlotAll();

  //Table
  void PrintTable(int ientry,TString plotkey,TString option="");

  //Canvas tools
  TString GetHistNameWithPrefixAndSuffix(TString histname,TString prefix,TString suffix);
  static TH1* GetAxisParent(TVirtualPad* pad=NULL);
  static TLegend* GetLegend(TVirtualPad* pad=NULL);
  static void SetLegendEntryLabel(TLegend* legend,int i,TString label);
  TH1* GetTH1(const TH1* hstack,bool deleteorigin=false) const;
  void DrawHistograms(Plot p);
  static vector<TH1*> VectorTH1(const vector<Hists>& hists);
  pair<double,double> GetMinMax(const vector<TH1*>& hists,TString option="") const;
  pair<double,double> GetMinMax(const vector<Hists>& hists,TString option="") const;
  void RebinXminXmax(TH1* hist,Plot p);
  double Normalize(TH1* hists,double val=1.,TString option="");
  vector<double> Normalize(vector<TH1*> hists,double val=1.,TString option="");
  vector<vector<double>> Normalize(vector<Hists> hists,double val=1.,TString option="");
  void WidthWeight(TH1* hist);
  void WidthWeight(vector<TH1*> hists);
  bool IsEntry(const Sample& sample);
  static TLegend* DrawLegend(const Plot& p);
  static TLegend* DrawLegendSys(const Plot& p);
  static void DrawPreliminary(TPad* c=NULL,TString era="",TString lumi="",TString option="");
  static void DrawTexts(const Plot& p);
  
  //Plot
  set<TString> GetHistKeys(TDirectory* dir,TRegexp regexp=".*");
  set<TString> GetHistKeys(TString filename,TString regexp=".*");
  set<TString> GetHistKeys(const Sample& sample,TString regexp=".*");
  virtual Plot MakePlot(TString plotkey,TString plotoption="");

  //etc
  TString ClassName() const;
  double GetChi2(TH1* h1,TH1* h2) const;
  double GetChi2(const TH1D* h1,const TH1D* h2,TMatrixD cov) const;
  double GetChi2(const TH1D* h1,const TH1D* h2,TMatrixD cov,int mask) const;
  int GetAutoColor() const;
  TString ParseForCondorRun(TString str) const;

  //working

  //TCanvas* GetCompareAFBAll(vector<TString> histnames,int sysbit=0,TString option="");
  //TCanvas* GetCompareAFBAll(TRegexp regexp,int sysbit=0,TString option="");
};

Plotter::Plotter(){
  SetupStyle();
  pdir=new TDirectory("plotdir","plotdir");
}
Plotter::~Plotter(){
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Setup ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Plotter::AddFile(TString key,TString file){
  Sample sample=Sample(file,"FILE");
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
  }else if(!samples[key].IsFile()||samples[key].title!=file) PError("[Plotter::AddFile] sample "+key+" already exists");
}
void Plotter::ScanFiles(TString path){
  scandir=path;
  vector<TString> files=Split(gSystem->GetFromPipe("find -L "+path+" -type f -name '*.root'"),"\n");
  if(!path.EndsWith("/")) path+="/";
  for(const auto& file:files){
    TString key=Replace(file,path,"");
    key=Replace(key,".root$","");
    AddFile(key,file);
  }
}
int Plotter::Setup(TString mode_,TString scandir){
  ScanFiles(scandir);
  SetupEntries(mode_);
  return true;
}
void Plotter::SetupStyle(){
  TH1::SetDefaultSumw2(kTRUE);
  gStyle->SetCanvasDefH(600);
  gStyle->SetCanvasDefW(700);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadColor(0);
  gStyle->SetFrameFillStyle(0);
  TGaxis::SetExponentOffset(-0.06,0.02);
}
TString Plotter::GetSetupStringForCondor(){
  return "Setup\\(\\\\\\\""+mode+"\\\\\\\",\\\\\\\""+scandir+"\\\\\\\"\\)";
}
void Plotter::SetupEntries(TString mode_){
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[Plotter::SetupEntries] mode = '"<<mode_<<"'"<<endl;
  entries.clear();
  mode=mode_;
  vector<TString> entry_keys=Split(mode_," ");
  for(auto entry_key:entry_keys) AddEntry(entry_key);
}
void Plotter::Reset(){
  samples.clear();
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
  if(key.BeginsWith("^data")){
    entry=Sample("data (sub.)","STACK",Style(kRed,-1,3001,"e2"),Style(kGreen,-1,3001,"e2"),Style(kBlue,-1,3001,"e2"),Style(kOrange,-1,3001,"e2"));
    key=key(1,key.Length()-1);
  }else if(key.BeginsWith("^")){
    entry=Sample("simulation","STACK",Style(kRed,-1,3001,"e2"),Style(kGreen,-1,3001,"e2"),Style(kBlue,-1,3001,"e2"),Style(kOrange,-1,3001,"e2"));
    key=key(1,key.Length()-1);
  }else if(key.BeginsWith("data")){
    entry=Sample("data (sub.)","SUM");
  }else entry=Sample("simulation","SUM");
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
      if(sample_keys.size()==1&&!entry.IsStack()){
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
    TString title=sample.title;
    title=title(title.Last('/')+1,title.Length());
    title.ReplaceAll(".root","");
    Sample s(title,"SAMPLE",GetAutoColor());
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
    if(!sample.IsFile()) continue;
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
    if(sample.IsFile()) continue;
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
vector<double> Plotter::GetAxisVector(const TAxis* axis) const {
  vector<double> bins;
  if(!axis) return bins;
  const double* array=axis->GetXbins()->GetArray();
  if(array){
    bins.assign(array,array+axis->GetNbins()+1);
  }else{
    for(int i=1;i<axis->GetNbins()+2;i++){
      bins.push_back(axis->GetBinLowEdge(i));
    }
  }
  return bins;
}
TH1* Plotter::GetHist(TString samplekey,TString plotkey,TString additional_option){
  return GetHist(samples[samplekey],MakePlot(plotkey,additional_option));
}
TH1* Plotter::GetHist(int ientry,TString plotkey,TString additional_option){
  if((int)entries.size()<=ientry){
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
  if(!sample.PassFilter(p.histname)){
    _depth--;
    return hist;
  }
  if(sample.IsStack()){
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
  }else if(sample.IsSum()){
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
  }else if(p.option.Contains("absx")){
    Plot temp_plot=p-"absx";
    TH1* temp_hist=GetHistFromSample(sample,temp_plot);
    if(temp_hist){
      int nbin=temp_hist->GetNbinsX();
      bool ok=true;
      vector<double> bins;
      for(int i=1;i<=nbin/2;i++){
	if(fabs(temp_hist->GetBinLowEdge(i)+temp_hist->GetBinLowEdge(nbin+2-i))>1e-10){
	  PError("[Plotter::GetHistFromSample] cannot do absx with asymmetric binning");
	  ok=false;
	}
	bins.push_back(temp_hist->GetBinLowEdge(nbin/2+i));
      }
      bins.push_back(temp_hist->GetBinLowEdge(nbin+1));
      if(ok){
	hist=new TH1D(temp_hist->GetName(),temp_hist->GetTitle(),nbin/2,&bins[0]);
	hist->SetDirectory(pdir);
	for(int i=1;i<nbin/2+2;i++){
	  hist->SetBinContent(i,temp_hist->GetBinContent(nbin/2+1-i)+temp_hist->GetBinContent(nbin/2+i));
	  hist->SetBinError(i,sqrt(pow(temp_hist->GetBinError(nbin/2+1-i),2)+pow(temp_hist->GetBinError(nbin/2+i),2)));
	}
	delete temp_hist;
      }
    }
  }else if(p.option.Contains("xsquare")){
    Plot temp_plot=p-"xsquare";
    hist=GetHistFromSample(sample,temp_plot);
    if(hist){
      vector<double> bins;
      for(int i=1;i<hist->GetNbinsX()+2;i++) bins.push_back(pow(hist->GetBinLowEdge(i),2));
      hist->SetBins(bins.size()-1,&bins[0]);
    }
  }else{
    for(auto [reg,newstr]:sample.replace) TPRegexp(reg).Substitute(p.histname,newstr);
    TString histname=p.histname;
    if(sample.HasTag(p.replace_tag)&&(p.replace_old!=""||p.replace_new!="")) TPRegexp(p.replace_old).Substitute(histname,p.replace_new);
    for(const auto& file:sample.subs){
      TH1* this_hist=GetHistFromFile(file.title,histname);
      if(this_hist){
	if(strstr(this_hist->ClassName(),"TH1")!=NULL){
	}else if(strstr(this_hist->ClassName(),"TH2")!=NULL){
	  TH2* hist2d=(TH2*)this_hist;
	  if(p.rebinX!=""||p.rebinY!=""||p.option.Contains("absX")||p.option.Contains("absY")){
	    vector<double> binsX= p.rebinX!="" ? VectorDouble(p.rebinX) : GetAxisVector(hist2d->GetXaxis());
	    vector<double> binsY= p.rebinY!="" ? VectorDouble(p.rebinY) : GetAxisVector(hist2d->GetYaxis());
	    if(p.option.Contains("absX")){
	      binsX.assign(binsX.end()-binsX.size()/2-1,binsX.end());
	      binsX[0]=0.;
	    }
	    if(p.option.Contains("absY")){
	      binsY.assign(binsY.end()-binsY.size()/2-1,binsY.end());
	      binsY[0]=0.;
	    }
	    TH2D* hist2d_rebin=new TH2D(hist2d->GetName(),hist2d->GetTitle(),binsX.size()-1,&binsX[0],binsY.size()-1,&binsY[0]);
	    for(int ix=0;ix<hist2d->GetXaxis()->GetNbins()+2;ix++){
	      for(int iy=0;iy<hist2d->GetYaxis()->GetNbins()+2;iy++){
		double x=hist2d->GetXaxis()->GetBinCenter(ix);
		double y=hist2d->GetYaxis()->GetBinCenter(iy);
		if(p.option.Contains("absX")) x=fabs(x);
		if(p.option.Contains("absY")) y=fabs(y);
		int ibin=hist2d_rebin->FindBin(x,y);
		hist2d_rebin->SetBinContent(ibin,hist2d_rebin->GetBinContent(ibin)+hist2d->GetBinContent(ix,iy));
		hist2d_rebin->SetBinError(ibin,sqrt(pow(hist2d_rebin->GetBinError(ibin),2)+pow(hist2d->GetBinError(ix,iy),2)));
	      }
	    }
	    delete hist2d;
	    hist2d=hist2d_rebin;
	  }
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
	  if(!p.option.Contains("noproject")){
	    if(p.project==""){
	      if(ixmin==0&&ixmax==-1){
		ixmin=1; ixmax=hist2d->GetNbinsX();
	      }
	      if(iymin==0&&iymax==-1){
		iymin=1; iymax=hist2d->GetNbinsY();
	      }
	      int nbin=(ixmax-ixmin+1)*(iymax-iymin+1);
	      this_hist=new TH1D(hist2d->GetName(),hist2d->GetTitle(),nbin,0,nbin);
	      int ibin=0;
	      for(int iy=iymin;iy<=iymax;iy++){
		for(int ix=ixmin;ix<=ixmax;ix++){
		  double val=hist2d->GetBinContent(ix,iy);
		  double err=hist2d->GetBinError(ix,iy);
		  this_hist->SetBinContent(++ibin,val);
		  this_hist->SetBinError(ibin,err);
		}
	      }
	      if(p.xtitle=="") p.xtitle="bin index";
	    }
	    else if(p.project=="x") this_hist=(TH1*)hist2d->ProjectionX("_px",iymin,iymax);
	    else if(p.project=="y") this_hist=(TH1*)hist2d->ProjectionY("_py",ixmin,ixmax);
	    else PError("[Plotter::GetHistFromSample] wrong projection or classname");
	    delete hist2d;
	  }
	}else if(strstr(this_hist->ClassName(),"TH3")!=NULL){
	  TH3* hist3d=(TH3*)this_hist;
	  if(p.rebinX!=""||p.rebinY!=""||p.rebinZ!=""||p.option.Contains("absX")||p.option.Contains("absY")||p.option.Contains("absZ")){
	    vector<double> binsX= p.rebinX!="" ? VectorDouble(p.rebinX) : GetAxisVector(hist3d->GetXaxis());
	    vector<double> binsY= p.rebinY!="" ? VectorDouble(p.rebinY) : GetAxisVector(hist3d->GetYaxis());
	    vector<double> binsZ= p.rebinZ!="" ? VectorDouble(p.rebinZ) : GetAxisVector(hist3d->GetZaxis());
	    if(p.option.Contains("absX")){
	      binsX.assign(binsX.end()-binsX.size()/2-1,binsX.end());
	      binsX[0]=0.;
	    }
	    if(p.option.Contains("absY")){
	      binsY.assign(binsY.end()-binsY.size()/2-1,binsY.end());
	      binsY[0]=0.;
	    }
	    if(p.option.Contains("absZ")){
	      binsZ.assign(binsZ.end()-binsZ.size()/2-1,binsZ.end());
	      binsZ[0]=0.;
	    }
	    TH3D* hist3d_rebin=new TH3D(hist3d->GetName(),hist3d->GetTitle(),binsX.size()-1,&binsX[0],binsY.size()-1,&binsY[0],binsZ.size()-1,&binsZ[0]);
	    for(int ix=0;ix<hist3d->GetXaxis()->GetNbins()+2;ix++){
	      for(int iy=0;iy<hist3d->GetYaxis()->GetNbins()+2;iy++){
		for(int iz=0;iz<hist3d->GetZaxis()->GetNbins()+2;iz++){
		  double x=hist3d->GetXaxis()->GetBinCenter(ix);
		  double y=hist3d->GetYaxis()->GetBinCenter(iy);
		  double z=hist3d->GetZaxis()->GetBinCenter(iz);
		  if(p.option.Contains("absX")) x=fabs(x);
		  if(p.option.Contains("absY")) y=fabs(y);
		  if(p.option.Contains("absZ")) z=fabs(z);
		  int ibin=hist3d_rebin->FindBin(x,y,z);
		  hist3d_rebin->SetBinContent(ibin,hist3d_rebin->GetBinContent(ibin)+hist3d->GetBinContent(ix,iy,iz));
		  hist3d_rebin->SetBinError(ibin,sqrt(pow(hist3d_rebin->GetBinError(ibin),2)+pow(hist3d->GetBinError(ix,iy,iz),2)));
		}
	      }
	    }
	    delete hist3d;
	    hist3d=hist3d_rebin;
	  }
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
	    if(p.project==""){
	      if(ixmin==0&&ixmax==-1){
		ixmin=1; ixmax=hist3d->GetNbinsX();
	      }
	      if(iymin==0&&iymax==-1){
		iymin=1; iymax=hist3d->GetNbinsY();
	      }
	      if(izmin==0&&izmax==-1){
		izmin=1; izmax=hist3d->GetNbinsZ();
	      }
	      int nbin=(ixmax-ixmin+1)*(iymax-iymin+1)*(izmax-izmin+1);
	      this_hist=new TH1D(hist3d->GetName(),hist3d->GetTitle(),nbin,0,nbin);
	      int ibin=0;
	      for(int iz=izmin;iz<=izmax;iz++){
		for(int iy=iymin;iy<=iymax;iy++){
		  for(int ix=ixmin;ix<=ixmax;ix++){
		    double val=hist3d->GetBinContent(ix,iy,iz);
		    double err=hist3d->GetBinError(ix,iy,iz);
		    this_hist->SetBinContent(++ibin,val);
		    this_hist->SetBinError(ibin,err);
		  }
		}
	      }
	      if(p.xtitle=="") p.xtitle="bin index";
	    }
	    else if(p.project=="x") this_hist=(TH1*)hist3d->ProjectionX("_px",iymin,iymax,izmin,izmax);
	    else if(p.project=="y") this_hist=(TH1*)hist3d->ProjectionY("_py",ixmin,ixmax,izmin,izmax);
	    else if(p.project=="z") this_hist=(TH1*)hist3d->ProjectionZ("_pz",ixmin,ixmax,iymin,iymax);
	    else if(p.project=="xy"){
	      if(ixmin==0&&ixmax==-1){
		ixmin=1; ixmax=hist3d->GetNbinsX();
	      }
	      if(iymin==0&&iymax==-1){
		iymin=1; iymax=hist3d->GetNbinsY();
	      }
	      if(izmin==0&&izmax==-1){
		izmin=0; izmax=hist3d->GetNbinsZ()+1;
	      }
	      hist3d->GetZaxis()->SetRange(izmin,izmax);
	      TH2* hist2d=(TH2*)hist3d->Project3D("yx o");
	      int nbin=(ixmax-ixmin+1)*(iymax-iymin+1);
	      this_hist=new TH1D(hist3d->GetName(),hist3d->GetTitle(),nbin,0,nbin);
	      int ibin=0;
	      for(int iy=iymin;iy<=iymax;iy++){
		for(int ix=ixmin;ix<=ixmax;ix++){
		  double val=hist2d->GetBinContent(ix,iy);
		  double err=hist2d->GetBinError(ix,iy);
		  this_hist->SetBinContent(++ibin,val);
		  this_hist->SetBinError(ibin,err);
		}
	      }
	      if(p.xtitle=="") p.xtitle="bin index";
	      delete hist2d;
	    }else if(p.project=="yz"){
	      if(ixmin==0&&ixmax==-1){
		ixmin=0; ixmax=hist3d->GetNbinsX()+1;
	      }
	      if(iymin==0&&iymax==-1){
		iymin=1; iymax=hist3d->GetNbinsY();
	      }
	      if(izmin==0&&izmax==-1){
		izmin=1; izmax=hist3d->GetNbinsZ();
	      }
	      hist3d->GetXaxis()->SetRange(ixmin,ixmax);
	      TH2* hist2d=(TH2*)hist3d->Project3D("zy o");
	      int nbin=(iymax-iymin+1)*(izmax-izmin+1);
	      this_hist=new TH1D(hist3d->GetName(),hist3d->GetTitle(),nbin,0,nbin);
	      int ibin=0;
	      for(int iz=izmin;iz<=izmax;iz++){
		for(int iy=iymin;iy<=iymax;iy++){
		  double val=hist2d->GetBinContent(iy,iz);
		  double err=hist2d->GetBinError(iy,iz);
		  this_hist->SetBinContent(++ibin,val);
		  this_hist->SetBinError(ibin,err);
		}
	      }
	      if(p.xtitle=="") p.xtitle="bin index";
	      delete hist2d;
	    }else if(p.project=="zx"){
	      if(ixmin==0&&ixmax==-1){
		ixmin=1; ixmax=hist3d->GetNbinsX();
	      }
	      if(iymin==0&&iymax==-1){
		iymin=0; iymax=hist3d->GetNbinsY()+1;
	      }
	      if(izmin==0&&izmax==-1){
		izmin=1; izmax=hist3d->GetNbinsZ();
	      }
	      hist3d->GetYaxis()->SetRange(iymin,iymax);
	      TH2* hist2d=(TH2*)hist3d->Project3D("xz o");
	      int nbin=(izmax-izmin+1)*(ixmax-ixmin+1);
	      this_hist=new TH1D(hist3d->GetName(),hist3d->GetTitle(),nbin,0,nbin);
	      int ibin=0;
	      for(int ix=ixmin;ix<=ixmax;ix++){
		for(int iz=izmin;iz<=izmax;iz++){
		  double val=hist2d->GetBinContent(iz,ix);
		  double err=hist2d->GetBinError(iz,ix);
		  this_hist->SetBinContent(++ibin,val);
		  this_hist->SetBinError(ibin,err);
		}
	      }
	      if(p.xtitle=="") p.xtitle="bin index";
	      delete hist2d;
	    }

	    else PError("[Plotter::GetHistFromSample] wrong projection or classname");
	    delete hist3d;
	  }
	}else if(strstr(this_hist->ClassName(),"TProfile")!=NULL){ 
	}else{
	  TObject* obj=(TObject*)this_hist;
	  GetHistActionForAdditionalClass(obj,p);
	  this_hist=(TH1*)obj;
	}
	this_hist->SetName(sample.title);
	this_hist->Scale(file.weight);
	if(!hist){
	  hist=(TH1*)this_hist->Clone();
	  hist->SetDirectory(pdir);
	  hist->Reset();
	}
	hist->Add(this_hist,sample.weight);
	delete this_hist;
      }
    }
    if(!hist&&sample.HasTag(p.replace_tag)&&(p.replace_old!=""||p.replace_new!="")){
      hist=GetHistFromSample(sample,pp+additional_option-"replace");
      if(hist)
	PWarning("[Plotter::GetHistFromSample] no "+histname+" "+sample.title+", use "+p.histname);
    }
  }
  if(hist){
    sample.ApplyStyle(hist);
    if(hist->InheritsFrom("TH1")&&p.option.Contains("nostat")){
      for(int i=0;i<hist->GetNcells();i++){
	hist->SetBinError(i,0);
      }
      hist->SetBinError(hist->GetNcells()/2,1e-10);
    }
    for(auto [scale,tag]:p.scales){
      if(sample.HasTag(tag,false)){
	hist->Scale(scale);
      }
    }
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
Hists Plotter::GetHistVariations(TString samplekey,TString plotkey,TString additional_option){
  return GetHistVariations(samples[samplekey],MakePlot(plotkey,additional_option));
}
Hists Plotter::GetHistVariations(int ientry,TString plotkey,TString additional_option){
  if((int)entries.size()<=ientry){
    PError("[Plotter::GetHistVariations] entries.size()<=ientry");
    return Hists();
  }
  return GetHistVariations(entries[ientry],MakePlot(plotkey,additional_option));
} 
Hists Plotter::GetHistVariations(const Sample& sample,const Plot& p){
  PInfo("[Plotter::GetHistVariations] "+sample.title+" "+p.name+" "+p.histname+" "+p.sysname);_depth++;

  Hists hists;
  hists.push_back(GetHist(sample,p));
  hists.entity=&sample;

  if(p.sysname!=""&&hists.at(0)){
    if(systematics.find(p.sysname)!=systematics.end()){
      map<TString,TString> variations=GetSystematicSuffixes(p.sysname);
      for(auto& [suffix,variation]:variations){
	TH1* this_hist=NULL;
	if(sample.HasTag(GetTagFromVariation(variation))){
	  this_hist=GetHist(sample,p,variation);
	  this_hist=GetTH1(this_hist,true);
	}
	hists.variations[suffix]=this_hist;
      }
      if(p.option.Contains("shapesys")){
	TString integraloption="";
	if(p.option.Contains("widthweight")) integraloption="width";
	TH1* hist0=GetTH1(hists.at(0));
	double val=hist0->Integral(integraloption);
	for(auto& [suffix,h]:hists.variations){
	  if(h) h->Scale(val/h->Integral(integraloption));
	}
	delete hist0;
      }
    }
  }
  if(hists.at(0)){
    sample.ApplyStyle(hists.at(0));
  }
  _depth--;
  return hists;
}
/*
      if(systematic.type==Systematic::Type::MULTI){
	vector<TH1*> syss;
	for(const auto& this_key:systematic.keys){
	  if(systematics.find(this_key)==systematics.end()){
	    PError("[Plotter::GetHistVariations] no systematic with key "+this_key);
	  }else{
	    Hists this_histsys=GetHistVariations(sample,p+("sysname:"+this_key)-"sysdetail"+"notaddsys");
	    if(this_histsys.size()==2){
	      //this_histsys.at(1)->SetName(this_key);
	      //this_histsys.at(1)->SetTitle(systematics[this_key].title);
	      this_histsys.at(1)->SetName(systematics[this_key].title);
	      syss.push_back(this_histsys.at(1));
	      if(p.option.Contains("chi2")){
		if(hists.cov.GetNcols()==0){
		  hists.cov.ResizeTo(this_histsys.cov);
		}
		hists.cov+=this_histsys.cov;
		if(p.option.Contains("chi2detail")){
		  TString title=systematics[this_key].title;
		  hists.subcovs[title].ResizeTo(this_histsys.cov);
		  hists.subcovs[title]=this_histsys.cov;
		}
	      }
	    }else if(this_histsys.size()>2) PError("[Plotter::GetHistVariations] this_histsys.size() > 2");
	  }
	}
	if(syss.size()>0){
	  if(p.option.Contains("sysdetail")){
	    for(int i=0;i<(int)syss.size();i++){
	      if(!p.option.Contains("notaddsys")){
		if(i==0) AddError(syss.at(i),hist0);
		else AddError(syss.at(i),syss.at(i-1));
	      }
	      hists.push_back(syss.at(i));
	    }
	  }else{
	    TH1* totalsys=syss.at(0);
	    for(int i=1;i<(int)syss.size();i++){
	      AddError(totalsys,syss.at(i));
	      delete syss.at(i);
	    }
	    totalsys->SetName(systematic.title);
	    //totalsys->SetTitle(systematic.title);
	    if(!p.option.Contains("notaddsys")) AddError(totalsys,hist0);
	    hists.push_back(totalsys);
	  }
	}
      }else if(sample.HasTag(systematic.tag)){
	PDebug("[Plotter::GetHistVariations] sysname='"+p.sysname+"' systype='"+systematic.GetTypeString()+"'");
	vector<TH1*> variations;
	for(const auto& option_variation:systematic.variations){
	  TH1* this_hist=GetHist(sample,p,option_variation);
	  this_hist=GetTH1(this_hist,true);
	  variations.push_back(this_hist);
	}
	PDebug("[Plotter::GetHistVariations] '"+p.sysname+"': "+variations.size()+" variations");
	if(p.option.Contains("shapesys")){
	  TString integraloption="";
	  if(p.option.Contains("widthweight")) integraloption="width";
	  double val=hist0->Integral(integraloption);
	  for(auto& var:variations){
	    if(var) var->Scale(val/var->Integral(integraloption));
	  }
	}
	if(p.option.Contains("chi2")){
	  TMatrixD cov(hist0->GetXaxis()->GetFirst(),hist0->GetXaxis()->GetLast(),hist0->GetXaxis()->GetFirst(),hist0->GetXaxis()->GetLast());
	  if(hists.cov.GetNcols()==0){
	    hists.cov.ResizeTo(cov);
	    hists.cov=cov;
	  }
	  cov=GetCov(hist0,variations,systematic.type);
	  if(p.option.Contains("decotest")&&p.sysname=="dyscale"&&p.histname.Contains("costym")){
	    if(p.project.Contains("x")||p.project==""){
	      for(int irow=cov.GetRowLwb(),nrow=cov.GetRowUpb()+1;irow<nrow;irow++){
		for(int icol=cov.GetColLwb(),ncol=cov.GetColUpb()+1;icol<ncol;icol++){
		  if((irow-1)/10==(icol-1)/10) continue;
		  cov[icol][irow]=0;
		}
	      }	     
	    }else{
	      for(int irow=cov.GetRowLwb(),nrow=cov.GetRowUpb()+1;irow<nrow;irow++){
		for(int icol=cov.GetColLwb(),ncol=cov.GetColUpb()+1;icol<ncol;icol++){
		  if(irow==icol) continue;
		  cov[icol][irow]=0;
		}
	      }
	    }
	  }
	  if(p.option.Contains("decoeffstat")&&p.sysname=="efficiencySF_stat"){
	    for(int irow=cov.GetRowLwb(),nrow=cov.GetRowUpb()+1;irow<nrow;irow++){
	      for(int icol=cov.GetColLwb(),ncol=cov.GetColUpb()+1;icol<ncol;icol++){
		if(irow==icol) continue;
		cov[icol][irow]=0;
	      }
	    }
	  }
	  hists.cov+=cov;
	}
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
	    histsys=GetEnvelope(hist0,variations);
	  }else if(systematic.type==Systematic::Type::GAUSSIAN){
	    histsys=GetRMSError(hist0,variations);
	  }else if(systematic.type==Systematic::Type::HESSIAN){
	    histsys=GetHessianError(hist0,variations);
	  }else if(systematic.type==Systematic::Type::CORRELATED){
	    histsys=GetCorrelatedError(hist0,variations);
	  }else{
	    PError((TString)"[Plotter::GetHistVariations] Wrong Systematic::Type "+Form("%d",systematic.type));
	  }
	  if(histsys){
	    sample.ApplyStyle(histsys,true);
	    histsys->SetName(systematic.title);
	    if(!p.option.Contains("notaddsys")) AddError(histsys,hist0);
	    hists.push_back(histsys);
	  }
	  for(unsigned int j=0;j<variations.size();j++){
	    delete variations.at(j);
	  }
	}
      }
      delete hist0;
    }
*/

vector<Hists> Plotter::GetHists(Plot& p){
  vector<Hists> hists;
  for(const auto& entry:entries) hists.push_back(GetHistVariations(entry,p));
  if(p.option.Contains("norm")&&hists.size()>1){
    TH1* h=GetTH1(hists.at(0).at(0));
    if(h){
      TString integraloption="";
      if(p.option.Contains("widthweight")) integraloption="width";
      if(p.option.Contains("normTo1")) h->Scale(1./h->Integral(integraloption));
      double val=h->Integral(integraloption);
      delete h;
      vector<vector<double>> scales=Normalize(hists,val,integraloption);
      if(scales.size()>1){
	TString newytitle="'"+p.ytitle+Form(" (Norm. SF=%.3f",scales.at(1).at(0));
	for(unsigned int i=2;i<scales.size();i++) newytitle+=Form(",%.3f",scales.at(i).at(0));
	newytitle+=")'";
	if(!p.option.Contains("normTo1")) p.SetOption("1:ytitle:"+newytitle);
	else  p.SetOption("1:ytitle:A.U");
      }
    }
  }
  return hists;
}
void Plotter::Blind(Plot& p){
  if(p.blind_xmin||p.blind_xmax){
    for(int i=0,n=entries.size();i<n;i++){
      if(entries[i].HasTag("data",false)){
	for(auto& h:p.hists.at(i)){
	  if(!h) continue;
	  int binmin=0,binmax=h->GetNcells();
	  if(p.blind_xmin!=p.blind_xmax){
	    binmin=h->FindBin(p.blind_xmin);
	    binmax=h->FindBin(p.blind_xmax);
	  }
	  if(i==0&&entries.size()==2&&p.hists.at(1).at(0)){
	    for(int ibin=binmin;ibin<binmax;ibin++){
	      h->SetBinContent(ibin,p.hists.at(1).at(0)->GetBinContent(ibin));
	      //h->SetBinError(ibin,0);
	    }	    
	  }else{
	    for(int ibin=binmin;ibin<binmax;ibin++){
	      h->SetBinContent(ibin,0);
	      //h->SetBinError(ibin,0);
	    }
	  }
	}
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Uncertainty //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TString Plotter::GetTagFromVariation(TString variation){
  vector<TString> words=Split(variation,",");
  if(words.size()==3){
    return words[2];
  }
  return "";
}
set<TString> Plotter::GetSystematicVariations(TString sysname){
  set<TString> rt;
  auto it=systematics.find(sysname);
  if(it==systematics.end()){
    PError("No systematic "+sysname);
    return rt;
  }
  const Systematic& sys=it->second;
  if(sys.type==Systematic::Type::MULTI){
    for(const auto& key:sys.keys){
      set<TString> this_rt=GetSystematicVariations(key);
      rt.insert(this_rt.begin(),this_rt.end());
    }
  }else{
    set<TString> this_rt(sys.variations.begin(),sys.variations.end());
    rt.insert(this_rt.begin(),this_rt.end());
  }    
  return rt;
}
TString Plotter::Variation2Suffix(TString variation){
  TString suffix;
  Plot p;
  p.SetOption(variation);
  if(p.replace_old=="$"){
    suffix="_"+p.replace_tag+p.replace_new;
  }else if(p.scales.size()){
    suffix="_"+p.scales[0].second+"_scale";
  }else{
    PError("[Plotter::GetSystematicSuffixes] Unknown variation "+variation);
    exit(1);
  }
  suffix.ReplaceAll("&&","_and_");
  suffix.ReplaceAll("||","_or_");
  suffix.ReplaceAll("(","_l_");
  suffix.ReplaceAll(")","_l_");
  return suffix;
}
map<TString,TString> Plotter::GetSystematicSuffixes(TString sysname){
  set<TString> variations=GetSystematicVariations(sysname);
  map<TString,TString> rt;
  for(const auto& variation:variations){
    TString suffix=Variation2Suffix(variation);
    rt[suffix]=variation;
  }
  return rt;
}
TH1* Plotter::GetEnvelope(TH1* central,const vector<const TH1*>& variations){
  PAll("[Plotter::GetEnvelope]");
  if(strstr(central->ClassName(),"THStack")) central=GetTH1(central);
  TH1* syshist=(TH1*)central->Clone("sys");
  syshist->SetDirectory(pdir);
  for(int i=0;i<syshist->GetNbinsX()+2;i++) syshist->SetBinError(i,0);
  for(int i=0;i<(int)variations.size();i++){
    for(int j=0;j<syshist->GetNbinsX()+2;j++){
      double diff=fabs(syshist->GetBinContent(j)-variations.at(i)->GetBinContent(j));
      if(diff>syshist->GetBinError(j)) syshist->SetBinError(j,diff);
    }
  }
  return syshist;
}
TH1* Plotter::GetEnvelope(TH1* central,TH1* variation1,TH1* variation2,TH1* variation3,TH1* variation4,TH1* variation5,TH1* variation6,TH1* variation7,TH1* variation8,TH1* variation9){
  vector<const TH1*> variations;
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
TH1* Plotter::GetHessianError(TH1* central,const vector<const TH1*>& variations){
  PAll("[Plotter::GetHessianError]");
  if(strstr(central->ClassName(),"THStack")) central=GetTH1(central);
  TH1* syshist=(TH1*)central->Clone("sys");
  syshist->SetDirectory(pdir);
  for(int i=0;i<syshist->GetNbinsX()+2;i++) syshist->SetBinError(i,0);
  for(int i=0;i<(int)variations.size();i++){
    for(int j=0;j<syshist->GetNbinsX()+2;j++){
      double diff=fabs(syshist->GetBinContent(j)-variations.at(i)->GetBinContent(j));
      syshist->SetBinError(j,sqrt(pow(syshist->GetBinError(j),2)+pow(diff,2)));
    }
  }
  return syshist;
}  
TH1* Plotter::GetRMSError(TH1* central,const vector<const TH1*>& variations){
  PAll("[Plotter::GetRMSError]");
  if(strstr(central->ClassName(),"THStack")) central=GetTH1(central);
  TH1* syshist=(TH1*)central->Clone("sys");
  syshist->SetDirectory(pdir);
  for(int i=0;i<syshist->GetNbinsX()+2;i++) syshist->SetBinError(i,0);
  for(int i=0;i<(int)variations.size();i++){
    for(int j=0;j<syshist->GetNbinsX()+2;j++){
      double diff=fabs(syshist->GetBinContent(j)-variations.at(i)->GetBinContent(j));
      syshist->SetBinError(j,sqrt(pow(syshist->GetBinError(j),2)+pow(diff,2)));
    }
  }
  for(int i=1;i<syshist->GetNbinsX()+1;i++) syshist->SetBinError(i,syshist->GetBinError(i)/sqrt(variations.size()));
  return syshist;
}
TH1* Plotter::GetCorrelatedError(TH1* central,const vector<const TH1*>& variations){
  PAll("[Plotter::GetCorrelatedError]");
  if(strstr(central->ClassName(),"THStack")) central=GetTH1(central);
  TH1* syshist=(TH1*)central->Clone("sys");
  syshist->SetDirectory(pdir);
  for(int i=0;i<syshist->GetNbinsX()+2;i++) syshist->SetBinError(i,0);
  for(int j=0;j<syshist->GetNbinsX()+2;j++){
    double total_diff=0;
    for(int i=0;i<(int)variations.size();i++){
      double diff=variations.at(i)->GetBinContent(j)-syshist->GetBinContent(j);
      total_diff+=diff;
    }
    syshist->SetBinError(j,total_diff);
  }
  return syshist;
}
TH1* Plotter::GetHistSys(TH1* hist0,const vector<const TH1*>& variations,Systematic::Type type){
  TH1* histsys=NULL;
  if(type==Systematic::Type::ENVELOPE){
    histsys=GetEnvelope(hist0,variations);
  }else if(type==Systematic::Type::GAUSSIAN){
    histsys=GetRMSError(hist0,variations);
  }else if(type==Systematic::Type::HESSIAN){
    histsys=GetHessianError(hist0,variations);
  }else if(type==Systematic::Type::CORRELATED){
    histsys=GetCorrelatedError(hist0,variations);
  }else{
    PError((TString)"[Plotter::GetHistSys] Wrong Systematic::Type "+Form("%d",type));
  }
  return histsys;
}
TH1* Plotter::GetHistSys(Hists& hists,TString sysname){
  TH1* histsys=NULL;
  if(sysname=="") return histsys;
  if(systematics.find(sysname)==systematics.end()){
    PError("[Plotter::GetHistSys] no systematic "+sysname);
    return histsys;
  }
  Systematic& systematic=systematics[sysname];
  if(systematic.type==Systematic::MULTI){
    for(const auto& this_key:systematic.keys){
      if(systematics.find(this_key)==systematics.end()){
	PError("[Plotter::GetHistSys] no systematic with key "+this_key);
      }else{
	TH1* this_histsys=GetHistSys(hists,this_key);
	if(histsys){
	  AddError(histsys,this_histsys);
	  this_histsys->SetDirectory(0);
	  delete this_histsys;
	}else{
	  histsys=this_histsys;
	}
      }
    }
  }else{
    vector<const TH1*> variations;
    for(auto& [suffix,variation]:GetSystematicSuffixes(sysname)){
      variations.push_back(hists[suffix]);
    }
    histsys=GetHistSys(hists.at(0),variations,systematic.type);
  }
  if(histsys){
    histsys->SetName(systematic.title);
  }
  return histsys;
}
void Plotter::EvalHistSys(Hists& hists,TString sysname,TString option){
  if(sysname=="") return;
  if(systematics.find(sysname)==systematics.end()){
    PError("[Plotter::GetHistSys] no systematic "+sysname);
  }
  Systematic& systematic=systematics[sysname];
  hists.resize(1);
  if(option.Contains("sysdetail")){
    if(systematic.type==Systematic::Type::MULTI){
      for(const auto& this_key:systematic.keys){
	hists.push_back(GetHistSys(hists,this_key));
      }
    }else{
      for(auto& [suffix,variation]:GetSystematicSuffixes(sysname)){
	hists.push_back((TH1*)hists[suffix]->Clone());
      }
    }
  }else{
    hists.push_back(GetHistSys(hists,sysname));
  }
  
  if(!option.Contains("notaddsys")
     &&!(option.Contains("sysdetail")&&systematic.type!=Systematic::Type::MULTI)){
    for(int i=1,n=hists.size();i<n;i++){
      AddError(hists.at(i),hists.at(i-1));
    }
  }
}
void Plotter::EvalHistSyss(Plot& p){
  if(p.sysname=="") return;
  // if(systematics.find(p.sysname)==systematics.end()){
  //   PError("[Plotter::GetHistSys] no systematic "+p.sysname);
  // }
  // Systematic& systematic=systematics[p.sysname];
  for(Hists& hists:p.hists){
    EvalHistSys(hists,p.sysname,p.option);
    // hists.resize(1);
    // if(p.option.Contains("sysdetail")){
    //   if(systematic.type==Systematic::Type::MULTI){
    // 	for(const auto& this_key:systematic.keys){
    // 	  hists.push_back(GetHistSys(hists,this_key));
    // 	}
    //   }else{
    // 	for(auto& [suffix,variation]:GetSystematicSuffixes(p.sysname)){
    // 	  hists.push_back((TH1*)hists[suffix]->Clone());
    // 	}
    //   }
    // }else{
    //   hists.push_back(GetHistSys(hists,p.sysname));
    // }
    
    // if(!p.option.Contains("notaddsys")
    //    &&!(p.option.Contains("sysdetail")&&systematic.type!=Systematic::Type::MULTI)){
    //   for(int i=1,n=hists.size();i<n;i++){
    // 	AddError(hists.at(i),hists.at(i-1));
    //   }
    // }
  }
}
TMatrixD Plotter::GetCov(Hists& hists0,Hists& hists1,TString sysname){
  Hists hists_diff;
  hists_diff.push_back(GetTH1(hists1.at(0)));
  hists_diff.at(0)->Add(hists0.at(0),-1);
  for(auto& [suffix,variation]:hists0.variations){
    hists_diff.variations[suffix]=GetTH1(hists1[suffix]);
    hists_diff.variations[suffix]->Add(hists0[suffix],-1);
  }
  return GetCov(hists_diff,sysname);
}
TMatrixD Plotter::GetCov(Hists& hists, TString sysname){
  int ifirst=hists.at(0)->GetXaxis()->GetFirst();
  int ilast=hists.at(0)->GetXaxis()->GetLast();
  TMatrixD cov(ifirst,ilast,ifirst,ilast);
  if(sysname=="") return cov;
  if(systematics.find(sysname)==systematics.end()){
    PError("[Plotter::GetCov] no systematic "+sysname);
    return cov;
  }
  Systematic& systematic=systematics[sysname];
  if(systematic.type==Systematic::MULTI){
    for(const auto& this_key:systematic.keys){
      if(systematics.find(this_key)==systematics.end()){
	PError("[Plotter::GetCov] no systematic with key "+this_key);
      }else{
	cov+=GetCov(hists,this_key);
      }
    }
  }else{
    vector<const TH1*> variations;
    for(auto& [suffix,variation]:GetSystematicSuffixes(sysname)){
      variations.push_back(hists[suffix]);
    }
    cov=GetCov(hists.at(0),variations,systematic.type);
  }
  return cov;
}
TMatrixD Plotter::GetCov(TH1* central,const vector<const TH1*>& variations,Systematic::Type type){
  int ifirst=central->GetXaxis()->GetFirst();
  int ilast=central->GetXaxis()->GetLast();
  TMatrixD cov(ifirst,ilast,ifirst,ilast);
  if(!central->InheritsFrom("TH1D")){
    PError((TString)"[Plotter::GetCov] not implemented");
  } 
  TH1D* hist0=(TH1D*)central;
  if(type==Systematic::Type::ENVELOPE){
    TMatrixD maxcov(ifirst,ilast,ifirst,ilast);
    double trace=0;
    for(const auto& var:variations){
      const TH1D* hist1=(TH1D*)var;
      TMatrixD val0(ifirst,ilast,0,0,hist0->GetArray()+ifirst);
      TMatrixD val(ifirst,ilast,0,0,hist1->GetArray()+ifirst);
      val-=val0;
      TMatrixD valT=val;
      valT.T();
      TMatrixD this_cov=val*valT;
      double this_trace=Trace(this_cov);
      if(this_trace>trace){
	maxcov=this_cov;
	trace=this_trace;
      }
    }
    cov+=maxcov;
  }else if(type==Systematic::Type::HESSIAN){
    for(const auto& var:variations){
      const TH1D* hist1=(TH1D*)var;
      TMatrixD val0(ifirst,ilast,0,0,hist0->GetArray()+ifirst);
      TMatrixD val(ifirst,ilast,0,0,hist1->GetArray()+ifirst);
      val-=val0;
      TMatrixD valT=val;
      valT.T();
      TMatrixD this_cov=val*valT;
      cov+=this_cov;
    }
  }else if(type==Systematic::Type::GAUSSIAN){
    for(const auto& var:variations){
      const TH1D* hist1=(TH1D*)var;
      TMatrixD val0(ifirst,ilast,0,0,hist0->GetArray()+ifirst);
      TMatrixD val(ifirst,ilast,0,0,hist1->GetArray()+ifirst);
      val-=val0;
      TMatrixD valT=val;
      valT.T();
      TMatrixD this_cov=val*valT;
      cov+=this_cov;
    }
    cov*=1./variations.size();
  }else if(type==Systematic::Type::CORRELATED){
    TMatrixD diff(ifirst,ilast,0,0);
    TMatrixD val0(ifirst,ilast,0,0,hist0->GetArray()+ifirst);
    for(const auto& var:variations){
      const TH1D* hist1=(TH1D*)var;
      TMatrixD val(ifirst,ilast,0,0,hist1->GetArray()+ifirst);
      diff+=val-val0;
    }
    TMatrixD diffT=diff;
    diffT.T();
    cov=diff*diffT;
  }else{
    PError((TString)"[Plotter::GetCov] Wrong Systematic::Type "+Form("%d",type));
  }
  return cov;
}
double Plotter::Trace(TMatrixD mat){
  double trace=0;
  if(mat.GetNrows()!=mat.GetNcols()){
    PError("Nrow!=Ncol");
  }
  for(int i=mat.GetRowLwb(),n=mat.GetRowUpb()+1;i<n;i++){
    trace+=mat[i][i];
  }
  return trace;
}
int Plotter::AddError(TH1* hist,TH1* sys) const {
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
      std::cout<<hist->GetName()<<" "<<hist->GetTitle()<<" "<<sys->GetName()<<" "<<sys->GetTitle()<<endl;
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
  if(!p.hists.size()) return;

  for(Hists& hists:p.hists){
    if(hists.entity){
      for(int i=0,n=hists.size();i<n;i++){
	hists.entity->ApplyStyle(hists.at(i),i);
      }
    }
  }

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

  EvalHistSyss(p);
  Blind(p);
  DrawHistograms(p);
  TH1* axisparent=GetAxisParent();
  axisparent->SetTickLength(0.015,"Y");
  double scale=1/TMath::Min(gPad->GetHNDC(),gPad->GetWNDC());
  axisparent->SetTitleSize(0.04*scale,"XYZ");
  axisparent->SetLabelSize(0.04*scale,"XYZ");
  if(p.option.Contains("xtitle++")) axisparent->SetTitleSize(axisparent->GetTitleSize("x")*1.5,"x");
  else if(p.option.Contains("xtitle+")) axisparent->SetTitleSize(axisparent->GetTitleSize("x")*1.2,"x");
  else if(p.option.Contains("xtitle--")) axisparent->SetTitleSize(axisparent->GetTitleSize("x")*0.6,"x");
  else if(p.option.Contains("xtitle-")) axisparent->SetTitleSize(axisparent->GetTitleSize("x")*0.8,"x");
  if(p.option.Contains("ytitle++")) axisparent->SetTitleSize(axisparent->GetTitleSize("y")*1.5,"y");
  else if(p.option.Contains("ytitle+")) axisparent->SetTitleSize(axisparent->GetTitleSize("y")*1.2,"y");
  else if(p.option.Contains("ytitle--")) axisparent->SetTitleSize(axisparent->GetTitleSize("y")*0.8,"y");
  else if(p.option.Contains("ytitle-")) axisparent->SetTitleSize(axisparent->GetTitleSize("y")*0.6,"y");
  axisparent->GetYaxis()->SetTitleOffset(1.8/scale);

  TLegend* legend=NULL;
  if(!p.option.Contains("noleg")) DrawLegend(p);
  if(p.option.Contains("sysleg")) DrawLegendSys(p);
  DrawTexts(p);

  if(p.option.Contains("logx")){
    gPad->SetLogx();
    axisparent->GetXaxis()->SetMoreLogLabels();
    if(axisparent->GetXaxis()->GetBinLowEdge(axisparent->GetXaxis()->GetFirst())==0){
      axisparent->GetXaxis()->SetRange(axisparent->GetXaxis()->GetFirst()+1,axisparent->GetXaxis()->GetLast());
    }
  }
  if(p.option.Contains("logy")){
    pair<double,double> minmax=GetMinMax(p.hists,"pos");
    double minimum=minmax.first,maximum=minmax.second;
    if(minimum<=0) minimum=maximum/1000;
    axisparent->GetYaxis()->SetRangeUser(minimum/20,maximum*20);
    gPad->SetLogy();
  }else{
    pair<double,double> minmax=GetMinMax(p.hists);
    double minimum=minmax.first,maximum=minmax.second;
    double range=fabs(maximum-minimum);
    double ymin=minimum/range<-0.01?minimum-0.1*range:0;
    double ymax=maximum+0.1*(maximum-ymin);
    axisparent->GetYaxis()->SetRangeUser(ymin,ymax);
  }
  if(p.ymin||p.ymax){
    axisparent->GetYaxis()->SetRangeUser(p.ymin,p.ymax);
  }
  axisparent->GetXaxis()->SetTitle(p.xtitle);
  axisparent->GetYaxis()->SetTitle(p.ytitle); 
  TString toptitle=p.title;
  if(p.sysname!="") toptitle+="_"+p.sysname;
  if(p.replace_old!=""||p.replace_new!="") TPRegexp(p.replace_old).Substitute(toptitle,p.replace_new);
  axisparent->SetTitle(toptitle);

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
vector<Hists> Plotter::GetRatioHists(Plot p){
  TH1* base=NULL;
  TString baseopt=p.option("base:[0-9]*");
  int ibase=0;
  if(baseopt!=""){
    ibase=((TString)baseopt(5,100)).Atoi();
  }else if(p.hists.size()==2){
    ibase=1;
  }else{
    ibase=0;
  }
  base=GetTH1(p.hists[ibase][0]);
    
  vector<Hists> hists_new;
  if(base){
    for(int i=0;i<base->GetNbinsX()+2;i++){
      base->SetBinError(i,0);
    }
    for(const Hists this_hists:p.hists){
      Hists this_hists_new=this_hists.Clone();
      this_hists_new.Divide(base);
      hists_new.push_back(move(this_hists_new));
    }  
    if(p.option.Contains("movesys")){
      if(entries.size()!=2){
	PError(Form("[Plotter::GetRatioHists] movesys fail entries.size()=%zu!=2",entries.size()));
      }
      Hists hists_temp=p.hists[1-ibase].Clone();
      hists_temp.Divide(p.hists[ibase]);
      hists_temp.Divide(hists_new[1-ibase].at(0));
      hists_new[1-ibase].variations.clear();
      hists_new[ibase].variations=hists_temp.variations;
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
  TH1* axisparent=GetAxisParent();
  if(axisparent){
    if(p.ytitle=="") axisparent->GetYaxis()->SetTitle("Ratio");
    if(p.ymin||p.ymax){
      axisparent->GetYaxis()->SetRangeUser(p.ymin,p.ymax);
    }else if(p.option.Contains("widewidewidey")){
      axisparent->GetYaxis()->SetRangeUser(0.01,4.99);
      axisparent->GetYaxis()->SetNdivisions(506);
    }else if(p.option.Contains("widewidey")){
      axisparent->GetYaxis()->SetRangeUser(0.01,1.99);
      axisparent->GetYaxis()->SetNdivisions(506);
    }else if(p.option.Contains("widey")){
      axisparent->GetYaxis()->SetRangeUser(0.501,1.499);
      axisparent->GetYaxis()->SetNdivisions(506);
    }else if(p.option.Contains("finefiney")){
      axisparent->GetYaxis()->SetRangeUser(0.951,1.049);
      axisparent->GetYaxis()->SetNdivisions(506);
    }else if(p.option.Contains("finey")){
      axisparent->GetYaxis()->SetRangeUser(0.901,1.099);
      axisparent->GetYaxis()->SetNdivisions(506);
    }else{
      axisparent->GetYaxis()->SetRangeUser(0.801,1.199);
      axisparent->GetYaxis()->SetNdivisions(504);
    }
  }
  _depth--;
}
void Plotter::DrawDiff(Plot p){
  PInfo("[Plotter::DrawDiff]"+p.option);_depth++;
  if(!gPad) gROOT->MakeDefCanvas();

  TH1* base=NULL;
  TString baseopt=p.option("base:[0-9]*");
  if(baseopt!="") base=GetTH1(p.hists[((TString)baseopt(5,100)).Atoi()][0]);
  else if(p.hists.size()==2) base=GetTH1(p.hists[1][0]);
  else base=GetTH1(p.hists[0][0]);  
  for(int i=0;i<base->GetNbinsX()+2;i++){
    base->SetBinError(i,0);
  }

  vector<Hists> hists_new;
  for(const vector<TH1*>& this_hists:p.hists){
    Hists this_hists_new;
    for(const auto& hist:this_hists){
      TH1* hist_new=NULL;
      if(hist){
	hist_new=GetTH1(hist);
	hist_new->Add(base,-1);
      }
      this_hists_new.push_back(hist_new);
    }
    hists_new.push_back(move(this_hists_new));
  }  
  delete base;

  p.hists=hists_new;
  DrawCompare(p);
  TH1* axisparent=GetAxisParent();
  if(axisparent){
    if(p.ytitle=="") axisparent->GetYaxis()->SetTitle("Difference");
    if(p.ymin||p.ymax){
      axisparent->GetYaxis()->SetRangeUser(p.ymin,p.ymax);
    }else{
      pair<double,double> minmax=GetMinMax(p.hists);
      double minimum=minmax.first,maximum=minmax.second;
      double range=fabs(maximum-minimum);
      axisparent->GetYaxis()->SetRangeUser(minimum/range<-0.01?minimum-0.1*range:0,maximum+0.1*range);
    }
  }
  _depth--;
}
void Plotter::DrawSig(Plot p){
  PInfo("[Plotter::DrawSig] "+p.option);_depth++;
  if(!gPad) gROOT->MakeDefCanvas();

  TH1* base=NULL;
  TString baseopt=p.option("base:[0-9]*");
  if(baseopt!="") base=GetTH1(p.hists[((TString)baseopt(5,100)).Atoi()].back());
  else if(p.hists.size()==2) base=GetTH1(p.hists[1].back());
  else if(p.hists.size()==1){
    base=GetTH1(p.hists[0].back());
    base->Reset();
  }else base=GetTH1(p.hists[0].back());

  TH1* sigma1=(TH1*)base->Clone("sigma1");
  sigma1->SetStats(0);
  sigma1->SetDirectory(pdir);
  sigma1->SetTitle("#pm 1 #sigma");
  sigma1->SetFillColor(kGreen);
  sigma1->SetFillStyle(3001);
  sigma1->SetMarkerSize(0);
  for(int i=0;i<sigma1->GetNbinsX()+2;i++){
    sigma1->SetBinContent(i,0);
    sigma1->SetBinError(i,1);
  }
  TH1* sigma2=(TH1*)sigma1->Clone("sigma2");
  sigma2->SetStats(0);
  sigma2->SetDirectory(pdir);
  for(int i=0;i<sigma2->GetNbinsX()+2;i++){
    sigma2->SetBinError(i,2);
  }
  sigma2->SetTitle("#pm 2 #sigma");
  sigma2->SetFillColor(kYellow);
  sigma2->Draw("e2");
  sigma1->Draw("same e2");  

  vector<Hists> hists_new;
  for(const vector<TH1*>& this_hists:p.hists){
    TH1* hist=NULL;
    for(int i=this_hists.size()-1;i>=0;i--){
      hist=this_hists.at(i);
      if(hist){
	hist=GetTH1(hist);
	for(int i=0;i<hist->GetNbinsX()+2;i++){
	  double content=0;
	  if(hist->GetBinError(i)||base->GetBinError(i)){
	    double error=sqrt(pow(hist->GetBinError(i),2)+pow(base->GetBinError(i),2));
	    content=(hist->GetBinContent(i)-base->GetBinContent(i))/error;
	  }
	  hist->SetBinContent(i,content);
	  if(content) hist->SetBinError(i,content*1e-30);
	  else hist->SetBinError(i,0);
	  if(p.blind_xmin||p.blind_xmax){
	    hist->SetBinContent(i,0);
	  }
	}
	break;
      }
    }
    Hists temp;
    temp.push_back(hist);
    hists_new.push_back(move(temp));
  }
  p.hists=hists_new;
  DrawCompare(p);
  TH1* axisparent=GetAxisParent();
  if(axisparent){
    if(p.ytitle=="") axisparent->GetYaxis()->SetTitle("Difference (#sigma)");
    axisparent->GetYaxis()->UnZoom();
    if(p.ymin||p.ymax){
      axisparent->GetYaxis()->SetRangeUser(p.ymin,p.ymax);
    }else if(p.option.Contains("widey")){
      axisparent->GetYaxis()->SetRangeUser(-7.99,7.99);
      axisparent->GetYaxis()->SetNdivisions(504);
    }else{
      axisparent->GetYaxis()->SetRangeUser(-3.99,3.99);
      axisparent->GetYaxis()->SetNdivisions(504);
    }
  }
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
  DrawCompare(p.GetSubPlot(1));
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
    axisparent->GetYaxis()->SetNdivisions(505);
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
  DrawCompare(p.GetSubPlot(1));
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
  DrawSig((p+"noleg").GetSubPlot(2));
  axisparent=GetAxisParent();
  if(axisparent) axisparent->SetTitle("");
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
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  gPad->SetTopMargin(pad->GetTopMargin()/0.65);
  DrawCompare(p.GetSubPlot(1));
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
  DrawDiff((p+"noleg").GetSubPlot(2));
  axisparent=GetAxisParent();
  if(axisparent){
    axisparent->SetTitle("");
    axisparent->GetYaxis()->SetNdivisions(305);
  }
  gPad->Update();
  gPad->Modified();
  _depth--;
}
void Plotter::DrawDoubleRatio(Plot& p){
  if(!gPad) gROOT->MakeDefCanvas();

  vector<vector<Hists>> vec_hists;
  for(auto& sub:p.subplots){
    vec_hists.push_back(GetRatioHists(p));
  }
  int baseindex=0;
  if(p.subplots.size()==2){
    baseindex=1;
    TString baseopt=p.option("base:[0-9]*");
    if(baseopt!="") baseindex=((TString)baseopt(5,100)).Atoi();
    for(int i=0,n=vec_hists[baseindex].size();i<n;i++){
      TH1* hist=vec_hists[baseindex][i][0];
      bool flag=false;
      for(int j=1;j<hist->GetNbinsX()+1;j++) if(hist->GetBinContent(j)!=1.) flag=true;
      if(!flag){
	for(int j=0,nj=vec_hists.size();j<nj;j++){
	  vec_hists[j].erase(vec_hists[j].begin()+i);
	}
      }
    }
    vector<Hists> hists_new;
    for(int i=0,ni=vec_hists[baseindex].size();i<ni;i++){
      TH1* base=(TH1*)vec_hists[baseindex][i][0]->Clone();
      for(int j=0;j<base->GetNbinsX()+2;j++) base->SetBinError(j,0);
      for(int j=0,nj=vec_hists.size();j<nj;j++){
	Hists this_hists_new;
	for(int k=0,nk=vec_hists[j][i].size();k<nk;k++){
	  if(vec_hists[j][i][k]) vec_hists[j][i][k]->Divide(base);
	  this_hists_new.push_back(vec_hists[j][i][k]);
	}
	hists_new.push_back(move(this_hists_new));
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
  if(entries.size()==0){
    PError("[Plotter::DrawPlot] Please SetupEntries ");
    _depth--;
    return NULL;
  }
  if(!pdir) pdir=new TDirectory("plotdir","plotdir");
  p.SetOption(additional_option);
  p.hists=GetHists(p);
  vector<TH1*> hists=VectorTH1(p.hists);
  if(hists.size()<entries.size()){
    PError("[Plotter::DrawPlot] #hists < #entries ");
    _depth--;
    return NULL;
  }
  TCanvas* c=gROOT->MakeDefCanvas();
  if(p.type==Plot::Type::Compare) DrawCompare(p);
  else if(p.type==Plot::Type::Ratio) DrawRatio(p);
  else if(p.type==Plot::Type::Diff) DrawDiff(p);
  else if(p.type==Plot::Type::Sig) DrawSig(p);
  else if(p.type==Plot::Type::CompareAndRatio) DrawCompareAndRatio(p);
  else if(p.type==Plot::Type::CompareAndDiff) DrawCompareAndDiff(p);
  else if(p.type==Plot::Type::CompareAndSig) DrawCompareAndSig(p);
  else if(p.type==Plot::Type::DoubleRatio) DrawDoubleRatio(p);
  if(p.option.Contains("preliminary")){
    TString era;
    if(p.era!="") era=p.era;
    else if(p.histname.Contains("2016a/")) era="2016preVFP";
    else if(p.histname.Contains("2016b/")) era="2016postVFP";
    else if(p.histname.Contains("2016[ab]/")) era="2016";
    else if(p.histname.Contains("2017/")) era="2017";
    else if(p.histname.Contains("2018/")) era="2018";
    else if(p.histname.Contains("201[6-8]/")||p.histname.Contains("201[678ab]+/")||p.histname.Contains("201[6-8][ab]?/")||p.histname.Contains("201[678][ab]?/")) era="Run2";
    DrawPreliminary(c,era,p.lumi,p.option);
    if(p.blind_xmin||p.blind_xmax){
      TLatex latex;
      latex.SetTextSize(0.035);
      latex.SetNDC();
      latex.SetTextAlign(21);
      latex.DrawLatex(0.5,1.05-c->GetTopMargin(),"#bf{#it{Blinded}}");
    }
  }
  if(p.option.Contains("chi2")){
    TH1D* hist0=(TH1D*)GetTH1(p.hists[0][0]);
    int ifirst=hist0->GetXaxis()->GetFirst();
    int ilast=hist0->GetXaxis()->GetLast();
    TMatrixD cov_syst(ifirst,ilast,ifirst,ilast);
    if(p.sysname!=""){
      if(p.hists.size()==1) cov_syst+=GetCov(p.hists[0],p.sysname);
      if(p.hists.size()==2) cov_syst+=GetCov(p.hists[0],p.hists[1],p.sysname);
    }
    TH1D* hist1=NULL;
    if(p.hists.size()>1){
      hist1=(TH1D*)GetTH1(p.hists[1][0]);
      if(p.hists[1].cov.GetNrows()!=0) cov_syst+=p.hists[1].cov;
    }else{
      hist1=(TH1D*)hist0->Clone();
      hist1->Reset();
    }

    TMatrixD cov_hist0_stat(ifirst,ilast,ifirst,ilast);
    TMatrixD cov_hist1_stat(ifirst,ilast,ifirst,ilast);
    for(int i=ifirst,n=ilast+1;i<n;i++){
      cov_hist0_stat[i][i]=pow(hist0->GetBinError(i),2);
      cov_hist1_stat[i][i]=pow(hist1->GetBinError(i),2);
    }

    TMatrixD cov(ifirst,ilast,ifirst,ilast);
    cov=cov_syst+cov_hist0_stat+cov_hist1_stat;

    double chi2=GetChi2(hist0,hist1,cov);
    //cout<<chi2<<endl;
    //cout<<GetChi2(hist0,hist1)<<endl;
    c->cd(1);
    TLatex latex;
    latex.SetTextSize(0.035/TMath::Min(gPad->GetHNDC(),gPad->GetWNDC()));
    latex.SetNDC();
    int ndf=ilast-ifirst+1;
    if(p.option.Contains("norm")) ndf--;
    latex.DrawLatex(gPad->GetLeftMargin()+0.01,1-gPad->GetTopMargin()-1.1*latex.GetTextSize(),Form("#chi^{2}/n_{dof} = %g / %d",chi2,ndf));
    if(p.option.Contains("pvalue")){
      latex.DrawLatex(gPad->GetLeftMargin()+0.01,1-gPad->GetTopMargin()-2.4*latex.GetTextSize(),Form("p = %g",TMath::Prob(chi2,ndf)));
    }
    if(p.option.Contains("chi2detail")){
      {
	int ncov=3;
	if(systematics[p.sysname].type==Systematic::Type::MULTI){
	  ncov=2+systematics[p.sysname].keys.size();
	}
	TString toptitle=p.title;
	if(p.sysname!="") toptitle+="_"+p.sysname;
	if(p.replace_old!=""||p.replace_new!="") TPRegexp(p.replace_old).Substitute(toptitle,p.replace_new);
	TH1* nominal=new TH1D("nominal",toptitle,ncov,0,ncov);
	nominal->SetDirectory(pdir);
	nominal->SetStats(0);
	nominal->GetYaxis()->SetTitle("#chi^{2}");
	nominal->GetXaxis()->SetLabelSize(nominal->GetXaxis()->GetLabelSize()*1.5);
	TH1* up=new TH1D("up","2#times",ncov,0,ncov);
	up->SetDirectory(pdir);
	up->SetLineColor(2);
	TH1* down=new TH1D("down","0#times",ncov,0,ncov);
	down->SetDirectory(pdir);
	down->SetLineColor(4);
	int icov=0;
	nominal->GetXaxis()->SetBinLabel(++icov,"data stat.");
	nominal->SetBinContent(icov,chi2);
	up->SetBinContent(icov,GetChi2(hist0,hist1,cov+3.*cov_hist0_stat));
	down->SetBinContent(icov,GetChi2(hist0,hist1,cov-cov_hist0_stat));
	nominal->GetXaxis()->SetBinLabel(++icov,"pred. stat.");
	nominal->SetBinContent(icov,chi2);
	up->SetBinContent(icov,GetChi2(hist0,hist1,cov+3.*cov_hist1_stat));
	down->SetBinContent(icov,GetChi2(hist0,hist1,cov-cov_hist1_stat));
	if(systematics[p.sysname].type!=Systematic::Type::MULTI){
	  nominal->GetXaxis()->SetBinLabel(++icov,"total syst.");
	  nominal->SetBinContent(icov,chi2);
	  up->SetBinContent(icov,GetChi2(hist0,hist1,cov+3.*cov_syst));
	  down->SetBinContent(icov,GetChi2(hist0,hist1,cov-cov_syst));
	}else{
	  for(const auto& key:systematics[p.sysname].keys){
	    TMatrixD this_cov=GetCov(p.hists[0],p.hists[1],key);
	    nominal->GetXaxis()->SetBinLabel(++icov,key);
	    nominal->SetBinContent(icov,chi2);
	    up->SetBinContent(icov,GetChi2(hist0,hist1,cov+3.*this_cov));
	    //down->SetBinContent(icov,GetChi2(hist0,hist1,cov-3./4.*this_cov));
	    down->SetBinContent(icov,GetChi2(hist0,hist1,cov-this_cov));
	  }
	}
	nominal->GetXaxis()->LabelsOption("v");

	TCanvas* cchi2=new TCanvas(c->GetName()+TString("_chi2"));
	cchi2->SetBottomMargin(0.3);
	nominal->Draw("hist");
	pair<double,double> minmax=GetMinMax({nominal,up,down});
	nominal->GetYaxis()->SetRangeUser(minmax.first*0.8,minmax.second*1.2);
	up->Draw("same hist");
	down->Draw("same hist");
	TLegend *leg=new TLegend(0.89,0.89,0.7,0.7);
	leg->AddEntry(nominal,"nominal");
	leg->AddEntry(up);
	leg->AddEntry(down);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->Draw();
	c->cd();
      }
      {
	TCanvas* cchi2impact=new TCanvas(c->GetName()+TString("_chi2impact"));
	TH1* hchi2=(TH1*)hist0->Clone("hchi2");
	hchi2->SetStats(0);
	hchi2->Reset();
	hchi2->SetDirectory(pdir);
	for(int i=hchi2->GetXaxis()->GetFirst();i<hchi2->GetXaxis()->GetLast()+1;i++){
	  hchi2->SetBinContent(i,GetChi2(hist0,hist1,cov,i));
	}
	hchi2->GetYaxis()->SetRangeUser(ndf*0.5,ndf*3);
	hchi2->Draw("hist");
	cchi2impact->SetGridx();
	if(p.option.Contains("logx")){
	  cchi2impact->SetLogx();
	}
	c->cd();	
      }
    }
    if(hist0) delete hist0;
    if(hist1) delete hist1;
  }
  c->Update();
  c->Modified();
  _depth--;
  return c;
}
TCanvas* Plotter::DrawPlot(vector<TH1*> hists,TString additional_option){
  Plot p=MakePlot("",additional_option);
  if(!pdir) pdir=new TDirectory("plotdir","plotdir");
  p.hists=vector<Hists>();
  for(auto h:hists){
    if(!h) continue;
    Hists hh;
    hh.push_back(h);
    p.hists.push_back(hh);
  }
  TCanvas* c=gROOT->MakeDefCanvas();
  if(p.type==Plot::Type::Compare) DrawCompare(p);
  else if(p.type==Plot::Type::Ratio) DrawRatio(p);
  else if(p.type==Plot::Type::Diff) DrawDiff(p);
  else if(p.type==Plot::Type::Sig) DrawSig(p);
  else if(p.type==Plot::Type::CompareAndRatio) DrawCompareAndRatio(p);
  else if(p.type==Plot::Type::CompareAndDiff) DrawCompareAndDiff(p);
  else if(p.type==Plot::Type::CompareAndSig) DrawCompareAndSig(p);
  else if(p.type==Plot::Type::DoubleRatio) DrawDoubleRatio(p);
  if(p.option.Contains("preliminary")){
    TString era;
    if(p.era!="") era=p.era;
    else if(p.histname.Contains("2016a/")) era="2016preVFP";
    else if(p.histname.Contains("2016b/")) era="2016postVFP";
    else if(p.histname.Contains("2016[ab]/")) era="2016";
    else if(p.histname.Contains("2017/")) era="2017";
    else if(p.histname.Contains("2018/")) era="2018";
    else if(p.histname.Contains("201[6-8]/")||p.histname.Contains("201[678ab]+/")||p.histname.Contains("201[6-8][ab]?/")||p.histname.Contains("201[678][ab]?/")) era="Run2";
    DrawPreliminary(c,era,p.lumi,p.option);
    if(p.blind_xmin||p.blind_xmax){
      TLatex latex;
      latex.SetTextSize(0.035);
      latex.SetNDC();
      latex.SetTextAlign(21);
      latex.DrawLatex(0.5,1.05-c->GetTopMargin(),"#bf{#it{Blinded}}");
    }
  }
  if(p.option.Contains("chi2")){
    TH1D* hist0=(TH1D*)GetTH1(p.hists[0][0]);
    int ifirst=hist0->GetXaxis()->GetFirst();
    int ilast=hist0->GetXaxis()->GetLast();
    TMatrixD cov_syst(ifirst,ilast,ifirst,ilast);
    if(p.sysname!=""){
      if(p.hists.size()==1) cov_syst+=GetCov(p.hists[0],p.sysname);
      if(p.hists.size()==2) cov_syst+=GetCov(p.hists[0],p.hists[1],p.sysname);
    }
    TH1D* hist1=NULL;
    if(p.hists.size()>1){
      hist1=(TH1D*)GetTH1(p.hists[1][0]);
      if(p.hists[1].cov.GetNrows()!=0) cov_syst+=p.hists[1].cov;
    }else{
      hist1=(TH1D*)hist0->Clone();
      hist1->Reset();
    }

    TMatrixD cov_hist0_stat(ifirst,ilast,ifirst,ilast);
    TMatrixD cov_hist1_stat(ifirst,ilast,ifirst,ilast);
    for(int i=ifirst,n=ilast+1;i<n;i++){
      cov_hist0_stat[i][i]=pow(hist0->GetBinError(i),2);
      cov_hist1_stat[i][i]=pow(hist1->GetBinError(i),2);
    }

    TMatrixD cov(ifirst,ilast,ifirst,ilast);
    cov=cov_syst+cov_hist0_stat+cov_hist1_stat;

    double chi2=GetChi2(hist0,hist1,cov);
    //cout<<chi2<<endl;
    //cout<<GetChi2(hist0,hist1)<<endl;
    c->cd(1);
    TLatex latex;
    latex.SetTextSize(0.035/TMath::Min(gPad->GetHNDC(),gPad->GetWNDC()));
    latex.SetNDC();
    int ndf=ilast-ifirst+1;
    if(p.option.Contains("norm")) ndf--;
    latex.DrawLatex(gPad->GetLeftMargin()+0.01,1-gPad->GetTopMargin()-1.1*latex.GetTextSize(),Form("#chi^{2}/n_{dof} = %g / %d",chi2,ndf));
    if(p.option.Contains("pvalue")){
      latex.DrawLatex(gPad->GetLeftMargin()+0.01,1-gPad->GetTopMargin()-2.4*latex.GetTextSize(),Form("p = %g",TMath::Prob(chi2,ndf)));
    }
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
  PDebug("[Plotter::SavePlot(TString plotkey,TString option,bool delete_canvas=true)]");
  Plot p=MakePlot(plotkey,option);
  TCanvas* c=DrawPlot(p);
  vector<TString> saves=Split(p.save,",");
  TString defaultname=p.name;
  TPRegexp("[/),(]").Substitute(defaultname,"_","g");
  defaultname.ReplaceAll("[","_");
  defaultname.ReplaceAll("]","_");
  for(auto save:saves){
    TString path=save;
    if(save==""||save.Contains(TRegexp("^\\.?pdf$"))||save.Contains(TRegexp("^\\.?png$"))){
      path=defaultname;
    }
    if(save.Contains(TRegexp("^\\.?pdf$"))) path+=".pdf";
    else if(save.Contains(TRegexp("^\\.?png$"))) path+=".png";
    if(!Basename(path).Contains(".")){
      path+=".png";
    }
    defaultname=path(0,path.Last('.'));
    SaveCanvas(c,path,false);
    TCanvas *ctemp=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(c->GetName()+TString("_chi2"));
    if(p.option.Contains("chi2detail")&&c&&ctemp){
      SaveCanvas(ctemp,path(0,path.Last('.'))+"_chi2"+path(path.Last('.'),path.Length()),false);
      if(delete_canvas) delete ctemp;
    }
    ctemp=(TCanvas*)gROOT->GetListOfCanvases()->FindObject(c->GetName()+TString("_chi2impact"));
    if(p.option.Contains("chi2detail")&&c&&ctemp){
      SaveCanvas(ctemp,path(0,path.Last('.'))+"_chi2impact"+path(path.Last('.'),path.Length()),false);
      if(delete_canvas) delete ctemp;
    }
  }
  if(delete_canvas){
    delete c;
    if(pdir) pdir->Delete();
  }
}
void Plotter::SavePlotCondor(TString plotkey,TString option){
  plotkey=ParseForCondorRun(plotkey);
  option=ParseForCondorRun(option);
  TString classname=ClassName();
  system("condor_run -a jobprio=1 -a jobbatchname="+classname+" bash -c \\\"cd $PWD\\;export ROOT_HIST=0\\;echo -e \\'\\#include\\\\\\\""+classname+".cc\\\\\\\"\\\\n"+classname+" plotter\\;\\\\nplotter."+GetSetupStringForCondor()+"\\;\\\\nplotter.plotdir=\\\\\\\""+plotdir+"\\\\\\\"\\;\\\\nplotter.SavePlot\\(\\\\\\\""+plotkey+"\\\\\\\",\\\\\\\""+option+"\\\\\\\"\\)\\\\n.q\\'\\|root -l -b\\\" 2>&1 |grep -v MakeDefCanvas &");
  system("sleep 0.02");
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
////////////////////////////// Table ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Plotter::PrintTable(int ientry,TString plotkey,TString option){
  Plot p=MakePlot(plotkey,option);
  Hists hists=GetHistVariations(entries[ientry],p);
  EvalHistSys(hists,p.sysname,p.option+" notaddsys sysdetail");
  
  if(hists.size()==0||!hists[0]) return;
  TH1* hist=GetTH1(hists[0]);
  cout<<"bin\t";
  for(int j=0,nj=hists.size();j<nj;j++){
    if(j==0) cout<<hists[j]->GetName()<<" stat ";
    else cout<<hists[j]->GetName()<<" ";
  }
  cout<<endl;
  for(int i=hist->GetXaxis()->GetFirst();i<hist->GetXaxis()->GetLast()+1;i++){
    cout<<hist->GetXaxis()->GetBinLowEdge(i)<<"<=x<"<<hist->GetXaxis()->GetBinLowEdge(i+1)<<" ";
    for(int j=0,nj=hists.size();j<nj;j++){
      double scale=1.;
      if(option.Contains("percent")) scale=fabs(100./hists[j]->GetBinContent(i));
      else if(option.Contains("relerr")) scale=fabs(1/hists[j]->GetBinContent(i));
      if(j==0){
	cout<<hists[j]->GetBinContent(i)<<" "<<hists[j]->GetBinError(i)*scale<<" ";
      }else{
	cout<<hists[j]->GetBinError(i)*scale<<" ";
      }
    }
    cout<<endl;
  }
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
TH1* Plotter::GetTH1(const TH1* hstack,bool deleteorigin) const{
  PAll("[Plotter::GetTH1(TH1* hstack)]");
  TH1* hist=NULL;
  if(hstack){
    if(hstack->InheritsFrom("THStack")){
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
vector<TH1*> Plotter::VectorTH1(const vector<Hists>& hists){
  PAll("[Plotter::VectorTH1(const vector<Hists>& hists)]");
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
      for(int i=hist->GetXaxis()->GetFirst(),n=hist->GetXaxis()->GetLast()+1;i<n;i++){
	double content=hist->GetBinContent(i);
	if(!option.Contains("pos")){
	  if(maximum<content) maximum=content;
	  if(minimum>content) minimum=content;
	}else{
	  if(maximum<content && content>0) maximum=content;
	  if(minimum>content && content>0) minimum=content;
	}
      }	
    }
  }
  return make_pair(minimum,maximum);
}
pair<double,double> Plotter::GetMinMax(const vector<Hists>& hists,TString option) const{
  return GetMinMax(VectorTH1(hists),option);
}
void Plotter::RebinXminXmax(TH1* hist,Plot p){
  PAll("[Plotter::RebinXminXmax]");
  if(hist){
    if(hist->InheritsFrom("THStack")){
      for(const auto& obj:*((THStack*)hist)->GetHists())
	RebinXminXmax((TH1*)obj,p);
    // }else if(hist->InheritsFrom("TH4D")){
    //   if(p.rebin!=""){
    // 	PError((TString)"[Plotter::RebinXminXmax] Unsupported class for rebin "+hist->ClassName());
    //   }
    //   if(p.xmin!=0||p.xmax!=0)
    // 	hist->GetXaxis()->SetRangeUser(p.xmin,p.xmax);
    }else if(hist->InheritsFrom("TH3")){
      if(p.rebin!=""){
	PError((TString)"[Plotter::RebinXminXmax] Unsupported class for rebin "+hist->ClassName());
      }
      if(p.xmin!=0||p.xmax!=0)
	hist->GetXaxis()->SetRangeUser(p.xmin,p.xmax);
    }else if(hist->InheritsFrom("TH2")){
      if(p.rebin!=""){
	PError((TString)"[Plotter::RebinXminXmax] Unsupported class for rebin "+hist->ClassName());
      }
      if(p.xmin!=0||p.xmax!=0)
	hist->GetXaxis()->SetRangeUser(p.xmin,p.xmax);
    }else if(strstr(hist->ClassName(),"TH1")||strstr(hist->ClassName(),"TProfile")){
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
	  for(int i=1;i<hist->GetNbinsX()+1;i++){
	    if(hist->GetBinContent(i)&&hist->GetBinError(i)){
	      start=i;
	      break;
	    }
	  }
	  for(int i=hist->GetNbinsX();i>=1;i--){
	    if(hist->GetBinContent(i)&&hist->GetBinError(i)){
	      end=i;
	      break;
	    }
	  }
	  if(start<end) hist->GetXaxis()->SetRange(start,end);
	}
      }else hist->GetXaxis()->SetRangeUser(p.xmin,p.xmax);
      if(p.xdivision!=0){
	hist->GetXaxis()->SetNdivisions(p.xdivision,false);
      }
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
vector<vector<double>> Plotter::Normalize(vector<Hists> histss,double val,TString option){
  vector<vector<double>> scaless;
  for(auto& hists:histss){
    vector<double> scales=Normalize(hists,val,option);
    scaless.push_back(scales);
    for(auto& [suffix,hist]:hists.variations){
      if(hist){
	hist->Scale(scales.at(0));
      }
    }
  }
  return scaless;
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
  maxheight=0.05;
  if(maxwidth<0.1) maxwidth=0.1;
  if(maxwidth>0.5) maxwidth=0.5;
  int entry_size=hists.size();
  if(entry_size<3) entry_size=3;
  if(entry_size>7) entry_size=7;
  
  vector<double> coordinates;
  double width=(maxwidth+0.03)/pad->GetWNDC();
  if(p.option.Contains("legwwwww")) width *= 0.5;
  else if(p.option.Contains("legwwww")) width *= 0.6;
  else if(p.option.Contains("legwww")) width *= 0.7;
  else if(p.option.Contains("legww")) width *= 0.8;
  else if(p.option.Contains("legw")) width *= 0.9;
  double height=(maxheight*entry_size)/pad->GetHNDC();
  if(p.option.Contains("leghhhhh")) height *= 0.5;
  else if(p.option.Contains("leghhhh")) height *= 0.6;
  else if(p.option.Contains("leghhh")) height *= 0.7;
  else if(p.option.Contains("leghh")) height *= 0.8;
  else if(p.option.Contains("legh")) height *= 0.9;

  if(p.option.Contains("topleftleg")||p.option.Contains("TLleg")) coordinates={pad->GetLeftMargin()+0.01,1-pad->GetTopMargin()-0.01,pad->GetLeftMargin()+0.01+width,1-pad->GetTopMargin()-0.01-height};
  else if(p.option.Contains("topmiddleleg")||p.option.Contains("TMleg")) coordinates={0.5-0.5*width,1-pad->GetTopMargin()-0.01,0.5+0.5*width,1-pad->GetTopMargin()-0.01-height};
  else if(p.option.Contains("middleleftleg")||p.option.Contains("MLleg")) coordinates={pad->GetLeftMargin()+0.01, 0.5-0.5*height, pad->GetLeftMargin()+0.01+width, 0.5+0.5*height};
  else if(p.option.Contains("middlemiddleleg")||p.option.Contains("MMleg")) coordinates={0.5-0.5*width, 0.5-0.5*height, 0.5+0.5*width, 0.5+0.5*height};
  else if(p.option.Contains("middlerightleg")||p.option.Contains("MRleg")) coordinates={1-pad->GetRightMargin()-0.01, 0.5-0.5*height, 1-pad->GetRightMargin()-width, 0.5+0.5*height};
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
TLegend* Plotter::DrawLegendSys(const Plot& p){
  PAll("[Plotter::DrawLegendSys(const Plot& p)]");
  vector<pair<TString,TH1*>> hists;
  for(const auto& this_hists:p.hists){
    for(int i=0,n=this_hists.size();i<n;i++){
      TH1* hist=this_hists.at(i);
      if(hist){
	if(i==0) hists.push_back(make_pair(TString()+hist->GetName()+" stat.",hist));
	else hists.push_back(make_pair(TString()+"#oplus "+hist->GetName(),hist));
      }
    }
  }
  TVirtualPad* pad=gPad;
  TCanvas* c=pad->GetCanvas();
  double maxwidth=0;
  double maxheight=0;
  c->cd();
  for(const auto& [title,hist]:hists){
    TLatex latex(-1,-1,title);
    latex.SetTextSize(0.04);
    UInt_t w,h;
    latex.GetBoundingBox(w,h);
    if(1.0*w/c->GetWw()>maxwidth) maxwidth=1.0*w/c->GetWw();
    if(1.0*h/c->GetWh()>maxheight) maxheight=1.0*h/c->GetWh();
  }
  double height=maxheight/pad->GetHNDC();

  TLegend* legend=new TLegend(pad->GetLeftMargin()+0.01,1-pad->GetTopMargin()-0.01,1-pad->GetRightMargin()-0.01,1-pad->GetTopMargin()-0.01-height);
  legend->SetNColumns(hists.size());
  for(const auto& [title,hist]:hists) legend->AddEntry(hist,title);
  legend->SetBorderSize(0);
  pad->cd();
  legend->Draw();
  return legend;
}
void Plotter::DrawPreliminary(TPad *c,TString era,TString lumi,TString option){
  if(!c) return;
  c->cd(1);
  TH1* axisparent=GetAxisParent();
  if(axisparent) axisparent->SetTitle("");
  c->cd();
  axisparent=GetAxisParent();
  if(axisparent) axisparent->SetTitle("");
  TLatex latex;
  latex.SetTextSize(0.035);
  latex.SetNDC();
  latex.SetTextAlign(11);
  double leftmargin=c->GetLeftMargin();
  double rightmargin=c->GetRightMargin();
  double topmargin=c->GetTopMargin();
  latex.DrawLatex(0.01+leftmargin,1.01-topmargin,"CMS #bf{#it{Preliminary}}");
  if(!option.Contains("nolumi")){
    latex.SetTextAlign(31);
    if(lumi!=""){
      latex.DrawLatex(1-rightmargin,1.01-topmargin,lumi+" fb^{-1} (13 TeV)");
    }else if(era=="2016preVFP"){
      latex.DrawLatex(1-rightmargin,1.01-topmargin,"19.5 fb^{-1} (13 TeV)");
    }else if(era=="2016postVFP"){
      latex.DrawLatex(1-rightmargin,1.01-topmargin,"16.8 fb^{-1} (13 TeV)");
    }else if(era=="2016"){
      latex.DrawLatex(1-rightmargin,1.01-topmargin,"36.3 fb^{-1} (13 TeV)");
    }else if(era=="2017"){
      latex.DrawLatex(1-rightmargin,1.01-topmargin,"41.5 fb^{-1} (13 TeV)");
    }else if(era=="2018"){
      latex.DrawLatex(1-rightmargin,1.01-topmargin,"59.8 fb^{-1} (13 TeV)");
    }else if(era=="Run2"){
      latex.DrawLatex(1-rightmargin,1.01-topmargin,"138 fb^{-1} (13 TeV)");      
    }
  }
  if(!option.Contains("noera")&&era!=""){
    latex.SetTextAlign(31);
    if(era=="2016preVFP"){
      latex.DrawLatex(1-rightmargin,1.05-topmargin,"Run 2016preVFP");
    }else if(era=="2016postVFP"){
      latex.DrawLatex(1-rightmargin,1.05-topmargin,"Run 2016postVFP");
    }else if(era=="2016"){
      latex.DrawLatex(1-rightmargin,1.05-topmargin,"Run 2016");
    }else if(era=="2017"){
      latex.DrawLatex(1-rightmargin,1.05-topmargin,"Run 2017");
    }else if(era=="2018"){
      latex.DrawLatex(1-rightmargin,1.05-topmargin,"Run 2018");
    }else if(era=="Run2"){
      latex.DrawLatex(1-rightmargin,1.05-topmargin,"Run II");
    }
  }
  latex.SetTextColor(2);
  latex.SetTextAlign(21);
  latex.DrawLatex(0.5*(leftmargin-rightmargin)+0.5,1.01-topmargin,"#it{Working in progress}");
  c->Update();
  return;
}
void Plotter::DrawTexts(const Plot& p){
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.035/TMath::Min(gPad->GetHNDC(),gPad->GetWNDC()));
  for(auto [x,y,t]:p.texts){
    latex.DrawLatex(x,y,t);
  }
  return;
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
  if(sample.IsFile()){
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
void Plotter::AddSystematic(TString key,TString title,Systematic::Type type,vector<TString> includes,TString tag){
  auto it=systematics.find(key);
  if(it==systematics.end()) systematics[key]=Systematic(title,type,includes,tag);
  else PError("systematic "+key+" already exists");
}
void Plotter::AddSystematic(TString key,TString title,Systematic::Type type,TString includes,TString tag){
  return AddSystematic(key,title,type,Split(includes," "),tag);
}


///////////////////////////////////////////////////////////////////////
///////////////////// etc /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
TString Plotter::ClassName() const {
  TString raw=typeid(*this).name();
  return gSystem->GetFromPipe("c++filt -t "+raw);
}
int Plotter::GetAutoColor() const {
  set<int> colors={1,2,3,4,5,6,7,8,9};
  for(const auto& color:colors){
    bool exist=false;
    for(const auto& entry:entries){
      if(entry.styles[0].linecolor==color){
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
double Plotter::GetChi2(const TH1D* hist0,const TH1D* hist1,TMatrixD cov) const {
  int ifirst=hist0->GetXaxis()->GetFirst();
  int ilast=hist0->GetXaxis()->GetLast();
  TMatrixD val0(ifirst,ilast,0,0,hist0->GetArray()+ifirst);
  TMatrixD val(ifirst,ilast,0,0,hist1->GetArray()+ifirst);
  val-=val0;
  TMatrixD valT=val;
  valT.T();
  TMatrixD covI=cov;
  covI.Invert();
  TMatrixD chi2mat=valT*covI*val;
  double chi2=Trace(chi2mat);
  return chi2;
}
double Plotter::GetChi2(const TH1D* hist0,const TH1D* hist1,TMatrixD cov,int mask) const {
  int ifirst=hist0->GetXaxis()->GetFirst();
  int ilast=hist0->GetXaxis()->GetLast();
  TMatrixD val0(ifirst,ilast,0,0,hist0->GetArray()+ifirst);
  TMatrixD val(ifirst,ilast,0,0,hist1->GetArray()+ifirst);
  val-=val0;
  if(mask>=ifirst&&mask<=ilast){
    for(int i=ifirst;i<ilast+1;i++){
      cov[i][mask]=0;
      cov[mask][i]=0;
    }
    val[mask][0]=0.;
    cov[mask][mask]=1.;
  }
  TMatrixD valT=val;
  valT.T();
  TMatrixD covI=cov;
  covI.Invert();
  TMatrixD chi2mat=valT*covI*val;
  double chi2=Trace(chi2mat);
  return chi2;
}
TString Plotter::ParseForCondorRun(TString str) const {
  str.ReplaceAll("'","\\\'\\\\\\\'\\\'");
  for(TString a:{"<",">","(",")","#","{","}"}) str.ReplaceAll(a,"\\"+a);
  return str;
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
