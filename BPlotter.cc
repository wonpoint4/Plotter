#include"SKFlatPlotter.cc"
#ifndef __BPlotter_CC__
#define __BPlotter_CC__
class BPlotter:public SKFlatPlotter{
public:
  BPlotter(TString mode_="ttll",TString Analyzer_="BjetAnalyzer");
  using SKFlatPlotter::GetHist;
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual void SetupEntries(TString mode_);
  virtual void DrawCompare(Plot p);

};
BPlotter::BPlotter(TString mode_,TString Analyzer_){Setup(mode_,Analyzer_);}
void BPlotter::SetupEntries(TString mode_){
  if(mode_=="eff"){
    Plotter::SetupEntries("data-unmatched_tt-mi-tau_mi-vv-wjets-st-qcdss-aa tt");
  }else if(mode_=="correct"){
    Plotter::SetupEntries("ttll ttll");
    entries[0].title="correct charge";
    entries[0].replace["correct"]="correct1";
    entries[0].styles[0].linecolor=2;
    entries[0].styles[0].linewidth=2;
    entries[1].title="wrong charge";
    entries[1].replace["correct"]="correct0";
    entries[1].styles[0].linecolor=4;
    entries[1].styles[0].linewidth=2;
  }else{
    Plotter::SetupEntries(mode_);
  }
  mode=mode_;
}

TH1* BPlotter::GetHist(const Sample& sample,Plot plot,TString additional_option){
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
void BPlotter::DrawCompare(Plot p){
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
void Report(TString channel){
  BPlotter aa;
  TH1* hp=aa.GetHist(0,channel+"/bpcharge_correct[01]_type[012]","rebin:{-5,-4,-3,-2,-1,0,1,2,3,4,5}");
  TH1* hm=aa.GetHist(0,channel+"/bmcharge_correct[01]_type[012]","rebin:{-5,-4,-3,-2,-1,0,1,2,3,4,5}");
  double total=hp->Integral()+hm->Integral();
  double total_type0=hp->Integral(5,6)+hm->Integral(5,6);
  double correct_type0=hp->Integral(6,6)+hm->Integral(5,5);
  double total_type1=hp->Integral(3,4)+hp->Integral(7,8)+hm->Integral(3,4)+hm->Integral(7,8);
  double correct_type1=hp->Integral(7,8)+hm->Integral(3,4);
  double correct2_type1=hp->Integral(4,4)+hp->Integral(8,8)+hm->Integral(3,3)+hm->Integral(7,7);
  double total_type2=hp->Integral(1,2)+hp->Integral(9,10)+hm->Integral(1,2)+hm->Integral(9,10);
  double correct_type2=hp->Integral(9,10)+hm->Integral(1,2);
  double correct2_type2=hp->Integral(2,2)+hp->Integral(10,10)+hm->Integral(1,1)+hm->Integral(9,9);
  cout<<fixed;;
  cout.precision(4);
  cout<<"Type\tFrac.\tAcc.\tjetChargeAccu."<<endl;
  cout<<"all\t"<<(total_type0+total_type1+total_type2)/total<<"\t"<<(correct_type0+correct_type1+correct_type2)/total<<"\t"<<(correct_type0+correct2_type1+correct2_type2)/total<<endl;
  cout<<"type0\t"<<total_type0/total<<"\t"<<correct_type0/total_type0<<endl;
  cout<<"type1\t"<<total_type1/total<<"\t"<<correct_type1/total_type1<<"\t"<<correct2_type1/total_type1<<endl;
  cout<<"type2\t"<<total_type2/total<<"\t"<<correct_type2/total_type2<<"\t"<<correct2_type2/total_type2<<endl;
  cout.unsetf(ios::fixed);
}
#endif
