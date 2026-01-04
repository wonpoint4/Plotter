#include"SKFlatPlotter.cc"
#ifndef __BBPlotter_CC__
#define __BBPlotter_CC__
class BBPlotter:public SKFlatPlotter{
public:
  BBPlotter(TString mode_="data ^tt+unmatched_tt+mi+tau_mi+vv+wjets+st+qcdb+aa",TString Analyzer_="BBAnalyzer");
  using SKFlatPlotter::GetHist;
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual void SetupEntries(TString mode_);
  virtual void SetupSamples();
  virtual void SetupSystematics();
  virtual void DrawCompare(Plot p);

};
BBPlotter::BBPlotter(TString mode_,TString Analyzer_){Setup(mode_,Analyzer_);}
void BBPlotter::SetupEntries(TString mode_){
  if(mode_=="eff"){
    //Plotter::SetupEntries("data-unmatched_tt-mi-tau_mi-vv-wjets-st-qcdss-aa tt");
    //Plotter::SetupEntries("data-unmatched_tt-mi-tau_mi-vv-wjets-st-aa tt");
    Plotter::SetupEntries("data-unmatched_tt-mi-tau_mi-vv-wjets-st-qcdb-aa tt");
    //Plotter::SetupEntries("data tt+unmatched_tt+mi+tau_mi+vv+wjets+st+qcdb+aa");
    //Plotter::SetupEntries("data-unmatched_tt-tau_mi-wjets-aa tt+mi+st+vv+qcdb");
  }else if(mode_=="charge"){
    Plotter::SetupEntries("ttll ttll");
    entries[0].title="b";
    entries[0].replace["ccharge"]="mcharge";
    entries[0].styles[0].linecolor=2;
    entries[0].styles[0].linewidth=2;
    entries[1].title="#bar{b}";
    entries[1].replace["ccharge"]="pcharge";
    entries[1].styles[0].linecolor=4;
    entries[1].styles[0].linewidth=2;
  }else{
    Plotter::SetupEntries(mode_);
  }
  mode=mode_;
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
void BBPlotter::DrawCompare(Plot p){
  Plotter::DrawCompare(p);
  if(p.histname.Contains("/charge")){
    TH1* axisparent=GetAxisParent();
    if(axisparent&&axisparent->GetNbinsX()==4){
      axisparent->GetXaxis()->SetBinLabel(1,"#minus #minus");
      axisparent->GetXaxis()->SetBinLabel(2,"#plus #minus");
      axisparent->GetXaxis()->SetBinLabel(3,"#minus #plus");
      axisparent->GetXaxis()->SetBinLabel(4,"#plus #plus");
      axisparent->GetXaxis()->SetTitle("bjet-pair charge");      
      axisparent->GetXaxis()->SetLabelSize(axisparent->GetXaxis()->GetLabelSize()*2);
    }
  }else if(p.histname.Contains("ccharge")){
    TH1* axisparent=GetAxisParent();
    axisparent->GetXaxis()->SetLabelSize(0);
    TLatex latex;
    latex.SetNDC();
    latex.DrawLatex(0.21,0.04,"#bf{e}^{-}");
    latex.DrawLatex(0.36,0.04,"#mu^{-}");
    latex.DrawLatex(0.66,0.04,"#mu^{+}");
    latex.DrawLatex(0.81,0.04,"#bf{e}^{+}");
    latex.DrawLatex(0.45,0.04,"#bf{jet charge}");
    TGaxis *axis=new TGaxis(-1,0,1,0,-1,1,502);
    axis->Draw();
    //axis->SetBinLabel
  }    
}
void BBPlotter::SetupSamples(){
  SKFlatPlotter::SetupSamples();
  samples["tt"]=Sample("t#bar{t}","SUM",kMagenta+1)+"ttll"+"ttlj";
  samples["unmatched_tt"]="unmatched_"%(Sample("t#bar{t} (unmatched)","SUM",kMagenta-4)+"ttll"+"ttlj")+"ttjj";
  samples["qcdb"]=Sample("QCD multi-jet","SAMPLE sim",kCyan)+TRegexp(Analyzer+SkimTree+"_.*QCD_bEnriched_HT.*");
}
void BBPlotter::SetupSystematics(){
  SKFlatPlotter::SetupSystematics();
}
#endif
