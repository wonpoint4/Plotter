#ifndef __AFBPLOTTER_CC__
#define __AFBPLOTTER_CC__
#include"Plotter.cc"
class AFBPlotter:public Plotter{
public:
  void SetupSystematics();
  int Setup(TString mode_);
  TString mode;
  TString analyzer;
  AFBPlotter(TString mode_="data ^amc+tau_amc+vv+wjets+tttw+aa");

  void SetupTH4D();
  pair<double,double> GetRange(TString histname,TString axisname);
  pair<double,double> GetAFBAndError(TH1* costhetaCS);

  using Plotter::GetHist;
  virtual TH1* GetHist(const Sample& sample,Plot plot,TString additional_option="");
  virtual void GetHistActionForAdditionalClass(TObject*& obj,Plot plot);
  virtual Plot MakePlot(TString name,TString option="");

  TH1* GetHistWeightedAFB(TH1* hist_forward_num,TH1* hist_forward_den,TH1* hist_backward_num,TH1* hist_backward_den);
  TH1* GetHistAFB(TH1* hist_forward,TH1* hist_backward);

  //TCanvas* DrawPlot(TString plotkey,TString option="");
  map<TString,TString> plot_axisnames;
};
/*
TCanvas* AFBPlotter::DrawPlot(TString plotkey,TString option=""){
  TCanvas* c=Plotter::DrawPlot(plotkey,option);
  if(c){
    Plot plot=plots[plotkey];
    plot.SetOption(option);
    for(auto [histname,axisname]:plot_axisnames){
      if(plot.name.Contains(TRegexp("/[^/]*"+histname))){
	if(plot.IsMultiPad()){
	  c->GetPad(2)->cd();
	}else{
	  c->cd();
	}
	TH1* axisowner=GetAxisParent(gPad);
	if(axisowner){
	  if(plot.histname.Contains(TRegexp("^mm[0-9]"))) axisname.ReplaceAll("ll","#mu#mu");
	  else if(plot.histname.Contains(TRegexp("^ee[0-9]"))) axisname.ReplaceAll("ll","ee");
	  axisowner->GetXaxis()->SetTitle(axisname);
	  gPad->Update();
	  gPad->Modified();
	}
      }
    }
    if(plot.IsMultiPad()){
      c->GetPad(1)->cd();
    }else{
      c->cd();
    }
    
    TH1* axisowner=GetAxisParent(gPad);
    if(axisowner){
      if(plot.name.Contains("AFB")) axisowner->GetYaxis()->SetTitle("A_{FB}"); 
      else if(plot.option.Contains("norm")) axisowner->GetYaxis()->SetTitle("Normalized");
      else if(plot.option.Contains("widthweight")) axisowner->GetYaxis()->SetTitle("Events / 1 GeV");
      //else axisowner->GetYaxis()->SetTitle(Form("Events / %g GeV",axisowner->GetBinWidth(1)));
      else axisowner->GetYaxis()->SetTitle("Events");
      
      if(gPad->GetPrimitive("TPave")){
	TLegend* leg=(TLegend*)gPad->GetPrimitive("TPave");
	for(auto obj:*leg->GetListOfPrimitives()){
	  TLegendEntry* entry=(TLegendEntry*)obj;
	  TString label=entry->GetLabel();
	  if(label.Contains("#rightarrowll")){
	    if(plot.histname.Contains(TRegexp("^mm[0-9]"))) label.ReplaceAll("ll","#mu#mu");
	    else if(plot.histname.Contains(TRegexp("^ee[0-9]"))) label.ReplaceAll("ll","ee");
	    entry->SetLabel(label);
	    break;
	  }
	}
      }

      //axisowner->SetTitle("");
      TLatex latex;
      latex.SetTextSize(0.07);
      latex.SetNDC();
      if(!(plot.IsMultiPad())) latex.SetTextSize(0.04);
      //latex.DrawLatex(0.17,0.92,"CMS #bf{#it{Preliminary}}");
      if(plot.histname.Contains(TRegexp("^[a-z]*2016/"))){
	//latex.DrawLatex(0.6,0.92,"35.92 fb^{-1} (13 TeV)");
      }else if(plot.histname.Contains(TRegexp("^[a-z]*2017/"))){
	//latex.DrawLatex(0.6,0.92,"41.53 fb^{-1} (13 TeV)");
      }else if(plot.histname.Contains(TRegexp("^[a-z]*2018/"))){
	//latex.DrawLatex(0.6,0.92,"59.74 fb^{-1} (13 TeV)");
      }	
      
      gPad->Update();
      gPad->Modified();
    }
  }
  return c;
}
*/
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
  }else if(plot.histname.Contains("AFB")){
    TH1* hist_forward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFB","costhetaCS")+" Umin:0 Umax:1");
    TH1* hist_backward=GetHist(sample,plot,"histname:"+Replace(plot.histname,"AFB","costhetaCS")+" Umin:-1 Umax:0");
    hist=GetHistAFB(hist_forward,hist_backward);
  }
  if(hist){
    sample.ApplyStyle(hist);
    hist->SetTitle(plot.name+plot.suffix);
    hist->SetDirectory(pdir);
    return hist;
  }else{
    return Plotter::GetHist(sample,plot);
  }
}
void AFBPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot){
  PAll("[AFBPlotter::GetHistActionForAdditionalClass(TObject*& obj,Plot plot)");
  if(strstr(obj->ClassName(),"TH4D")!=NULL){
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

AFBPlotter::AFBPlotter(TString mode_){
  SetupTH4D();
  ScanFiles(TString()+getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/AFBAnalyzer/");
  
  samples["data"]=Sample("data",Sample::Type::DATA,kBlack,20)+TRegexp("/DATA/AFBAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]")+TRegexp("/DATA/AFBAnalyzer_SkimTree_Dilepton_.*EG.*_[A-Z]")+TRegexp("/DATA/AFBAnalyzer_.*Electron.*_[A-Z]");
  samples["mm2016"]=Sample("data (#mu#mu2016)",Sample::Type::DATA,kBlack,20)+TRegexp("2016.*/DATA/AFBAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["mm2017"]=Sample("data (#mu#mu2017)",Sample::Type::DATA,kRed,20)+TRegexp("2017.*/DATA/AFBAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["mm2018"]=Sample("data (#mu#mu2018)",Sample::Type::DATA,kBlue,20)+TRegexp("2018.*/DATA/AFBAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["ee2016"]=Sample("data (ee2016)",Sample::Type::DATA,kBlack,22)+TRegexp("2016.*/DATA/AFBAnalyzer_SkimTree_Dilepton_.*EG.*_[A-Z]");
  samples["ee2017"]=Sample("data (ee2017)",Sample::Type::DATA,kRed,22)+TRegexp("2017.*/DATA/AFBAnalyzer_SkimTree_Dilepton_.*EG.*_[A-Z]");
  samples["ee2018"]=Sample("data (ee2018)",Sample::Type::DATA,kBlue,22)+TRegexp("2018.*/DATA/AFBAnalyzer_SkimTree_Dilepton_.*EG.*_[A-Z]");
  samples["mm"]=Sample("data (#mu#mu)",Sample::Type::DATA,kBlack,20)+TRegexp("/DATA/AFBAnalyzer_SkimTree_Dilepton_DoubleMuon_[A-Z]");
  samples["ee"]=Sample("data (ee)",Sample::Type::DATA,kRed,22)+TRegexp("/DATA/AFBAnalyzer_SkimTree_Dilepton_.*EG.*_[A-Z]");

  samples["ww"]=Sample("WW",Sample::Type::BG,kBlue)+TRegexp("/AFBAnalyzer_.*WW_pythia");
  samples["wz"]=Sample("WZ",Sample::Type::BG,kGreen)+TRegexp("/AFBAnalyzer_.*WZ_pythia");
  samples["zz"]=Sample("ZZ",Sample::Type::BG,kCyan)+TRegexp("/AFBAnalyzer_.*ZZ_pythia");
  samples["vv"]=Sample("Diboson",Sample::Type::BG,kBlue)+TRegexp("/AFBAnalyzer_.*[W-Z][W-Z]_pythia");
  samples["wjets"]=Sample("W",Sample::Type::BG,kYellow)+TRegexp("/AFBAnalyzer_.*WJets_MG");
  samples["tt"]=Sample("t#bar{t}",Sample::Type::BG,kMagenta)+TRegexp("/AFBAnalyzer_.*TTLL_powheg");
  samples["ttlj"]=Sample("TTLJ",Sample::Type::BG,kMagenta+6)+TRegexp("/AFBAnalyzer_.*TTLJ_powheg");
  samples["tw"]=Sample("tW",Sample::Type::BG,kMagenta+10)+TRegexp("/AFBAnalyzer_.*SingleTop_tW_.*top_NoFullyHad");
  samples["st"]=Sample("ST",Sample::Type::BG,kMagenta+12)+TRegexp("/AFBAnalyzer_.*SingleTop_[st]ch_.*");
  samples["tttw"]=Sample("t#bar{t}, tW",Sample::Type::SUM,kMagenta)+"tt"+"tw";
  samples["qcdm"]=Sample("QCD Mu-enriched",Sample::Type::BG,kCyan)+TRegexp("/AFBAnalyzer_.*QCD_Pt.*MuEnrichedPt5$");
  samples["qcdm1"]=Sample("QCD Mu-enriched 10to20",Sample::Type::BG,1)+TRegexp("/AFBAnalyzer_.*QCD_Pt-15to20.*MuEnrichedPt5$");
  samples["qcdm2"]=Sample("QCD Mu-enriched 20to30",Sample::Type::BG,2)+TRegexp("/AFBAnalyzer_.*QCD_Pt-20to30.*MuEnrichedPt5$");
  samples["qcdm3"]=Sample("QCD Mu-enriched 30to50",Sample::Type::BG,3)+TRegexp("/AFBAnalyzer_.*QCD_Pt-30to50.*MuEnrichedPt5$");
  samples["qcdm4"]=Sample("QCD Mu-enriched 50to80",Sample::Type::BG,4)+TRegexp("/AFBAnalyzer_.*QCD_Pt-50to80.*MuEnrichedPt5$");
  samples["qcdm5"]=Sample("QCD Mu-enriched 80to120",Sample::Type::BG,5)+TRegexp("/AFBAnalyzer_.*QCD_Pt-80to120.*MuEnrichedPt5$");
  samples["qcdm6"]=Sample("QCD Mu-enriched 120to170",Sample::Type::BG,6)+TRegexp("/AFBAnalyzer_.*QCD_Pt-120to170.*MuEnrichedPt5$");
  samples["qcde"]=Sample("QCD EM-enriched",Sample::Type::BG,kCyan+4)+TRegexp("/AFBAnalyzer_.*QCD_Pt.*EMEnriched$");
  samples["qcdee"]=Sample("QCD bcToE",Sample::Type::BG,kCyan)+TRegexp("/AFBAnalyzer_.*QCD_Pt.*bcToE$");

  samples["aa0j"]=Sample("#gamma#gamma#rightarrowll",Sample::Type::BG,kMagenta+12)+TRegexp("/AFBAnalyzer_.*GamGamToLL_0j$");
  samples["aa"]=Sample("#gamma#gamma#rightarrowll",Sample::Type::BG,kYellow+1)+TRegexp("/AFBAnalyzer_.*GamGamToLL$");

  samples["amc"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/AFBAnalyzer_.*DYJets$");
  samples["amcJet"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/AFBAnalyzer_.*DY[0-9]Jets$");
  samples["amcPt"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/AFBAnalyzer_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
  samples["amcM"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/AFBAnalyzer_.*DYJets_M-[0-9]*to[0-9Inf]*$");
  samples["mg"]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp("/AFBAnalyzer_.*DYJets_MG$");
  samples["powheg"]=Sample("#gamma*/Z#rightarrowll (POWHEG)",Sample::Type::SIGNAL,kRed)+TRegexp("/AFBAnalyzer_.*ZToEE_M_50_120$");
  TString dysamples[]={"amc","amcJet","amcPt","amcM","mg"};
  for(auto dysample:dysamples){
    samples["tau_"+dysample]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,kGreen)+dysample);
    samples["lhe_"+dysample]="lhe_"%(Sample("#gamma*/Z#rightarrowll (LHE)",Sample::Type::SIGNAL,kBlue)+dysample);
    samples["gen_"+dysample]="gen_"%(Sample("#gamma*/Z#rightarrowll (GEN)",Sample::Type::SIGNAL,kGreen)+dysample);
    samples["genfid_"+dysample]="genfid_"%(Sample("#gamma*/Z#rightarrowll (GEN fiducial)",Sample::Type::SIGNAL,kMagenta)+dysample);
    samples["truth_"+dysample]="truth_"%(Sample("#gamma*/Z#rightarrowll (truth)",Sample::Type::SIGNAL,kCyan)+dysample);
  }
    
  samples["ss"]="ss_"%(Sample("QCD multi-jet",Sample::Type::SUM,kCyan)+"data"-"amc"-"tau_amc"-"vv"-"wjets"-"tt"-"tw"-"aa");

  samples["amcPt_stack"]=Sample("DY Pt-binned",Sample::Type::STACK,kBlue)+TRegexp("/AFBAnalyzer_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
  for(auto& sub:samples["amcPt_stack"].subs) sub.title=sub.title(TRegexp("Pt-[0-9]*To[0-9Inf]*"));
  samples["amcM_stack"]=Sample("DY M-binned",Sample::Type::STACK,kBlue)+TRegexp("/AFBAnalyzer_.*DYJets_M-[0-9]*to[0-9Inf]*$");
  for(auto& sub:samples["amcM_stack"].subs) sub.title=sub.title(TRegexp("M-[0-9]*to[0-9Inf]*"));

  /*
  samples["private"]=Sample("aMC@NLO private",Sample::Type::A,Style(kBlue,20));
  samples["private"].replace["([em][em])20[0-9][0-9]"]="$1";
  samples["private"].files.push_back(make_tuple("/data8/Users/hsseo/GeneratorTools/Hist/DY4D_MG_DY_NLO0_aew$",1,"",""));
  samples["private_correct"]=(Sample("correct direction",Sample::Type::A,Style(kRed,20))+"private")%"_correct";
  samples["private_0"]=(Sample("PDG sin^{2}#theta",Sample::Type::A,Style(kRed,20))+"private")%"_0";
  */
  plot_axisnames["dimass"]="m(ll) [GeV]";
  plot_axisnames["dirap"]="y(ll)";
  plot_axisnames["dipt"]="p_{T}(ll) [GeV]";
  plot_axisnames["costhetaCS"]="cos#theta_{CS}";
  plot_axisnames["AFB(m)"]="m(ll) [GeV]";
  plot_axisnames["AFB(y)"]="y(ll)";
  plot_axisnames["AFB(pt)"]="p_{T}(ll) [GeV]";

  Setup(mode_);
}

int AFBPlotter::Setup(TString mode_){
  Reset();
  if(mode_=="dab"||mode_=="data:amc+bg") mode="data:amc+bg";
  else if(mode_=="dabs"||mode_=="data:amc+bg+ss") mode="data:amc+bg+ss";
  else if(mode_=="dba"||mode_=="data-bg:amc") mode="data-bg:amc";
  else if(mode_=="dajb"||mode_=="data:amcJet+bg") mode="data:amcJet+bg";
  else if(mode_=="dajbs"||mode_=="data:amcJet+bg+ss") mode="data:amcJet+bg+ss";
  else if(mode_=="dbaj"||mode_=="data-bg:amcJet") mode="data-bg:amcJet";
  else if(mode_=="damb"||mode_=="data:amcM+bg") mode="data:amcM+bg";
  else if(mode_=="m"||mode_=="muon") mode="muon";
  else if(mode_=="e"||mode_=="electron") mode="electron";
  else if(mode_=="ae"||mode_=="amc_electron") mode="amc_electron";
  else if(mode_=="aje"||mode_=="amcJet_electron") mode="amcJet_electron"; 
  else if(mode_=="am"||mode_=="amc_muon") mode="amc_muon";
  else if(mode_=="ajm"||mode_=="amcJet_muon") mode="amcJet_muon";
  else if(mode_=="gae"||mode_=="genamc_electron") mode="genamc_electron";
  else if(mode_=="gam"||mode_=="genamc_muon") mode="genamc_muon";
  else if(mode_=="aga"||mode_=="amc:genamc") mode="amc:genamc";
  else if(mode_=="wr") mode="wr";
  else if(mode_=="appamm") mode="amc_pp:amc_mm";
  else if(mode_=="mc") mode="mc";
  else {
    mode=mode_;
    //cout<<"Invalid mode "<<mode_<<endl;
    //exit(1);
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
  systematics["RECOSF"]=MakeSystematic("RECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_RECOSF_up _RECOSF_down");
  systematics["IDSF"]=MakeSystematic("IDSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_IDSF_up _IDSF_down");
  systematics["ISOSF"]=MakeSystematic("ISOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_ISOSF_up _ISOSF_down");
  systematics["triggerSF"]=MakeSystematic("triggerSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_triggerSF_up _triggerSF_down");
  systematics["PUreweight"]=MakeSystematic("PUreweight",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_PUreweight_up _PUreweight_down");
  systematics["prefireweight"]=MakeSystematic("prefireweight",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_prefireweight_up _prefireweight_down");
  systematics["scale"]=MakeSystematic("scale",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_scale_up _scale_down");
  systematics["smear"]=MakeSystematic("smear",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_smear_up _smear_down");
  systematics["JER"]=MakeSystematic("JER",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_JER_up _JER_down");
  systematics["JES"]=MakeSystematic("JES",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_JES_up _JES_down");
  systematics["uncl"]=MakeSystematic("uncl",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_uncl_up _uncl_down");
  systematics["metsys"]=MakeSystematic("totalsys",Systematic::Type::MULTI,0,"JER uncl JES");
  systematics["alphaS"]=MakeSystematic("alphaS",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL),"_alphaS_up _alphaS_down");
  systematics["scalevariation"]=MakeSystematic("scalevariation",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL),"_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8");

  vector<TString> prefixes;
  for(int i=0;i<100;i++) prefixes.push_back(Form("_pdf%d",i));
  systematics["pdfh"]=MakeSystematic("pdfh",Systematic::Type::HESSIAN,(1<<Sample::Type::SIGNAL),prefixes);
  systematics["pdfg"]=MakeSystematic("pdfg",Systematic::Type::GAUSSIAN,(1<<Sample::Type::SIGNAL),prefixes);

  systematics["noRECOSF"]=MakeSystematic("noRECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noRECOSF");
  systematics["noIDSF"]=MakeSystematic("noIDSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noIDSF");
  systematics["noISOSF"]=MakeSystematic("noISOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noISOSF");
  systematics["notriggerSF"]=MakeSystematic("notriggerSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_notriggerSF");
  systematics["noPUreweight"]=MakeSystematic("noPUreweight",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noPUreweight");
  systematics["noprefireweight"]=MakeSystematic("noprefireweight",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noprefireweight");
  systematics["nozptcor"]=MakeSystematic("nozptcor",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL),"_nozptcor");
  systematics["noefficiencySF"]=MakeSystematic("noefficiencySF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_noefficiencySF");
  systematics["IDSF_POG"]=MakeSystematic("IDSF_POG",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_IDSF_POG");
  systematics["selective"]=MakeSystematic("selective",Systematic::Type::ENVELOPE,(1<<Sample::Type::DATA)+(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_selective");
  systematics["efficiencySF"]=MakeSystematic("efficiencySF",Systematic::Type::MULTI,0,"RECOSF IDSF ISOSF triggerSF");
  systematics["totalsys"]=MakeSystematic("totalsys",Systematic::Type::MULTI,0,"RECOSF IDSF ISOSF triggerSF PUreweight prefireweight scale smear alphaS scalevariation pdf nozptcor");
}

void AFBPlotter::SetupTH4D(){
  if(TClass::GetClass("TH4D")==NULL){
    gROOT->ProcessLine(".L TH4D/TH4D.cxx+");
  }
}
  
#endif
