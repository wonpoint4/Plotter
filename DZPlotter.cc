#ifndef __DZPLOTTER_CC__
#define __DZPLOTTER_CC__
#include"Plotter.cc"
#if __has_include("TH4D.h")
#include "TH4D.h"
#endif

class DZPlotter:public Plotter{
public:
  void SetupSystematics();
  int Setup(TString mode_);
  DZPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa",TString suffix="");
  using Plotter::GetHist;
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual void GetHistActionForAdditionalClass(TObject*& obj,Plot plot);
  virtual Plot MakePlot(TString plotkey,TString option="");
};
Plot DZPlotter::MakePlot(TString plotkey,TString option){
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
TH1* DZPlotter::GetHist(const Sample& sample,Plot plot,TString additional_option){
  TH1* hist=NULL;
  plot.SetOption(additional_option);
  if(plot.histname.Contains("_eff_dzsf_dz")){
    TH1* hist_num=GetHist(sample,plot,"histname:"+Replace(plot.histname,"_eff_dzsf_dz","_num_dzsf_dz"));
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
DZPlotter::DZPlotter(TString mode_,TString suffix){
  ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/DZAnalyzer"+suffix+"/");

  samples["muon"]=Sample("data (#mu#mu)","SAMPLE data",kBlack,20)+TRegexp("/DATA/DZAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]")+TRegexp("/DATA/DZAnalyzer_SkimTree_Dilepton_SingleMuon_[A-Z]");
  samples["electron"]=Sample("data (ee)","SAMPLE data",kBlack,20)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_.*EG.*_[A-Z]")+TRegexp("/DZAnalyzer_SkimTree_Dilepton_SingleElectron_[A-Z]");
  samples["data"]=Sample("data","SAMPLE data",kBlack,20)+"muon"+"electron";
  samples["amc"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_DYJets$");
  samples["tau_amc"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_DYJets$"));
  samples["amcS20"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_DYJets_Summer20$");
  samples["tau_amcS20"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_DYJets_Summer20$"));
  samples["mi"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_DYJetsTo.*MiNNLO$");
  samples["tau_mi"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_DYJetsTo.*MiNNLO$"));
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_DYJets_MG$");
  samples["tau_mg"]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_DYJets_MG$"));
  samples["vv"]=Sample("Diboson","SAMPLE sim",kBlue)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_[W-Z][W-Z]_pythia$");
  samples["wjets"]=Sample("W","SAMPLE sim",kYellow)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_WJets_MG$");
  samples["tt"]=Sample("t#bar{t}","SAMPLE sim",kMagenta)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_TTLL_powheg$");
  samples["tw"]=Sample("t#bar{t}","SAMPLE sim",kMagenta+10)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_SingleTop_tW_.*top_NoFullyHad$");
  samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  samples["aa"]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim",kYellow+1)+TRegexp("/DZAnalyzer_SkimTree_Dilepton_GGToLL$");
  samples["ss"]="ss_"%(Sample("QCD multi-jet","SUM",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");
  samples["ss_mi"]="ss_"%(Sample("QCD multi-jet","SUM",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tttw"-"aa");

  samples["sim_stack"]=Sample("sim","STACK",Style(kRed,-1,3001,"e2"),Style(kCyan,-1,3001,"e2"))+"amc"+"tau_amc"+"vv"+"wjets"+"tttw";
  samples["sim"]=Sample("simulation","SUM",Style(kRed,22,3001,"e2"),Style(kCyan,-1,3001,"e2"))+"amc"+"tau_amc"+"vv"+"wjets"+"tttw";
  samples["sim_noSF"]=(Sample("w/o efficiency SF","SUM",kBlue)+"amc"+"tau_amc"+"vv"+"wjets"+"tttw")%"_noefficiencySF";
  //for(auto& sub:samples["sim_noSF"].subs) sub.type=Sample::Type::A;
  samples["sim_noSF"].styles[0].linewidth=1;

  Setup(mode_);
}

int DZPlotter::Setup(TString mode_){
  Reset();

  mode=mode_;

  SetupEntries(mode_);
  SetupSystematics();

  if(DEBUG) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(DEBUG) std::cout<<"[Setup] nsys: "<<systematics.size()<<endl;
  if(DEBUG) std::cout<<"[Setup] nplot: "<<plots.size()<<endl;

  return 1;
}
void DZPlotter::SetupSystematics(){
  if(DEBUG)  cout<<"[SetupSystematics]"<<endl;
  int nreplica=20;
  AddSystematic("efficiencySF_stat","efficiencySF_stat",Systematic::Type::GAUSSIAN,FormRange("_efficiencySF_stat%d",Range(20)),"sim");
  AddSystematic("electronRECOSF_sys1","electronRECOSF_sys1",Systematic::Type::ENVELOPE,"_electronRECOSF_s1_m0 _electronRECOSF_s1_m1","sim");
  AddSystematic("electronRECOSF_sys2","electronRECOSF_sys2",Systematic::Type::ENVELOPE,"_electronRECOSF_s2_m0 _electronRECOSF_s2_m1","sim");
  AddSystematic("electronRECOSF_sys3","electronRECOSF_sys3",Systematic::Type::ENVELOPE,"_electronRECOSF_s3_m0 _electronRECOSF_s3_m1","sim");
  AddSystematic("electronRECOSF_sys4","electronRECOSF_sys4",Systematic::Type::ENVELOPE,"_electronRECOSF_s4_m0 _electronRECOSF_s4_m1","sim");
  AddSystematic("electronRECOSF","electronRECOSF_sys",Systematic::Type::MULTI,"electronRECOSF_sys1 electronRECOSF_sys2 electronRECOSF_sys3 electronRECOSF_sys4","sim");
  AddSystematic("electronIDSF_sys1","electronIDSF_sys1",Systematic::Type::ENVELOPE,"_electronIDSF_s1_m0","sim");
  AddSystematic("electronIDSF_sys2","electronIDSF_sys2",Systematic::Type::ENVELOPE,"_electronIDSF_s2_m0","sim");
  AddSystematic("electronIDSF_sys3","electronIDSF_sys3",Systematic::Type::ENVELOPE,"_electronIDSF_s3_m0","sim");
  AddSystematic("electronIDSF_sys4","electronIDSF_sys4",Systematic::Type::ENVELOPE,"_electronIDSF_s4_m0","sim");
  AddSystematic("electronIDSF_sys5","electronIDSF_sys5",Systematic::Type::ENVELOPE,"_electronIDSF_s5_m0","sim");
  AddSystematic("electronIDSF_sys6","electronIDSF_sys6",Systematic::Type::ENVELOPE,"_electronIDSF_s6_m0","sim");
  AddSystematic("electronIDSF_sys7","electronIDSF_sys7",Systematic::Type::ENVELOPE,"_electronIDSF_s7_m0","sim");
  AddSystematic("electronIDSF","electronIDSF_sys",Systematic::Type::MULTI,"electronIDSF_sys1 electronIDSF_sys2 electronIDSF_sys3 electronIDSF_sys4 electronIDSF_sys5 electronIDSF_sys6 electronIDSF_sys7","sim");
  AddSystematic("muonIDSF_sys1","muonIDSF_sys1",Systematic::Type::ENVELOPE,"_muonIDSF_s1_m0 _muonIDSF_s1_m1","sim");
  AddSystematic("muonIDSF_sys2","muonIDSF_sys2",Systematic::Type::ENVELOPE,"_muonIDSF_s2_m0 _muonIDSF_s2_m1","sim");
  AddSystematic("muonIDSF_sys3","muonIDSF_sys3",Systematic::Type::ENVELOPE,"_muonIDSF_s3_m0","sim");
  AddSystematic("muonIDSF_sys4","muonIDSF_sys4",Systematic::Type::ENVELOPE,"_muonIDSF_s4_m0 _muonIDSF_s4_m1","sim");
  AddSystematic("muonIDSF","muonIDSF_sys",Systematic::Type::MULTI,"muonIDSF_sys1 muonIDSF_sys2 muonIDSF_sys3 muonIDSF_sys4","sim");
  AddSystematic("triggerSF_sys1","triggerSF_sys1",Systematic::Type::ENVELOPE,"_triggerSF_s1_m0 _triggerSF_s1_m1","sim");
  AddSystematic("triggerSF_sys2","triggerSF_sys2",Systematic::Type::ENVELOPE,"_triggerSF_s2_m0 _triggerSF_s2_m1","sim");
  AddSystematic("triggerSF_sys3","triggerSF_sys3",Systematic::Type::ENVELOPE,"_triggerSF_s3_m0","sim");
  AddSystematic("triggerSF_sys4","triggerSF_sys4",Systematic::Type::ENVELOPE,"_triggerSF_s4_m0","sim");
  AddSystematic("triggerSF_sys5","triggerSF_sys5",Systematic::Type::ENVELOPE,"_triggerSF_s5_m0","sim");
  AddSystematic("triggerSF_sys6","triggerSF_sys6",Systematic::Type::ENVELOPE,"_triggerSF_s6_m0","sim");
  AddSystematic("triggerSF","triggerSF_sys",Systematic::Type::MULTI,"triggerSF_sys1 triggerSF_sys2 triggerSF_sys3 triggerSF_sys4 triggerSF_sys5 triggerSF_sys6","sim");

  AddSystematic("noRECOSF","noRECOSF",Systematic::Type::ENVELOPE,"_noRECOSF","sim");
  AddSystematic("noIDSF","noIDSF",Systematic::Type::ENVELOPE,"_noIDSF","sim");
  AddSystematic("noISOSF","noISOSF",Systematic::Type::ENVELOPE,"_noISOSF","sim");
  AddSystematic("notriggerSF","notriggerSF",Systematic::Type::ENVELOPE,"_notriggerSF","sim");

  AddSystematic("nozptcor","nozptcor",Systematic::Type::ENVELOPE,"_nozptcor","dy");
  AddSystematic("noefficiencySF","noefficiencySF",Systematic::Type::ENVELOPE,"_noefficiencySF","sim");
  AddSystematic("efficiencySF","eff. SF unc.",Systematic::Type::MULTI,"efficiencySF_stat electronRECOSF electronIDSF muonIDSF triggerSF");

}
void DZPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot){
  PAll("[DZPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)");
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
    PError((TString)"[DZPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)] Unsupported class name "+obj->ClassName());
  }
}

#endif
