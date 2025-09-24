#ifndef __TTLJPLOTTER_CC__
#define __TTLJPLOTTER_CC__
#include"SKFlatPlotter.cc"
#if __has_include("TH4D.h")
#include "TH4D.h"
#endif

class ttljPlotter:public SKFlatPlotter{
public:
  ttljPlotter(TString mode_="data ^correct_ttlj+wrong_ttlj+unmatched_ttlj+ttlj_2b+ttall_2b+stall_2b+qcd_2b+ewkall_2b", float norm_=1., TString analyzer_="ttljAnalyzer");
  ~ttljPlotter();
  using Plotter::GetHist;
  virtual void SetupEntries(TString mode_);
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual TH1* GetHist(int ientry,TString plotkey,TString additional_option="") {return Plotter::GetHist(ientry,plotkey,additional_option); } // for pyroot compatibility
  virtual void GetHistActionForAdditionalClass(TObject*& obj,Plot plot);
  virtual Plot MakePlot(TString name,TString option="");
  float norm_;
};
Plot ttljPlotter::MakePlot(TString plotkey,TString option){
  Plot plot=Plotter::MakePlot(plotkey,option);
  if(plot.histname.Contains("ee2016a/")) plot.lumi="4.00";
  else if(plot.histname.Contains("ee2016b/")) plot.lumi="1.97";
  else if(plot.histname.Contains("ee2017/")) plot.lumi="41.5";
  else if(plot.histname.Contains("ee2018/")) plot.lumi="59.8";
  else if(plot.histname.Contains("mm2016a/")) plot.lumi="19.5";
  else if(plot.histname.Contains("mm2016b/")) plot.lumi="8.46";
  else if(plot.histname.Contains("mm2017/")) plot.lumi="6.51";
  else if(plot.histname.Contains("mm2018/")) plot.lumi="2.97";
  //if(plot.histname.Contains("_eff")&&plot.type==Plot::Type::CompareAndRatio){
  return plot;
}
TH1* ttljPlotter::GetHist(const Sample& sample,Plot plot,TString additional_option){
  TH1* hist=NULL;
  plot.SetOption(additional_option);
  if(plot.histname.Contains("_eff_dzsf_dz")){
    TH1* hist_num=GetHist(sample,plot,"histname:"+Replace(plot.histname,"_eff_dzsf_dz","_num_dzsf_dz\
"));
    TH1* hist_den=GetHist(sample,plot,"histname:"+Replace(plot.histname,"_eff_dzsf_dz","_den"));
    hist=(TH1*)hist_num->Clone("eff");
    hist->Divide(hist_num,hist_den,1,1,"B");
  }else if(plot.histname.Contains("_eff_dzsf")){
    TH1* hist_num=GetHist(sample,plot,"histname:"+Replace(plot.histname,"_eff_dzsf","_num_dzsf"));
    TH1* hist_den=GetHist(sample,plot,"histname:"+Replace(plot.histname,"_eff_dzsf","_den"));
    hist=(TH1*)hist_num->Clone("eff");
    hist->Divide(hist_num,hist_den,1,1,"B");
  }else if(plot.histname.Contains("_eff")){
    TH1* hist_num=GetHist(sample,plot,"histname:"+Replace(plot.histname,"_eff","_num"));
    TH1* hist_den=GetHist(sample,plot,"histname:"+Replace(plot.histname,"_eff","_den"));
    hist=(TH1*)hist_num->Clone("eff");
    hist->Divide(hist_num,hist_den,1,1,"B");
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
void ttljPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot){
  PAll("[ttljPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)");
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
    PError((TString)"[ttljPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)] Unsupported class name "+obj->ClassName());
  }
}

ttljPlotter::ttljPlotter(TString mode_, float norm, TString Analyzer_){
  norm_ = norm;
  Setup(mode_, Analyzer_);
}
ttljPlotter::~ttljPlotter(){}

void ttljPlotter::SetupEntries(TString mode_){
  TString entrystring;
  if(mode_ == "ttlj") entrystring = "correct_ttlj wrong_ttlj unmatched_ttlj";
  else if(mode_ == "data_sub ttlj") entrystring = Form("data-%f*unmatched_ttlj-%f*ttall_2b-%f*stall_2b-%f*qcd_2b-%f*ewkall_2b correct_ttlj+wrong_ttlj", norm_, norm_, norm_, norm_, norm_);
  else if(mode_ == "data mc") entrystring = "data correct_ttlj+wrong_ttlj+unmatched_ttlj+ttlj_2b+ttall_2b+stall_2b+qcd_2b+ewkall_2b";
  else {
    entrystring=mode_;
  }

  Plotter::SetupEntries(entrystring);
  mode=mode_;
  if(mode_=="data_sub ttlj"){
    entries[0].title="data (bkg sub.)";
  }
}
  
#endif
