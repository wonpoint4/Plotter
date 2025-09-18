#ifndef __AFBPLOTTER_CC__
#define __AFBPLOTTER_CC__
#include"SKFlatPlotter.cc"
#if __has_include("TH4D.h")
#include "TH4D.h"
#endif
#if __has_include("dybAnalyzer.h")
#include "dybAnalyzer.h"
#endif
class dybPlotter:public SKFlatPlotter{
public:
  //dybPlotter(TString mode_="data",TString analyzer_="dybAnalyzer");
  dybPlotter(TString mode_="data ^dyb_mi+dybbar_mi+mi+dyc_mi+dycbar_mi+dyudsg_mi+tau_mi+vv+tt+tw+st+wjets+ss_mi+aa",TString analyzer_="dybAnalyzer");
  ~dybPlotter();
  virtual void SetupEntries(TString mode_);
  virtual void SetupTH4D();

  virtual pair<double,double> GetRange(TString histname,TString axisname);
  virtual pair<double,double> GetAFBAndError(TH1* costhetaCS);

  using Plotter::GetHist;
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual TH1* GetHist(int ientry,TString plotkey,TString additional_option="") {return Plotter::GetHist(ientry,plotkey,additional_option); } // for pyroot compatibility
  virtual void GetHistActionForAdditionalClass(TObject*& obj,Plot plot);
  virtual Plot MakePlot(TString name,TString option="");

  virtual TH1* GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den);
  virtual TH1* GetHistAFB(TH1* hist_forward,TH1* hist_backward);

  //TCanvas* DrawPlot(TString plotkey,TString option="");

  ClassDef(dybPlotter,0);
};
ClassImp(dybPlotter);
pair<double,double> dybPlotter::GetRange(TString histname,TString axisname){
  TString rangestring=histname(TRegexp(axisname+"[[-+0-9.]*,[-+0-9.]*]"));
  TString first=rangestring(axisname.Length()+1,rangestring.Index(',')-axisname.Length()-1);
  TString second=rangestring(rangestring.Index(',')+1,rangestring.Length()-rangestring.Index(',')-2);
  return make_pair(first.Atof(),second.Atof());
}  

TH1* dybPlotter::GetHist(const Sample& sample,Plot plot,TString additional_option){
  TH1* hist=NULL;
  plot.SetOption(additional_option);
  for(auto [reg,newstr]:sample.replace)
    if(reg!="^"&&reg!="$"&&reg!="/([^/]*)$")
      TPRegexp(reg).Substitute(plot.histname,newstr);
  if(plot.option.Contains("doublediff")){
    TH1* hist_data_ee=GetHist(samples["data"],plot-"doublediff","histname:"+Replace(plot.histname,"LL201","ee201"));
    TH1* hist_data_mm=GetHist(samples["data"],plot-"doublediff","histname:"+Replace(plot.histname,"LL201","mm201"));    
    TH1* hist_sim_ee=GetHist(sample,plot-"doublediff","histname:"+Replace(plot.histname,"LL201","ee201"));
    TH1* hist_sim_mm=GetHist(sample,plot-"doublediff","histname:"+Replace(plot.histname,"LL201","mm201"));
    hist_data_mm->Add(hist_sim_mm,-1);
    hist_data_ee->Add(hist_sim_ee,-1);
    hist_data_mm->Add(hist_data_ee,-1);
    return hist_data_mm;
  }else if(plot.histname.Contains("weightedAFB")){
    TH1* hist_forward_num=GetHist(sample,plot,"histname:"+Replace(plot.histname,"weightedAFB","costhetaCS_num")+" Umin:0 Umax:1");
    TH1* hist_forward_den=GetHist(sample,plot,"histname:"+Replace(plot.histname,"weightedAFB","costhetaCS_den")+" Umin:0 Umax:1");
    TH1* hist_backward_num=GetHist(sample,plot,"histname:"+Replace(plot.histname,"weightedAFB","costhetaCS_num")+" Umin:-1 Umax:0");
    TH1* hist_backward_den=GetHist(sample,plot,"histname:"+Replace(plot.histname,"weightedAFB","costhetaCS_den")+" Umin:-1 Umax:0");
    hist=GetHistWeightedAFB(hist_forward_num,hist_forward_den,hist_backward_num,hist_backward_den);
  }else if(plot.histname.Contains("AFBrecoil")){
    TH1* hist_forward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFBrecoil","costhetaRecoil")+" Umin:0 Umax:1");
    TH1* hist_backward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFBrecoil","costhetaRecoil")+" Umin:-1 Umax:0");
    hist=GetHistAFB(hist_forward,hist_backward);
  }else if(plot.histname.Contains("AFBR")){
    TH1* hist_forward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFBR","costhetaR")+" Umin:0 Umax:1");
    TH1* hist_backward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFBR","costhetaR")+" Umin:-1 Umax:0");
    hist=GetHistAFB(hist_forward,hist_backward);
  }else if(plot.histname.Contains("AFBT")){
    TH1* hist_forward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFBT","costhetaT")+" Umin:0 Umax:1");
    TH1* hist_backward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFBT","costhetaT")+" Umin:-1 Umax:0");
    hist=GetHistAFB(hist_forward,hist_backward);
  }else if(plot.histname.Contains("AFB")){
    TH1* hist_forward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFB","costhetaCS")+" Umin:0 Umax:1");
    TH1* hist_backward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFB","costhetaCS")+" Umin:-1 Umax:0");
    hist=GetHistAFB(hist_forward,hist_backward);
  }
  if(hist){
    sample.ApplyStyle(hist);
    hist->SetTitle(plot.title);
    hist->SetDirectory(pdir);
    return hist;
  }else{
    return Plotter::GetHist(sample,plot);
  }
}
void dybPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot){
  PAll("[dybPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)");
  if(strstr(obj->ClassName(),"TH4D")!=NULL){
    if(plot.option.Contains("noproject")) return;
    TH4D* hist4d=(TH4D*)obj;
    int ixmin=0,iymin=0,izmin=0,iumin=0;
    int ixmax=-1,iymax=-1,izmax=-1,iumax=-1;
    if(plot.Xmin||plot.Xmax){
      ixmin=hist4d->GetXaxis()->FindBin(plot.Xmin);
      ixmax=hist4d->GetXaxis()->FindBin(plot.Xmax-0.00001);
      if(fabs(plot.Xmin-hist4d->GetXaxis()->GetBinLowEdge(ixmin))>0.00001)
	PWarning(Form("[Plotter::GetHistFromSample] Xmin=%f is not exact edge... use %f",plot.Xmin,hist4d->GetXaxis()->GetBinLowEdge(ixmin)));
      if(fabs(plot.Xmax-hist4d->GetXaxis()->GetBinUpEdge(ixmax))>0.00001)
	PWarning(Form("[Plotter::GetHistFromSample] Xmax=%f is not exact edge... use %f",plot.Xmax,hist4d->GetXaxis()->GetBinUpEdge(ixmax)));
    }
    if(plot.Ymin||plot.Ymax){
      iymin=hist4d->GetYaxis()->FindBin(plot.Ymin);
      iymax=hist4d->GetYaxis()->FindBin(plot.Ymax-0.00001);
      if(fabs(plot.Ymin-hist4d->GetYaxis()->GetBinLowEdge(iymin))>0.00001)
	PWarning(Form("[Plotter::GetHistFromSample] Ymin=%f is not exact edge... use %f",plot.Ymin,hist4d->GetYaxis()->GetBinLowEdge(iymin)));
      if(fabs(plot.Ymax-hist4d->GetYaxis()->GetBinUpEdge(iymax))>0.00001)
	PWarning(Form("[Plotter::GetHistFromSample] Ymax=%f is not exact edge... use %f",plot.Ymax,hist4d->GetYaxis()->GetBinUpEdge(iymax)));
    }
    if(plot.Zmin||plot.Zmax){
      izmin=hist4d->GetZaxis()->FindBin(plot.Zmin);
      izmax=hist4d->GetZaxis()->FindBin(plot.Zmax-0.00001);
      if(fabs(plot.Zmin-hist4d->GetZaxis()->GetBinLowEdge(izmin))>0.00001)
	PWarning(Form("[Plotter::GetHistFromSample] Zmin=%f is not exact edge... use %f",plot.Zmin,hist4d->GetZaxis()->GetBinLowEdge(izmin)));
      if(fabs(plot.Zmax-hist4d->GetZaxis()->GetBinUpEdge(izmax))>0.00001)
	PWarning(Form("[Plotter::GetHistFromSample] Zmax=%f is not exact edge... use %f",plot.Zmax,hist4d->GetZaxis()->GetBinUpEdge(izmax)));
    }
    if(plot.Umin||plot.Umax){
      iumin=hist4d->GetUaxis()->FindBin(plot.Umin);
      iumax=hist4d->GetUaxis()->FindBin(plot.Umax-0.00001);
      if(fabs(plot.Umin-hist4d->GetUaxis()->GetBinLowEdge(iumin))>0.00001)
	PWarning(Form("[Plotter::GetHistFromSample] Umin=%f is not exact edge... use %f",plot.Umin,hist4d->GetUaxis()->GetBinLowEdge(iumin)));
      if(fabs(plot.Umax-hist4d->GetUaxis()->GetBinUpEdge(iumax))>0.00001)
	PWarning(Form("[Plotter::GetHistFromSample] Umax=%f is not exact edge... use %f",plot.Umax,hist4d->GetUaxis()->GetBinUpEdge(iumax)));
    }
    if(plot.project=="") obj=(TObject*)hist4d->ProjectionU("_pu",ixmin,ixmax,iymin,iymax,izmin,izmax);
    else if(plot.project=="x") obj=(TObject*)hist4d->ProjectionX("_px",iymin,iymax,izmin,izmax,iumin,iumax);
    else if(plot.project=="y") obj=(TObject*)hist4d->ProjectionY("_py",ixmin,ixmax,izmin,izmax,iumin,iumax);
    else if(plot.project=="z") obj=(TObject*)hist4d->ProjectionZ("_pz",ixmin,ixmax,iymin,iymax,iumin,iumax);
    else if(plot.project=="u") obj=(TObject*)hist4d->ProjectionU("_pu",ixmin,ixmax,iymin,iymax,izmin,izmax);
    //else if(plot.project=="xz") obj=(TObject*)hist4d->ProjectionXZ("_pxz",iymin,iymax,iumin,iumax);
    else{
      PError("[Plotter::GetHist] wrong axisstring or classname");
    }
    delete hist4d;
  }else{
    PError((TString)"[dybPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)] Unsupported class name "+obj->ClassName());
  }
}
Plot dybPlotter::MakePlot(TString plotkey,TString option){
  Plot plot=Plotter::MakePlot(plotkey,option);
  if(plot.histname.Contains(TRegexp("m[[-+0-9.]*,[-+0-9.]*]/"))){
    auto range=GetRange(plot.histname,"m");
    plot.SetOption(Form("Xmin:%f Xmax:%f",range.first,range.second));
    plot.histname=Replace(plot.histname,"m[[-+0-9.]*,[-+0-9.]*]/","");
  }
  if(plot.histname.Contains(TRegexp("y[[-+0-9.]*,[-+0-9.]*]/"))){
    auto range=GetRange(plot.histname,"y");
    plot.SetOption(Form("Ymin:%f Ymax:%f",range.first,range.second));
    plot.histname=Replace(plot.histname,"y[[-+0-9.]*,[-+0-9.]*]/","");
  }
  if(plot.histname.Contains(TRegexp("pt[[-+0-9.]*,[-+0-9.]*]/"))){
    auto range=GetRange(plot.histname,"pt");
    plot.SetOption(Form("Zmin:%f Zmax:%f",range.first,range.second));
    plot.histname=Replace(plot.histname,"pt[[-+0-9.]*,[-+0-9.]*]/","");
  }
  if(plot.histname.Contains(TRegexp("(m)$"))) plot.histname=Replace(plot.histname,"(m)$","(x)");
  if(plot.histname.Contains(TRegexp("(pt)$"))) plot.histname=Replace(plot.histname,"(pt)$","(z)");

  if(plot.histname.Contains(TRegexp("(x)$"))){plot.histname=Replace(plot.histname,"(x)$","");plot.SetOption("project:x");}
  if(plot.histname.Contains(TRegexp("(y)$"))){plot.histname=Replace(plot.histname,"(y)$","");plot.SetOption("project:y");}
  if(plot.histname.Contains(TRegexp("(z)$"))){plot.histname=Replace(plot.histname,"(z)$","");plot.SetOption("project:z");}
  if(plot.histname.Contains(TRegexp("(u)$"))){plot.histname=Replace(plot.histname,"(u)$","");plot.SetOption("project:u");}
  if(plot.histname.Contains(TRegexp("(x,z)$"))){plot.histname=Replace(plot.histname,"(x,z)$","");plot.SetOption("project:xz");}

  if(plot.histname.Contains("dimass")){plot.histname=Replace(plot.histname,"dimass","costhetaCS");plot.SetOption("project:x");}
  else if(plot.histname.Contains("mass")){plot.histname=Replace(plot.histname,"mass","costhetaRecoil");plot.SetOption("project:x");}
  if(plot.histname.Contains("dirap")){plot.histname=Replace(plot.histname,"dirap","costhetaCS");plot.SetOption("project:y");}
  if(plot.histname.Contains("cha")){plot.histname=Replace(plot.histname,"cha","costhetaRecoil");plot.SetOption("project:y");}
  if(plot.histname.Contains("dipt")){plot.histname=Replace(plot.histname,"dipt","costhetaCS");plot.SetOption("project:z");}
  //if(plot.histname.Contains("bpt")){plot.histname=Replace(plot.histname,"bpt","costhetaRecoil");plot.SetOption("project:z");}

  if(plot.xmin==0&&plot.xmax==0){
    if(plot.project(0)=='x'){plot.xmin=plot.Xmin;plot.xmax=plot.Xmax;}
    else if(plot.project(0)=='y'){plot.xmin=plot.Ymin;plot.xmax=plot.Ymax;}
    else if(plot.project(0)=='z'){plot.xmin=plot.Zmin;plot.xmax=plot.Zmax;}
    else if(plot.project(0)=='u'){plot.xmin=plot.Umin;plot.xmax=plot.Umax;}
  }
  if(plot.ymin==0&&plot.ymax==0){
    if(plot.project(1)=='x'){plot.ymin=plot.Xmin;plot.ymax=plot.Xmax;}
    else if(plot.project(1)=='y'){plot.ymin=plot.Ymin;plot.ymax=plot.Ymax;}
    else if(plot.project(1)=='z'){plot.ymin=plot.Zmin;plot.ymax=plot.Zmax;}
    else if(plot.project(1)=='u'){plot.ymin=plot.Umin;plot.ymax=plot.Umax;}
  }

  TString ll="ll";
  if(plot.histname.Contains("mm201")) ll="#mu#mu";
  else if(plot.histname.Contains("ee201")) ll="ee";

  if(plot.xtitle==""){
    if(plot.project=="x") plot.SetOption("xtitle:'m("+ll+") [GeV]'");
    else if(plot.project=="y") plot.SetOption("xtitle:y("+ll+")");
    else if(plot.project=="z") plot.SetOption("xtitle:'p_{T}("+ll+") [GeV]'");
    else if(plot.project=="u"){
      if(plot.histname.Contains("costhetaCS")) plot.SetOption("xtitle:cos#theta_{CS}");
    }
  }
  
  if(plot.ytitle==""){
    if(plot.type==Plot::Type::Compare){
      if(plot.histname.Contains("AFB")) plot.SetOption("ytitle:A_{FB}");
      else if(option.Contains("norm")) plot.SetOption("ytitle:Normalized");
      else if(option.Contains("widthweight")) plot.SetOption("ytitle:'Events / 1 GeV'");
      else plot.SetOption("ytitle:Events");
    }else if(plot.IsMultiPad()){
      if(plot.histname.Contains("AFB")) plot.SetOption("1:ytitle:A_{FB}");
      else if(option.Contains("norm")) plot.SetOption("1:ytitle:Normalized");
      else if(option.Contains("widthweight")) plot.SetOption("1:ytitle:'Events / 1 GeV'");
      else plot.SetOption("1:ytitle:Events");
    }
  }

  if(plot.histname.Contains("AFB")&&entries.size()>1&&!option.Contains("type:")) plot.SetOption(Form("type:%d",Plot::Type::CompareAndSig));
  return plot;
}

pair<double,double> dybPlotter::GetAFBAndError(TH1* hist){
  if(!hist) return make_pair(0,0);
  TH1* to_delete=NULL;
  if(strstr(hist->ClassName(),"THStack")!=NULL){
    hist=GetTH1(hist);    
    to_delete=hist;
  }
  double ef,eb;
  int zerobin=hist->FindBin(0);
  double valf=hist->IntegralAndError(zerobin,hist->GetNbinsX()+2,ef);
  double valb=hist->IntegralAndError(0,zerobin-1,eb);
  if(to_delete) delete to_delete;
  return make_pair((valf-valb)/(valf+valb),2*sqrt(ef*ef*valb*valb+eb*eb*valf*valf)/pow(valf+valb,2));
}

TH1* dybPlotter::GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den){
  PDebug("[dybPlotter::GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den)]");
  if(!hist_forward_num||!hist_forward_den||!hist_backward_num||!hist_backward_den){
    PError("[dybPlotter::GetHistweightedAFB] It is null TH1*");
    return NULL;
  }
  hist_forward_num=GetTH1(hist_forward_num,true);
  hist_forward_den=GetTH1(hist_forward_den,true);
  hist_backward_num=GetTH1(hist_backward_num,true);
  hist_backward_den=GetTH1(hist_backward_den,true);
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
TH1* dybPlotter::GetHistAFB(TH1* hist_forward,TH1* hist_backward){
  PDebug("[dybPlotter::GetHistAFB(TH1* hist_forward,TH1* hist_backward)]");
  if(!hist_forward||!hist_backward){
    PError("[dybPlotter::GetHistAFB] It is null TH1*");
    return NULL;
  }
  hist_forward=GetTH1(hist_forward,true);
  hist_backward=GetTH1(hist_backward,true);
  TH1* hist=(TH1*)hist_forward->Clone();
  hist->SetDirectory(pdir);
  hist->Reset();
  for(int i=0;i<hist->GetNcells();i++){
    double valf=hist_forward->GetBinContent(i);
    double ef=hist_forward->GetBinError(i);
    double valb=hist_backward->GetBinContent(i);
    double eb=hist_backward->GetBinError(i);
    if(valf+valb){
      hist->SetBinContent(i,(valf-valb)/(valf+valb));
      hist->SetBinError(i,2*sqrt(ef*ef*valb*valb+eb*eb*valf*valf)/pow(valf+valb,2));
    }
  }
  TString this_title=hist_forward->GetTitle();
  hist->SetTitle(this_title.ReplaceAll("forward","AFB"));
  return hist;
}  

dybPlotter::dybPlotter(TString mode_,TString Analyzer_){
  SetupTH4D();
  Setup(mode_,Analyzer_);
}
dybPlotter::~dybPlotter(){}

void dybPlotter::SetupEntries(TString mode_){
  TString entrystring;
  if(mode_=="amcel") entrystring="data ^amc+tau_amc+vv+wjets+tttw+ss_amc+aa";
  else if(mode_=="amcmu") entrystring="data ^amc+tau_amc+vv+wjets+tttw+1.8*ss_amc+aa";
  else if(mode_=="mgel") entrystring="data ^mg+tau_mg+vv+wjets+tttw+ss_mg+aa";
  else if(mode_=="mgmu") entrystring="data ^mg+tau_mg+vv+wjets+tttw+1.8*ss_mg+aa";
  else if(mode_=="miel") entrystring="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa";
  else if(mode_=="mimu") entrystring="data ^mi+tau_mi+vv+wjets+tttw+1.8*ss_mi+aa";
  else if(mode_=="doublediff") entrystring="mi+tau_mi+vv+wjets+tttw+ss_mi+aa";
  else if(mode_ == "mc") entrystring = "^dyb_mi+dybbar_mi+mi+dyc_mi+dycbar_mi+dyudsg_mi+tau_mi+vv+tttw+ss_mi+aa";
  else if(mode_ == "dyb") entrystring = "dyb_mi dybbar_mi";
  else if(mode_ == "dy") entrystring = "dyb_mi dybbar_mi mi+dyc_mi+dycbar_mi+dyudsg_mi+tau_mi";
  else if(mode_ == "sigbkg") entrystring = "dyb_mi+dybbar_mi mi+dyc_mi+dycbar_mi+dyudsg_mi+tau_mi+vv+tttw+ss_mi+aa";
  else if(mode_ == "sigbkg2") entrystring = "dyb_mi+dybbar_mi mi tttw";
  else {
    entrystring=mode_;
  }

  Plotter::SetupEntries(entrystring);
  mode=mode_;
  if(mode=="doublediff"){
    entries[0].title="[data_{#mu#mu}-sim_{#mu#mu}]-[data_{ee}-sim_{ee}]";
  }
}
void dybPlotter::SetupTH4D(){
  if(TClass::GetClass("TH4D")==NULL){
    gROOT->ProcessLine(".L TH4D/TH4D.cxx+");
  }
}
  
#endif
