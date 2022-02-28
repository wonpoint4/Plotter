#ifndef __AFBPLOTTER_CC__
#define __AFBPLOTTER_CC__
#include"Plotter.cc"
#if __has_include("TH4D.h")
#include "TH4D.h"
#endif
class AFBPlotter:public Plotter{
public:
  void SetupSystematics();
  int Setup(TString mode_);
  TString analyzer;
  AFBPlotter(TString mode_="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa",TString analyzer_="AFBAnalyzer");
  ~AFBPlotter();

  void SetupTH4D();
  pair<double,double> GetRange(TString histname,TString axisname);
  pair<double,double> GetAFBAndError(TH1* costhetaCS);

  using Plotter::GetHist;
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual TH1* GetHist(int ientry,TString plotkey,TString additional_option="") {return Plotter::GetHist(ientry,plotkey,additional_option); } // for pyroot compatibility
  virtual void GetHistActionForAdditionalClass(TObject*& obj,Plot plot);
  virtual Plot MakePlot(TString name,TString option="");

  TH1* GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den);
  TH1* GetHistAFB(TH1* hist_forward,TH1* hist_backward);

  //TCanvas* DrawPlot(TString plotkey,TString option="");

  ClassDef(AFBPlotter,0);
};
ClassImp(AFBPlotter);
pair<double,double> AFBPlotter::GetRange(TString histname,TString axisname){
  TString rangestring=histname(TRegexp(axisname+"[[-+0-9.]*,[-+0-9.]*]"));
  TString first=rangestring(axisname.Length()+1,rangestring.Index(',')-axisname.Length()-1);
  TString second=rangestring(rangestring.Index(',')+1,rangestring.Length()-rangestring.Index(',')-2);
  return make_pair(first.Atof(),second.Atof());
}  

TH1* AFBPlotter::GetHist(const Sample& sample,Plot plot,TString additional_option){
  TH1* hist=NULL;
  plot.SetOption(additional_option);
  if(plot.histname.Contains("weightedAFB")){
    TH1* hist_forward_num=GetHist(sample,plot,"histname:"+Replace(plot.histname,"weightedAFB","costhetaCS_num")+" Umin:0 Umax:1");
    TH1* hist_forward_den=GetHist(sample,plot,"histname:"+Replace(plot.histname,"weightedAFB","costhetaCS_den")+" Umin:0 Umax:1");
    TH1* hist_backward_num=GetHist(sample,plot,"histname:"+Replace(plot.histname,"weightedAFB","costhetaCS_num")+" Umin:-1 Umax:0");
    TH1* hist_backward_den=GetHist(sample,plot,"histname:"+Replace(plot.histname,"weightedAFB","costhetaCS_den")+" Umin:-1 Umax:0");
    hist=GetHistWeightedAFB(hist_forward_num,hist_forward_den,hist_backward_num,hist_backward_den);
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
void AFBPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot){
  PAll("[AFBPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)");
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
    PError((TString)"[AFBPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)] Unsupported class name "+obj->ClassName());
  }
}
Plot AFBPlotter::MakePlot(TString plotkey,TString option){
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
  if(plot.histname.Contains("dirap")){plot.histname=Replace(plot.histname,"dirap","costhetaCS");plot.SetOption("project:y");}
  if(plot.histname.Contains("dipt")){plot.histname=Replace(plot.histname,"dipt","costhetaCS");plot.SetOption("project:z");}

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

  if(plot.project=="x") plot.SetOption("xtitle:'m("+ll+") [GeV]'");
  else if(plot.project=="y") plot.SetOption("xtitle:y("+ll+")");
  else if(plot.project=="z") plot.SetOption("xtitle:'p_{T}("+ll+") [GeV]'");
  else if(plot.project=="u"){
    if(plot.histname.Contains("costhetaCS")) plot.SetOption("xtitle:cos#theta_{CS}");
  }
  
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

  if(plot.histname.Contains("AFB")&&entries.size()>1&&!option.Contains("type:")) plot.SetOption(Form("type:%d",Plot::Type::CompareAndSig));
  return plot;
}

pair<double,double> AFBPlotter::GetAFBAndError(TH1* hist){
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

TH1* AFBPlotter::GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den){
  PDebug("[AFBPlotter::GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den)]");
  if(!hist_forward_num||!hist_forward_den||!hist_backward_num||!hist_backward_den){
    PError("[AFBPlotter::GetHistweightedAFB] It is null TH1*");
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
TH1* AFBPlotter::GetHistAFB(TH1* hist_forward,TH1* hist_backward){
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

AFBPlotter::AFBPlotter(TString mode_,TString analyzer_){
  analyzer=analyzer_;
  analyzer.ReplaceAll("_backup","");
  SetupTH4D();
  ScanFiles(TString()+getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/"+analyzer_+"/");
  
  samples["data"]=Sample("data","SAMPLE data",kBlack,20)+TRegexp("/DATA/"+analyzer+"_SkimTree_Dilepton_DoubleMuon_[A-Z]")+TRegexp("/DATA/"+analyzer+"_SkimTree_Dilepton_SingleMuon_[A-Z]")+TRegexp("/DATA/"+analyzer+"_SkimTree_Dilepton_SingleElectron_[A-Z]")+TRegexp("/DATA/"+analyzer+"_SkimTree_Dilepton_DoubleEG_[A-Z]")+TRegexp("/DATA/"+analyzer+"_SkimTree_Dilepton_EGamma_[A-Z]");
  samples["mm2016"]=Sample("data (#mu#mu2016a)","SAMPLE data mm 2016a",kBlack,20)+TRegexp("2016preVFP.*/DATA/"+analyzer+"_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["mm2016"]=Sample("data (#mu#mu2016b)","SAMPLE data mm 2016b",kBlack,20)+TRegexp("2016postVFP.*/DATA/"+analyzer+"_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["mm2017"]=Sample("data (#mu#mu2017)","SAMPLE data mm 2017",kRed,20)+TRegexp("2017.*/DATA/"+analyzer+"_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["mm2018"]=Sample("data (#mu#mu2018)","SAMPLE data mm 2018",kBlue,20)+TRegexp("2018.*/DATA/"+analyzer+"_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["ee2016"]=Sample("data (ee2016a)","SAMPLE data ee 2016a",kBlack,22)+TRegexp("2016preVFP.*/DATA/"+analyzer+"_SkimTree_Dilepton_.*EG.*_[A-Z]");
  samples["ee2016"]=Sample("data (ee2016b)","SAMPLE data ee 2016b",kBlack,22)+TRegexp("2016postVFP.*/DATA/"+analyzer+"_SkimTree_Dilepton_.*EG.*_[A-Z]");
  samples["ee2017"]=Sample("data (ee2017)","SAMPLE data ee 2017",kRed,22)+TRegexp("2017.*/DATA/"+analyzer+"_SkimTree_Dilepton_.*EG.*_[A-Z]");
  samples["ee2018"]=Sample("data (ee2018)","SAMPLE data ee 2018",kBlue,22)+TRegexp("2018.*/DATA/"+analyzer+"_SkimTree_Dilepton_.*EG.*_[A-Z]");
  samples["mm"]=Sample("data (#mu#mu)","SAMPLE data",kBlack,20)+TRegexp("/DATA/"+analyzer+"_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["ee"]=Sample("data (ee)","SAMPLE data",kRed,22)+TRegexp("/DATA/"+analyzer+"_SkimTree_Dilepton_.*EG.*_[A-Z]");

  samples["ww"]=Sample("WW","SAMPLE sim",kBlue)+TRegexp("/"+analyzer+"_.*WW_pythia");
  samples["wz"]=Sample("WZ","SAMPLE sim",kGreen)+TRegexp("/"+analyzer+"_.*WZ_pythia");
  samples["zz"]=Sample("ZZ","SAMPLE sim",kCyan)+TRegexp("/"+analyzer+"_.*ZZ_pythia");
  samples["vv"]=Sample("Diboson","SAMPLE sim",kBlue)+TRegexp("/"+analyzer+"_.*[W-Z][W-Z]_pythia");
  samples["wjets"]=Sample("W","SAMPLE sim",kYellow)+TRegexp("/"+analyzer+"_.*WJets_MG");
  samples["tt"]=Sample("t#bar{t}","SAMPLE sim tt",kMagenta)+TRegexp("/"+analyzer+"_.*TTLL_powheg");
  samples["ttlj"]=Sample("TTLJ","SAMPLE sim",kMagenta+6)+TRegexp("/"+analyzer+"_.*TTLJ_powheg");
  samples["tw"]=Sample("tW","SAMPLE sim",kMagenta+10)+TRegexp("/"+analyzer+"_.*SingleTop_tW_.*top_NoFullyHad");
  samples["st"]=Sample("ST","SAMPLE sim",kMagenta+12)+TRegexp("/"+analyzer+"_.*SingleTop_[st]ch_.*");
  samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  samples["qcdm"]=Sample("QCD Mu-enriched","SAMPLE sim",kCyan)+TRegexp("/"+analyzer+"_.*QCD_Pt.*MuEnrichedPt5$");
  samples["qcdm1"]=Sample("QCD Mu-enriched 10to20","SAMPLE sim",1)+TRegexp("/"+analyzer+"_.*QCD_Pt-15to20.*MuEnrichedPt5$");
  samples["qcdm2"]=Sample("QCD Mu-enriched 20to30","SAMPLE sim",2)+TRegexp("/"+analyzer+"_.*QCD_Pt-20to30.*MuEnrichedPt5$");
  samples["qcdm3"]=Sample("QCD Mu-enriched 30to50","SAMPLE sim",3)+TRegexp("/"+analyzer+"_.*QCD_Pt-30to50.*MuEnrichedPt5$");
  samples["qcdm4"]=Sample("QCD Mu-enriched 50to80","SAMPLE sim",4)+TRegexp("/"+analyzer+"_.*QCD_Pt-50to80.*MuEnrichedPt5$");
  samples["qcdm5"]=Sample("QCD Mu-enriched 80to120","SAMPLE sim",5)+TRegexp("/"+analyzer+"_.*QCD_Pt-80to120.*MuEnrichedPt5$");
  samples["qcdm6"]=Sample("QCD Mu-enriched 120to170","SAMPLE sim",6)+TRegexp("/"+analyzer+"_.*QCD_Pt-120to170.*MuEnrichedPt5$");
  samples["qcde"]=Sample("QCD EM-enriched","SAMPLE sim",kCyan+4)+TRegexp("/"+analyzer+"_.*QCD_Pt.*EMEnriched$");
  samples["qcdee"]=Sample("QCD bcToE","SAMPLE sim",kCyan)+TRegexp("/"+analyzer+"_.*QCD_Pt.*bcToE$");

  samples["aagr"]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim",kYellow+1)+TRegexp("/"+analyzer+"_.*GamGamToLL_GR$");
  samples["aa"]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim",kYellow+1)+TRegexp("/"+analyzer+"_.*GamGamToLL$");

  samples["amc"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+analyzer+"_.*DYJets$");
  samples["amcS19"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+analyzer+"_.*DYJets_Summer19$");
  samples["amcJet"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+analyzer+"_.*DY[0-9]Jets$");
  samples["amcPt"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+analyzer+"_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
  samples["amcM"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+analyzer+"_.*DYJets_M-[0-9]*to[0-9Inf]*$");
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+analyzer+"_.*DYJets_MG$");
  samples["powheg"]=Sample("#gamma*/Z#rightarrowll (POWHEG)","SAMPLE sim dy",kRed)+TRegexp("/"+analyzer+"_.*ZToEE_M_50_120$");
  samples["mi"]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy",kRed)+TRegexp("/"+analyzer+"_.*DYJetsTo[EMuTau]*_MiNNLO$");
  TString dysamples[]={"amc","amcJet","amcPt","amcM","mg","mi"};
  for(auto dysample:dysamples){
    samples["tau_"+dysample]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SAMPLE sim",kGreen)+dysample);
    samples["lhe_"+dysample]="lhe_"%(Sample("#gamma*/Z#rightarrowll (LHE)","SAMPLE sim dy",kBlue)+dysample);
    samples["gen_"+dysample]="gen_"%(Sample("#gamma*/Z#rightarrowll (GEN)","SAMPLE sim dy",kGreen)+dysample);
    samples["genfid_"+dysample]="genfid_"%(Sample("#gamma*/Z#rightarrowll (GEN fiducial)","SAMPLE sim dy",kMagenta)+dysample);
    samples["truth_"+dysample]="truth_"%(Sample("#gamma*/Z#rightarrowll (truth)","SAMPLE sim dy",kCyan)+dysample);
    samples["ss_"+dysample]="ss_"%(Sample("QCD multi-jet","SUM",kCyan)+"data"-dysample-("tau_"+dysample)-"vv"-"wjets"-"tt"-"tw"-"aa");
  }
    
  samples["ss"]="ss_"%(Sample("QCD multi-jet","SUM",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"tt"-"tw");

  samples["amcPt_stack"]=Sample("DY Pt-binned","STACK",kBlue)+TRegexp("/"+analyzer+"_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
  for(auto& sub:samples["amcPt_stack"].subs) sub.title=sub.title(TRegexp("Pt-[0-9]*To[0-9Inf]*"));
  samples["amcM_stack"]=Sample("DY M-binned","STACK",kBlue)+TRegexp("/"+analyzer+"_.*DYJets_M-[0-9]*to[0-9Inf]*$");
  for(auto& sub:samples["amcM_stack"].subs) sub.title=sub.title(TRegexp("M-[0-9]*to[0-9Inf]*"));

  samples["miel_nodata"]=Sample("SM Prediction","SUM",Style(kRed,-1,1001,"e2"),Style(kGreen,-1,1001,"e2"),Style(kBlue,-1,1001,"e2"),Style(kMagenta,-1,1001,"e2"))+"mi"+"tau_mi"+"vv"+"wjets"+"tttw"+"ss_mi";
  samples["mimu_nodata"]=Sample("SM Prediction","SUM",Style(kRed,-1,1001,"e2"),Style(kGreen,-1,1001,"e2"),Style(kBlue,-1,1001,"e2"),Style(kMagenta,-1,1001,"e2"))+"mi"+"tau_mi"+"vv"+"wjets"+"tttw"+1.7*samples["ss_mi"];

  /*
  samples["private"]=Sample("aMC@NLO private",Sample::Type::A,Style(kBlue,20));
  samples["private"].replace["([em][em])20[0-9][0-9]"]="$1";
  samples["private"].files.push_back(make_tuple("/data8/Users/hsseo/GeneratorTools/Hist/DY4D_MG_DY_NLO0_aew$",1,"",""));
  samples["private_correct"]=(Sample("correct direction",Sample::Type::A,Style(kRed,20))+"private")%"_correct";
  samples["private_0"]=(Sample("PDG sin^{2}#theta",Sample::Type::A,Style(kRed,20))+"private")%"_0";
  */
  Setup(mode_);
}
AFBPlotter::~AFBPlotter(){}

int AFBPlotter::Setup(TString mode_){
  Reset();
  if(mode_=="amcel") mode="data ^amc+tau_amc+vv+wjets+tttw+ss_amc+aa";
  else if(mode_=="amcmu") mode="data ^amc+tau_amc+vv+wjets+tttw+1.7*ss_amc+aa";
  else if(mode_=="mgel") mode="data ^mg+tau_mg+vv+wjets+tttw+ss_mg+aa";
  else if(mode_=="mgmu") mode="data ^mg+tau_mg+vv+wjets+tttw+1.7*ss_mg+aa";
  else if(mode_=="miel") mode="data ^mi+tau_mi+vv+wjets+tttw+ss_mi+aa";
  else if(mode_=="mimu") mode="data ^mi+tau_mi+vv+wjets+tttw+1.7*ss_mi+aa";
  else {
    mode=mode_;
  }

  SetupEntries(mode);
  SetupSystematics();

  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[AFBPlotter::Setup] nentries: "<<entries.size()<<endl;
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[AFBPlotter::Setup] nsys: "<<systematics.size()<<endl;
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[AFBPlotter::Setup] nplot: "<<plots.size()<<endl;

  return 1;
}
  
void AFBPlotter::SetupSystematics(){
  if(Verbosity>VERBOSITY::WARNING)  std::cout<<"[AFBPlotter::SetupSystematics]"<<endl;
  AddSystematic("efficiencySF_stat","efficiencySF_stat",Systematic::Type::GAUSSIAN,FormRange("_efficiencySF_stat%d",Range(20)),"sim");
  AddSystematic("electronRECOSF_sys1","electronRECOSF_sys1",Systematic::Type::ENVELOPE,"_electronRECOSF_s1_m0","sim");
  AddSystematic("electronRECOSF_sys2","electronRECOSF_sys2",Systematic::Type::ENVELOPE,"_electronRECOSF_s2_m0","sim");
  AddSystematic("electronRECOSF_sys3","electronRECOSF_sys3",Systematic::Type::ENVELOPE,"_electronRECOSF_s3_m0","sim");
  AddSystematic("electronRECOSF_sys4","electronRECOSF_sys4",Systematic::Type::ENVELOPE,"_electronRECOSF_s4_m0","sim");
  AddSystematic("electronRECOSF_sys","electronRECOSF_sys",Systematic::Type::MULTI,"electronRECOSF_sys1 electronRECOSF_sys2 electronRECOSF_sys3 electronRECOSF_sys4");
  AddSystematic("electronIDSF_sys1","electronIDSF_sys1",Systematic::Type::ENVELOPE,"_electronIDSF_s1_m0","sim");
  AddSystematic("electronIDSF_sys2","electronIDSF_sys2",Systematic::Type::ENVELOPE,"_electronIDSF_s2_m0","sim");
  AddSystematic("electronIDSF_sys3","electronIDSF_sys3",Systematic::Type::ENVELOPE,"_electronIDSF_s3_m0","sim");
  AddSystematic("electronIDSF_sys4","electronIDSF_sys4",Systematic::Type::ENVELOPE,"_electronIDSF_s4_m0","sim");
  AddSystematic("electronIDSF_sys","electronIDSF_sys",Systematic::Type::MULTI,"electronIDSF_sys1 electronIDSF_sys2 electronIDSF_sys3 electronIDSF_sys4");
  AddSystematic("muonIDSF_sys1","muonIDSF_sys1",Systematic::Type::ENVELOPE,"_muonIDSF_s1_m0 _muonIDSF_s1_m1","sim");
  AddSystematic("muonIDSF_sys2","muonIDSF_sys2",Systematic::Type::ENVELOPE,"_muonIDSF_s2_m0 _muonIDSF_s2_m1","sim");
  AddSystematic("muonIDSF_sys3","muonIDSF_sys3",Systematic::Type::ENVELOPE,"_muonIDSF_s3_m0 _muonIDSF_s3_m1","sim");
  AddSystematic("muonIDSF_sys4","muonIDSF_sys4",Systematic::Type::ENVELOPE,"_muonIDSF_s4_m0 _muonIDSF_s4_m1","sim");
  AddSystematic("muonIDSF_sys","muonIDSF_sys",Systematic::Type::MULTI,"muonIDSF_sys1 muonIDSF_sys2 muonIDSF_sys3 muonIDSF_sys4");
  AddSystematic("triggerSF_sys1","triggerSF_sys1",Systematic::Type::ENVELOPE,"_triggerSF_s1_m0 _triggerSF_s1_m1","sim");
  AddSystematic("triggerSF_sys2","triggerSF_sys2",Systematic::Type::ENVELOPE,"_triggerSF_s2_m0 _triggerSF_s2_m1","sim");
  AddSystematic("triggerSF_sys3","triggerSF_sys3",Systematic::Type::ENVELOPE,"_triggerSF_s3_m0 _triggerSF_s3_m1","sim");
  AddSystematic("triggerSF_sys4","triggerSF_sys4",Systematic::Type::ENVELOPE,"_triggerSF_s4_m0 _triggerSF_s4_m1","sim");
  AddSystematic("triggerSF_sys","triggerSF_sys",Systematic::Type::MULTI,"triggerSF_sys1 triggerSF_sys2 triggerSF_sys3 triggerSF_sys4");
  AddSystematic("efficiencySF","efficiencySF",Systematic::Type::MULTI,"efficiencySF_stat electronRECOSF_sys electronIDSF_sys muonIDSF_sys triggerSF_sys");
  AddSystematic("PUweight","PUweight",Systematic::Type::ENVELOPE,"_PUweight_up _PUweight_down","sim");
  AddSystematic("prefireweight","prefiring unc.",Systematic::Type::ENVELOPE,"_prefireweight_up _prefireweight_down","sim");
  AddSystematic("CFSF","charge-flip",Systematic::Type::ENVELOPE,"_CFSF_up _CFSF_down","sim");
  AddSystematic("btagh","b tagging (heavy)",Systematic::Type::ENVELOPE,"_btagSF_hup _btagSF_hdown","sim");
  AddSystematic("btagl","b tagging (light)",Systematic::Type::ENVELOPE,"_btagSF_lup _btagSF_ldown","sim");
  AddSystematic("btag_old","b tagging",Systematic::Type::ENVELOPE,"_btagSF_up _btagSF_down","sim");
  AddSystematic("btag","b tagging",Systematic::Type::MULTI,"btagh btagl");
  AddSystematic("scale","scale",Systematic::Type::ENVELOPE,"_scale_up _scale_down","sim");
  AddSystematic("smear","smear",Systematic::Type::ENVELOPE,"_smear_up _smear_down","sim");
  AddSystematic("JER","JER",Systematic::Type::ENVELOPE,"_JER_up _JER_down","sim");
  AddSystematic("JES","JES",Systematic::Type::ENVELOPE,"_JES_up _JES_down","sim");
  AddSystematic("uncl","uncl",Systematic::Type::ENVELOPE,"_uncl_up _uncl_down","sim");
  AddSystematic("metsys","totalsys",Systematic::Type::MULTI,"JER uncl JES");

  AddSystematic("dyscale","#mu_{R}/#mu_{F}",Systematic::Type::ENVELOPE,"_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8","dy");
  AddSystematic("dyalphaS","alphaS",Systematic::Type::ENVELOPE,"_alphaS_up _alphaS_down","dy");
  AddSystematic("dypdf","PDF",Systematic::Type::HESSIAN,FormRange("_pdf%d",Range(100)),"dy");

  AddSystematic("ttscale","#mu_{R}/#mu_{F}",Systematic::Type::ENVELOPE,"_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8","tt");
  AddSystematic("ttalphaS","alphaS",Systematic::Type::ENVELOPE,"_alphaS_up _alphaS_down","tt");
  AddSystematic("ttpdf","PDF",Systematic::Type::HESSIAN,FormRange("_pdf%d",Range(100)),"tt");

  AddSystematic("noRECOSF","noRECOSF",Systematic::Type::ENVELOPE,"_noRECOSF","sim");
  AddSystematic("noIDSF","noIDSF",Systematic::Type::ENVELOPE,"_noIDSF","sim");
  AddSystematic("noISOSF","noISOSF",Systematic::Type::ENVELOPE,"_noISOSF","sim");
  AddSystematic("notriggerSF","notriggerSF",Systematic::Type::ENVELOPE,"_notriggerSF","sim");
  AddSystematic("noPUweight","noPUweight",Systematic::Type::ENVELOPE,"_noPUweight","sim");
  AddSystematic("noprefireweight","noprefireweight",Systematic::Type::ENVELOPE,"_noprefireweight","sim");
  AddSystematic("nozptcor","nozptcor",Systematic::Type::ENVELOPE,"_nozptcor","dy");
  AddSystematic("noefficiencySF","noefficiencySF",Systematic::Type::ENVELOPE,"_noefficiencySF","sim");
  AddSystematic("IDSF_POG","IDSF_POG",Systematic::Type::ENVELOPE,"_IDSF_POG","sim");
  AddSystematic("selective","selective",Systematic::Type::ENVELOPE,"_selective");
  //AddSystematic("totalsys","totalsys",Systematic::Type::MULTI,0,"RECOSF IDSF ISOSF triggerSF PUweight prefireweight scale smear alphaS scalevariation pdf nozptcor");
   
  AddSystematic("sys","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight btag CFSF efficiencySF");
  AddSystematic("dytheory","theory (DY)",Systematic::Type::MULTI,"dyalphaS dyscale dypdf");
  AddSystematic("tttheory","theory (t#bar{t})",Systematic::Type::MULTI,"ttalphaS ttscale ttpdf");
  AddSystematic("totalsys","syst. unc.",Systematic::Type::MULTI,"sys dytheory tttheory");
  AddSystematic("test","totalsys",Systematic::Type::MULTI,"btag dyscale ttscale");
}

void AFBPlotter::SetupTH4D(){
  if(TClass::GetClass("TH4D")==NULL){
    gROOT->ProcessLine(".L TH4D/TH4D.cxx+");
  }
}
  
#endif
