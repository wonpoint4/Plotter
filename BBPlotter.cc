#include"SKFlatPlotter.cc"
#ifndef __BBPlotter_CC__
#define __BBPlotter_CC__
class BBPlotter:public SKFlatPlotter{
public:
  BBPlotter(TString mode_="data ^tt+unmatched_tt+mi+tau_mi+vv+wjets+st+qcdss+aa",TString Analyzer_="BBAnalyzer");
  using SKFlatPlotter::GetHist;
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual void SetupSamples();
  virtual void SetupSystematics();

};
BBPlotter::BBPlotter(TString mode_,TString Analyzer_){
  if(mode_=="eff"){
    Setup("data-unmatched_tt-mi-tau_mi-vv-wjets-st-qcdss-aa tt",Analyzer_);
  }else{
    Setup(mode_,Analyzer_);
  }
}

TH1* BBPlotter::GetHist(const Sample& sample,Plot plot,TString additional_option){
  TH1* hist=NULL;
  plot.SetOption(additional_option);
  if(plot.option.Contains("eff:")){
    TString opt=plot.option(TRegexp("eff:[^ ]*"));
    TString histname_den=opt(4,opt.Length());
    TH1* hist_num=GetTH1(GetHist(sample,plot-"eff"),true);
    TH1* hist_den=GetTH1(GetHist(sample,plot-"eff","histname:"+histname_den),true);
    hist=(TH1*)hist_num->Clone();
    hist->Divide(hist_num,hist_den,1,1,"B");
    if(hist_num) delete hist_num;
    if(hist_den) delete hist_den;
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
void BBPlotter::SetupSamples(){
  SKFlatPlotter::SetupSamples();
  samples["tt"]=Sample("t#bar{t}","SUM",kMagenta+1)+"ttll"+"ttlj";
  samples["unmatched_tt"]="unmatched_"%(Sample("t#bar{t} (unmatched)","SUM",kMagenta-4)+"ttll"+"ttlj")+"ttjj";
}
void BBPlotter::SetupSystematics(){
  SKFlatPlotter::SetupSystematics();
}
#endif
