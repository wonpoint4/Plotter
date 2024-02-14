#include"SKFlatPlotter.cc"
#ifndef __AFBSYSTPLOTTER_CC__
#define __AFBSYSTPLOTTER_CC__
class AFBSystPlotter:public SKFlatPlotter{
public:
  AFBSystPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tt+st+qcdss+aa",TString Analyzer_="AFBAnalyzerSyst");
  using SKFlatPlotter::GetHist;
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual TH1* GetHistAFB(TH1* hist_forward,TH1* hist_backward);
};
AFBSystPlotter::AFBSystPlotter(TString mode_,TString Analyzer_):SKFlatPlotter(mode_,Analyzer_){}

TH1* AFBSystPlotter::GetHist(const Sample& sample,Plot plot,TString additional_option){
  TH1* hist=NULL;
  plot.SetOption(additional_option);
  if(plot.option.Contains("AFB")){
    TH1* hist_forward=GetHist(sample,plot-"AFB"," Ymin:0 Ymax:1 project:x"); 
    TH1* hist_backward=GetHist(sample,plot-"AFB"," Ymin:-1 Ymax:0 project:x");
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
TH1* AFBSystPlotter::GetHistAFB(TH1* hist_forward,TH1* hist_backward){
  PDebug("[AFBPlotter::GetHistAFB(TH1* hist_forward,TH1* hist_backward)]");
  if(!hist_forward||!hist_backward){
    PError("[AFBPlotter::GetHistAFB] It is null TH1*");
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

#endif
