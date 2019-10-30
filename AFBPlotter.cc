#ifndef __AFBPLOTTER_CC__
#define __AFBPLOTTER_CC__
#include"Plotter.cc"
class AFBPlotter:public Plotter{
public:
  void SetupEntries();
  void SetupSystematics();
  //void SetupPlots();
  //void SavePlotsCondor(int channel,int year,int njob);
  int Setup(TString mode_);
  TString mode;
  TString analyzer;
  map<TString,vector<double>> binning;
  map<TString,TString> binformat;
  AFBPlotter(TString mode_="dab",TString flag="");

  double GetChi2(TH1* h1,TH1* h2=NULL);
  void DetectBinning();
  TH1* GetHistRaw(TString filename,TString histname);
  pair<double,double> GetRange(TString histname,TString axisname);
  pair<double,double> GetAFBAndError(TH1* costhetaCS);
  TH1* GetHist(const Sample& sample,TString histname,TString suffix="",int varibit=0);
  TCanvas* DrawPlot(TString plotkey,TString option="");
  map<TString,TString> plot_axisnames;
};
TCanvas* AFBPlotter::DrawPlot(TString plotkey,TString option=""){
  TCanvas* c;
  if(option.Contains("nodata")){
    Sample data;
    int i;
    bool flag=false;
    for(i=0;i<entries.size();i++){
      if(entries[i].title=="data"){
	data=entries[i];
	entries.erase(entries.begin()+i);
	flag=true;
	break;
      }
    }
    c=Plotter::DrawPlot(plotkey,option);
    if(flag) entries.insert(entries.begin()+i,data);
  }else c=Plotter::DrawPlot(plotkey,option);
  if(c){
    Plot plot=plots[plotkey];
    plot.SetOption(option);
    for(auto [histname,axisname]:plot_axisnames){
      if(plot.histname.Contains(TRegexp("/[^/]*"+histname))){
	if(plot.type==Plot::Type::CompareAndRatio||plot.type==Plot::Type::CompareAndDiff){
	  c->GetPad(2)->cd();
	}else{
	  c->cd();
	}
	TH1* axisowner=GetAxisParent(gPad);
	if(axisowner){
	  if(plot.histname.Contains(TRegexp("^muon"))) axisname.ReplaceAll("ll","#mu#mu");
	  else if(plot.histname.Contains(TRegexp("^electron"))) axisname.ReplaceAll("ll","ee");
	  axisowner->GetXaxis()->SetTitle(axisname);
	  gPad->Update();
	  gPad->Modified();
	}
      }
    }
    if(plot.type==Plot::Type::CompareAndRatio||plot.type==Plot::Type::CompareAndDiff){
      c->GetPad(1)->cd();
    }else{
      c->cd();
    }

    TH1* axisowner=GetAxisParent(gPad);
    if(axisowner){
      if(plot.histname.Contains("AFB")) axisowner->GetYaxis()->SetTitle("A_{FB}"); 
      else if(plot.option.Contains("norm")) axisowner->GetYaxis()->SetTitle("Normalized");
      else if(plot.option.Contains("widthweight")) axisowner->GetYaxis()->SetTitle("Events / 1 GeV");
      else axisowner->GetYaxis()->SetTitle(Form("Events / %g GeV",axisowner->GetBinWidth(1)));

      if(gPad->GetPrimitive("TPave")){
	TLegend* leg=(TLegend*)gPad->GetPrimitive("TPave");
	for(auto obj:*leg->GetListOfPrimitives()){
	  TLegendEntry* entry=(TLegendEntry*)obj;
	  TString label=entry->GetLabel();
	  if(label.Contains("#rightarrowll")){
	    if(plot.histname.Contains(TRegexp("^muon"))) label.ReplaceAll("ll","#mu#mu");
	    else if(plot.histname.Contains(TRegexp("^electron"))) label.ReplaceAll("ll","ee");
	    entry->SetLabel(label);
	    break;
	  }
	}
      }

      axisowner->SetTitle("");
      TLatex latex;
      latex.SetTextSize(0.07);
      latex.SetNDC();
      if(plot.type!=Plot::Type::CompareAndRatio&&plot.type!=Plot::Type::CompareAndDiff) latex.SetTextSize(0.04);
      latex.DrawLatex(0.17,0.92,"CMS #bf{#it{Preliminary}}");
      if(plot.histname.Contains(TRegexp("^[a-z]*2016/"))&&!plot.option.Contains("nodata")){
	latex.DrawLatex(0.6,0.92,"35.92 fb^{-1} (13 TeV)");
      }else if(plot.histname.Contains(TRegexp("^[a-z]*2017/"))&&!plot.option.Contains("nodata")){
	latex.DrawLatex(0.6,0.92,"41.53 fb^{-1} (13 TeV)");
      }else if(plot.histname.Contains(TRegexp("^[a-z]*2018/"))&&!plot.option.Contains("nodata")){
	latex.DrawLatex(0.6,0.92,"59.74 fb^{-1} (13 TeV)");
      }	
	
      gPad->Update();
      gPad->Modified();
    }
  }
  return c;
}

pair<double,double> AFBPlotter::GetRange(TString histname,TString axisname){
  TString rangestring=histname(TRegexp(axisname+"[[0-9.]*,[0-9.]*]"));
  TString first=rangestring(axisname.Length()+1,rangestring.Index(',')-axisname.Length()-1);
  TString second=rangestring(rangestring.Index(',')+1,rangestring.Length()-rangestring.Index(',')-2);
  return make_pair(first.Atof(),second.Atof());
}  

TH1* AFBPlotter::GetHistRaw(TString filename,TString histname){
  //if(DEBUG>3) cout<<"###DEBUG### AFBPlotter::GetHistRaw(TString filename,TString histname)"<<endl;
  TString syear=histname(TRegexp("201[6-8]"));
  if(!filename.Contains(syear)) return NULL;
    
  TList* hlist=new TList;
  for(const auto& [axisname,axisbin]:binning){
    bool breakflag=false;
    pair<double,double> range=GetRange(histname,axisname);
    for(int i=0;i<axisbin.size()-1;i++){
      if(range.first==axisbin[i]&&range.second==axisbin[i+1]) break;
      if(range.first<=axisbin[i]&&range.second>=axisbin[i+1]){
	TString this_histname=Replace(histname,TRegexp(axisname+"[[0-9.]*,[0-9.]*]"),Form("%s[%s,%s]",axisname.Data(),Form(binformat[axisname],binning[axisname][i]),Form(binformat[axisname],binning[axisname][i+1])));
	TH1* this_hist=GetHistRaw(filename,this_histname);
	if(this_hist) hlist->Add(this_hist);
	breakflag=true;
      }
    }
    if(breakflag) break;
  }
  TH1* hist=NULL;
  if(hlist->GetSize()){
    hist=(TH1*)hlist->At(0)->Clone();
    hist->Reset();
    hist->SetName(histname);
    hist->Merge(hlist);
    hist->SetDirectory(pdir);
  }else{
    hist=Plotter::GetHistRaw(filename,histname);
  }
  hlist->Delete();
  delete hlist;
  return hist;
  
}
TH1* AFBPlotter::GetHist(const Sample& sample,TString histname,TString suffix,int varibit){
  TString afbstring=histname(TRegexp("AFB([a-z]+)"));
  if(afbstring!=""){
    TString axisname=afbstring(4,afbstring.Length()-5);
    pair<double,double> range=GetRange(histname,axisname);
    TH1D* afbhist=new TH1D(sample.title,histname+suffix,binning[axisname].size()-1,&binning[axisname][0]);
    afbhist->SetDirectory(pdir);
    if(histname.Contains("weightedAFB("+axisname+")")) afbstring="weightedAFB("+axisname+")";
    for(int i=0;i<binning[axisname].size()-1;i++){
      if(range.first<=binning[axisname][i]&&range.second>=binning[axisname][i+1]){
	TString this_histname=Replace(histname,TRegexp(axisname+"[[0-9.]*,[0-9.]*]"),Form("%s[%s,%s]",axisname.Data(),Form(binformat[axisname],binning[axisname][i]),Form(binformat[axisname],binning[axisname][i+1])));
	TH1* hist_forward=GetHist(sample,Replace(this_histname,afbstring,"forward"),suffix,varibit);
	TH1* hist_backward=GetHist(sample,Replace(this_histname,afbstring,"backward"),suffix,varibit);
	if(hist_forward&&hist_backward){
	  double val=0,err=0;
	  hist_forward=GetTH1(hist_forward,true);
	  hist_backward=GetTH1(hist_backward,true);
	  if(histname.Contains("weightedAFB")){
	    double valfn=hist_forward->GetBinContent(2);
	    double valfd=hist_forward->GetBinContent(3);
	    double efd=hist_forward->GetBinError(3);
	    double valbn=hist_backward->GetBinContent(2);
	    double valbd=hist_backward->GetBinContent(3);
	    double ebd=hist_backward->GetBinError(3);
	    val=3./8.*(valfd-valbd)/(valfn+valbn);
	    err=3./8.*(valbn*valfd+valfn*valbd)/pow(valfn+valbn,2)*sqrt(pow(efd/valfd,2)+pow(ebd/valbd,2));
	  }else{
	    double valf=hist_forward->GetBinContent(1);
	    double ef=hist_forward->GetBinError(1);
	    double valb=hist_backward->GetBinContent(1);
	    double eb=hist_backward->GetBinError(1);
	    val=(valf-valb)/(valf+valb);
	    err=2*sqrt(ef*ef*valb*valb+eb*eb*valf*valf)/pow(valf+valb,2);
	  }
	  afbhist->SetBinContent(i+1,val);
	  afbhist->SetBinError(i+1,err);
	  delete hist_forward;
	  delete hist_backward;
	}
      }
    }
    sample.ApplyStyle(afbhist);
    afbhist->SetTitle(histname+suffix);
    return afbhist;
  }else{
    return Plotter::GetHist(sample,histname,suffix,varibit);
  }
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

  
void AFBPlotter::DetectBinning(){
  binning.clear();
  binformat.clear();
  map<TString,set<double>> edges;
  set<TString> histkeys=GetHistKeys(entries[0]);
  for(const auto& histkey:histkeys){
    int start=0;
    while(1){
      int extent;
      start=histkey.Index(TRegexp("[a-zA-Z]+[[0-9.]*,[0-9.]*]"),&extent,start);
      if(start==-1) break;
      TString match=histkey(start,extent);
      TString axisname=match(TRegexp("[a-zA-Z]*"));
      TString lowedge=match(match.Index('[')+1,match.Index(',')-match.Index('[')-1);
      TString highedge=match(match.Index(',')+1,match.Index(']')-match.Index(',')-1);
      edges[axisname].insert(lowedge.Atof());
      edges[axisname].insert(highedge.Atof());
      if(binformat[axisname]==""){
	if(lowedge.Index('.')==-1) binformat[axisname]="%.0f";
	else binformat[axisname]=Form("%s%df","%.",lowedge.Length()-lowedge.Index('.')-1);
      }
      start+=extent;
    }
  }
  for(const auto& [axisname,axisedges]:edges)
    for(const auto& edge:axisedges) 
      binning[axisname].push_back(edge);
}

AFBPlotter::AFBPlotter(TString mode_="dab",TString flag=""){
  analyzer="AFBAnalyzer";
  vector<TString> files=Split(gSystem->GetFromPipe("find $SKFlatOutputDir$SKFlatV/"+analyzer+"/ -type f"),"\n");
  for(const auto& file:files){
    TString year=file("/"+analyzer+"/[0-9]+/"); year=year(analyzer.Length()+2,4);
    TString key=Replace(file,TString()+getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/"+analyzer+"/","");
    Sample frag;
    frag.files.push_back(make_tuple(file,1.,"",""));
    samples[key]=frag;
  } 
  
  if(flag!="") flag="/"+flag+"__";
  vector<TString> syears={"2016","2017","2018"};
  for(const auto& syear:syears){
    samples["muon"+syear]=Sample("muon"+syear,Sample::Type::DATA,kBlack)+TRegexp(syear+flag+"/DATA/AFBAnalyzer_SkimTree_SMP_DoubleMuon_[A-Z]");
    samples["electron"+syear]=Sample("electron"+syear,Sample::Type::DATA,kBlack)+TRegexp(syear+flag+"/DATA/AFBAnalyzer_SkimTree_SMP_.*EG.*_[A-Z]");

    samples["amc"+syear]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_SMP_DYJets.root");
    samples["amctt"+syear]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::SIGNAL,kGreen)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_SMP_DYJets.root"));
    samples["genamc"+syear]=Sample("#gamma*/Z#rightarrowll (GEN)",Sample::Type::SIGNAL,kRed)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_GEN_DYJets.root");
    //samples["amc"+syear+"_nozptcor"]=Sample("amc"+syear+"_nozptcor",Sample::Type::SIGNAL,kRed,"amc"+syear,1.,"","_nozptcor"); //FIXME no 2018

    //samples["amcPt"+syear]=Sample("amcPt"+syear,Sample::Type::SIGNAL,kRed,TRegexp("SkimTree_SMP_DYJets_Pt-[0-9]+To[0-9]+_"+syear));
    
    samples["amcJet"+syear]=Sample("#gamma*/Z#rightarrowll",Sample::Type::SIGNAL,kRed)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_SMP_DY[0-9]Jets.root");
    samples["amcJettt"+syear]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::SIGNAL,kGreen)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_SMP_DY[0-9]Jets.root"));
    samples["genamcJet"+syear]=Sample("#gamma*/Z#rightarrowll (GEN)",Sample::Type::SIGNAL,kRed)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_GEN_DY[0-9]Jets.root");
    //samples["amcJet"+syear+"_nozptcor"]=Sample("amcJet"+syear+"_nozptcor",Sample::Type::SIGNAL,kRed,"amcJet"+syear,1.,"","_nozptcor");
    
    //samples["mg"+syear]=Sample("mg"+syear,Sample::Type::SIGNAL,kRed,TRegexp("SkimTree_SMP_DYJets_MG_"+syear));
    //samples["powhegmuon"+syear]=Sample("powhegmuon"+syear,Sample::Type::SIGNAL,kBlue,TRegexp("ZToMuMu_M_[0-9]+_[0-9]+_"+syear));
    //samples["powhegelectron"+syear]=Sample("powhegelectron"+syear,Sample::Type::SIGNAL,kBlue,TRegexp("ZToEE_M_[0-9]+_[0-9]+_"+syear));

    //samples["genamc"+syear]=Sample("genamc"+syear,Sample::Type::GEN,kRed,TRegexp("SkimTree_GEN_DYJets_"+syear),1.,"gen_"); //FIXME no 2018
    //samples["genamcJet"+syear]="gen_"%(Sample("genamcJet"+syear,Sample::Type::GEN,kRed,TRegexp("SkimTree_GEN_DY[0-9]Jets_"+syear),1.,"gen_"));
    //samples["truthamcJet"+syear]=Sample("truthamcJet"+syear,Sample::Type::GEN,kRed,TRegexp("SkimTree_SMP_DY[0-9]Jets_"+syear),1.,"gen_");
    //samples["genpowhegmuon"+syear]="gen_"%(Sample("powhegmuon"+syear,Sample::Type::SIGNAL,kBlue)+TRegexp("ZToMuMu_M_[0-9]+_[0-9]+_"+syear));
    //samples["genpowhegelectron"+syear]="gen_"%(Sample("genpowhegelectron"+syear,Sample::Type::SIGNAL,kBlue)+TRegexp("ZToEE_M_[0-9]+_[0-9]+_"+syear),1.,"gen_");
    
    //FIXME no 2018
    //if(syear!="2018") samples["amctt"+syear]=Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,EColor::kGreen,"SkimTree_SMP_DYJets_"+syear,"tau_");
    //FIXME no 2016
    //samples["mgtt"+syear]=Sample("#gamma*/Z#rightarrow#tau#tau",Sample::Type::BG,EColor::kGreen,TRegexp("SkimTree_SMP_DYJets_MG_"+syear),"tau_");

    samples["vv"+syear]=Sample("Diboson",Sample::Type::BG,kBlue)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_SMP_[W-Z][W-Z]_pythia");
    samples["wjets"+syear]=Sample("W",Sample::Type::BG,kYellow)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_SMP_WJets_MG");
    samples["tt"+syear]=Sample("t#bar{t}",Sample::Type::BG,kMagenta)+TRegexp(syear+flag+"/AFBAnalyzer_SkimTree_SMP_TTLL_powheg");
  }

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
  if(mode_=="dajb"||mode_=="data.amcJet+bg") mode="data.amcJet+bg";
  else if(mode_=="dajbs"||mode_=="data.amcJet+bg+ss") mode="data.amcJet+bg+ss";
  else if(mode_=="dab"||mode_=="data.amc+bg") mode="data.amc+bg";
  else if(mode_=="dabs"||mode_=="data.amc+bg+ss") mode="data.amc+bg+ss";
  else if(mode_=="dba"||mode_=="data-bg.amc") mode="data-bg.amc";
  else if(mode_=="dbaj"||mode_=="data-bg.amcJet") mode="data-bg.amcJet";
  else if(mode_=="m"||mode_=="muon") mode="muon";
  else if(mode_=="e"||mode_=="electron") mode="electron";
  else if(mode_=="ae"||mode_=="amc_electron") mode="amc_electron";
  else if(mode_=="aje"||mode_=="amcJet_electron") mode="amcJet_electron"; 
  else if(mode_=="am"||mode_=="amc_muon") mode="amc_muon";
  else if(mode_=="ajm"||mode_=="amcJet_muon") mode="amcJet_muon";
  else if(mode_=="gae"||mode_=="genamc_electron") mode="genamc_electron";
  else if(mode_=="gam"||mode_=="genamc_muon") mode="genamc_muon";
  else if(mode_=="aga"||mode_=="amc.genamc") mode="amc.genamc";
  else if(mode_=="wr") mode="wr";
  else if(mode_=="appamm") mode="amc_pp.amc_mm";
  else {
    cout<<"Invalid mode "<<mode_<<endl;
    exit(1);
  }

  SetupEntries();
  DetectBinning();
  SetupSystematics();
  SetupPlots("plot_AFBAnalyzer/"+mode+"/plot.dat");

  if(DEBUG) cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(DEBUG) cout<<"[Setup] nsys: "<<systematics.size()<<endl;
  if(DEBUG) cout<<"[Setup] nplot: "<<plots.size()<<endl;

  return 1;
}

void AFBPlotter::SetupEntries(){
  if(DEBUG)  cout<<"[AFBPlotter::SetupEntries]"<<endl;
  if(mode=="data.amcJet+bg"){
    entries.push_back(Sample("data",Sample::Type::DATA,kBlack)+TRegexp("^muon201[6-8]$")+TRegexp("^electron201[6-8]$"));
    entries.push_back(Sample("simulation",Sample::Type::STACK,kRed)+TRegexp("^amcJet201[6-8]$")+TRegexp("^amcJettt201[6-8]$")+TRegexp("^vv201[6-8]$")+TRegexp("^wjets201[6-8]$")+TRegexp("^tt201[6-8]$"));
  }else if(mode=="data.amcJet+bg+ss"){
    entries.push_back(Sample("data",Sample::Type::DATA,kBlack)+TRegexp("^muon201[6-8]$")+TRegexp("^electron201[6-8]$"));
    entries.push_back(Sample("simulation",Sample::Type::STACK,kRed)+TRegexp("^amcJet201[6-8]$")+TRegexp("^amcJettt201[6-8]$")+TRegexp("^vv201[6-8]$")+TRegexp("^wjets201[6-8]$")+TRegexp("^tt201[6-8]$"));
    Sample samesign="ss_"%(Sample("QCD dijet",Sample::Type::BG,kCyan)+TRegexp("^muon201[6-8]$")+TRegexp("^electron201[6-8]$")+(-1)*(Sample()+TRegexp("^amcJet201[6-8]$")+TRegexp("^amcJettt201[6-8]$")+TRegexp("^vv201[6-8]$")+TRegexp("^wjets201[6-8]$")+TRegexp("^tt201[6-8]$")));
    entries.back()=entries.back()+samesign;
  }else if(mode=="data.amc+bg"){
    entries.push_back(Sample("data",Sample::Type::DATA,kBlack)+TRegexp("^muon201[6-8]$")+TRegexp("^electron201[6-8]$"));
    entries.push_back(Sample("simulation",Sample::Type::STACK,kRed)+TRegexp("^amc201[6-8]$")+TRegexp("^amctt201[6-8]$")+TRegexp("^vv201[6-8]$")+TRegexp("^wjets201[6-8]$")+TRegexp("^tt201[6-8]$"));
  }else if(mode=="data.amc+bg+ss"){
    entries.push_back(Sample("data",Sample::Type::DATA,kBlack)+TRegexp("^muon201[6-8]$")+TRegexp("^electron201[6-8]$"));
    entries.push_back(Sample("simulation",Sample::Type::STACK,kRed)+TRegexp("^amc201[6-8]$")+TRegexp("^amctt201[6-8]$")+TRegexp("^vv201[6-8]$")+TRegexp("^wjets201[6-8]$")+TRegexp("^tt201[6-8]$"));
    Sample samesign="ss_"%(Sample("QCD dijet",Sample::Type::BG,kCyan)+TRegexp("^muon201[6-8]$")+TRegexp("^electron201[6-8]$")+(-1)*(Sample()+TRegexp("^amc201[6-8]$")+TRegexp("^amctt201[6-8]$")+TRegexp("^vv201[6-8]$")+TRegexp("^wjets201[6-8]$")+TRegexp("^tt201[6-8]$")));
    entries.back()=entries.back()+samesign;
  }else if(mode=="amc.genamc"){
    entries.push_back(Sample("amc",Sample::Type::SIGNAL,kRed)+TRegexp("^amc201[6-8]$"));
    entries.push_back(Sample("genamc",Sample::Type::SIGNAL,kBlue)+TRegexp("^genamc201[6-8]$"));
  }else if(mode.Contains(TRegexp("muon$"))||mode.Contains(TRegexp("electron$"))){
    TString schannel;
    if(mode.Contains(TRegexp("muon$"))) schannel="muon";
    else if(mode.Contains(TRegexp("electron$"))) schannel="electron";
    vector<TString> syears={"2016","2017","2018"};
    for(int i=0;i<syears.size();i++){
      if(mode.Contains(TRegexp("^amc_"))) entries.push_back(Sample("amc"+syears[i],Sample::Type::SIGNAL,i+2)+TRegexp("^amc"+syears[i]+"$"));
      else if(mode.Contains(TRegexp("^genamc_"))) entries.push_back(Sample("genamc"+syears[i],Sample::Type::SIGNAL,i+2)+TRegexp("^genamc"+syears[i]+"$"));
      else if(mode.Contains(TRegexp("^amcJet_"))) entries.push_back(Sample("amcJet"+syears[i],Sample::Type::SIGNAL,i+2)+TRegexp("^amcJet"+syears[i]+"$"));
      else entries.push_back(Sample(schannel+syears[i],Sample::Type::DATA,i+2)+TRegexp("^"+schannel+syears[i]+"$"));
      entries.back().style.markerstyle=20+i;
      entries.back().prefix=schannel+syears[i]+"/";
    }
  }else if(mode=="wr"){
    entries.push_back(Sample("W_{R}#rightarrowlN#rightarrowlljj",Sample::Type::B,kCyan+1)+TRegexp("WRtoNLtoLLJJ_WR1600_N200"));
    entries.back().style.fillcolor=0;
    entries.back().style.linewidth=2;
    entries.back().style.drawoption="hist e";
    entries.push_back(Sample("standard model",Sample::Type::STACK,kRed)+TRegexp("^amcJet201[6-8]$")+TRegexp("^amcJettt201[6-8]$")+TRegexp("^vv201[6-8]$")+TRegexp("^wjets201[6-8]$")+TRegexp("^tt201[6-8]$"));
    //entries.push_back(Sample("data",Sample::Type::DATA,kBlack)+TRegexp("^muon201[6-8]$")+TRegexp("^electron201[6-8]$"));
  }else if(mode=="amc_pp.amc_mm"){
    entries.push_back("pp_"%(Sample("e^{+}e^{+}",Sample::Type::SIGNAL,kRed)+TRegexp("^amcJet201[6-8]$")));
    entries.back().style.linewidth=2;
    entries.back().style.fillcolor=0;
    entries.back().style.drawoption="hist e";
    entries.push_back("mm_"%(Sample("e^{-}e^{-}",Sample::Type::SIGNAL,kBlue)+TRegexp("^amcJet201[6-8]$")));
    entries.back().style.linewidth=2;
    entries.back().style.fillcolor=0;
    entries.back().style.drawoption="hist e";
  }    
}
  
  
//  else if(mode==1){ //data-bg vs amc
//    samples["data"]=MakeSample("data",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.),make_tuple("amctt"+syear,-1.),make_tuple("vv"+syear,-1.),make_tuple("wjets"+syear,-1.),make_tuple("tt"+syear,-1.));
//    samples["amc"]=MakeSample((channel==Channel::MUON)?"#gamma*/Z#rightarrow#mu#mu":"#gamma*/Z#rightarrowee",Sample::Type::SUM,kRed,make_tuple("amc"+syear,1.));
//  }else if(mode==2){ //data-bg vs amc vs amcgen
//    samples["data"]=MakeSample("data",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.),make_tuple("amctt"+syear,-1.),make_tuple("vv"+syear,-1.),make_tuple("wjets"+syear,-1.),make_tuple("tt"+syear,-1.));
//    samples["amc"]=MakeSample("aMC@NLO",Sample::Type::SUM,kRed,make_tuple("amc"+syear,1.));
//    samples["genamc"]=MakeSample("aMC@NLO(Gen)",Sample::Type::SUM,kMagenta,make_tuple("genamc"+syear,1.));
//  }else if(mode==3){ //data-bg vs amc vs amcJet vs madgraph
//    samples["data"]=MakeSample("data",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.),make_tuple("amctt"+syear,-1.),make_tuple("vv"+syear,-1.),make_tuple("wjets"+syear,-1.),make_tuple("tt"+syear,-1.));
//    //samples["amc"]=MakeSample("aMC@NLO",Sample::Type::SUM,kRed,make_tuple("amc"+syear,1.));
//    samples["amcJet"]=MakeSample("aMC@NLO Jet-binned",Sample::Type::SUM,kGreen,make_tuple("amcJet"+syear,1.));
//    //samples["mg"]=MakeSample("Madgraph",Sample::Type::SUM,kBlue,make_tuple("mg"+syear,1.));
//    //samples["powheg"]=MakeSample("powheg",Sample::Type::SUM,kBlue,make_tuple("powheg"+schannel+syear,1.));
//  }else if(mode==4){ //data-bg vs amcJet vs truthamcJet vs genamcJet
//    samples["data"]=MakeSample("data",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.),make_tuple("amctt"+syear,-1.),make_tuple("vv"+syear,-1.),make_tuple("wjets"+syear,-1.),make_tuple("tt"+syear,-1.));
//    samples["amcJet"]=MakeSample("aMC@NLO Jet-binned",Sample::Type::SUM,kGreen,make_tuple("amcJet"+syear,1.));
//    samples["truthamcJet"]=MakeSample("aMC@NLO Jet-binned(truth level)",Sample::Type::SUM,kRed,make_tuple("truthamcJet"+syear,1.));
//    //samples["genamcJet"]=MakeSample("aMC@NLO Jet-binned(GEN)",Sample::Type::SUM,kBlue,make_tuple("genamcJet"+syear,1.));
//  }else if(mode==190702){ //data-bg vs amc vs amcJet vs genamc vs genamcJet
//    samples["data"]=MakeSample("data",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.),make_tuple("amctt"+syear,-1.),make_tuple("vv"+syear,-1.),make_tuple("wjets"+syear,-1.),make_tuple("tt"+syear,-1.));
//    samples["amc"]=MakeSample("aMC@NLO",Sample::Type::SUM,kRed,make_tuple("amc"+syear,1.));
//    samples["amc"].markerstyle=21;
//    samples["amc"].markersize=0.6;
//    samples["genamc"]=MakeSample("aMC@NLO(GEN)",Sample::Type::SUM,kMagenta,make_tuple("genamc"+syear,1.));
//    samples["genamc"].markerstyle=25;
//    samples["genamc"].markersize=0.6;
//    samples["amcJet"]=MakeSample("aMC@NLO Jet-binned",Sample::Type::SUM,kBlue,make_tuple("amcJet"+syear,1.));
//    samples["amcJet"].markerstyle=21;
//    samples["amcJet"].markersize=0.6;
//    samples["genamcJet"]=MakeSample("aMC@NLO Jet-binned(GEN)",Sample::Type::SUM,kGreen,make_tuple("genamcJet"+syear,1.));
//    samples["genamcJet"].markerstyle=25;
//    samples["genamcJet"].markersize=0.6;
//    for(auto& [samplename,sample]:samples) sample.fillcolor=0;
//  }else if(mode==1907020){ //genamc vs genamcJet
//    samples["data"]=MakeSample("data",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.),make_tuple("amctt"+syear,-1.),make_tuple("vv"+syear,-1.),make_tuple("wjets"+syear,-1.),make_tuple("tt"+syear,-1.));
//    samples["data"].fillcolor=0;
//    samples["amc"]=MakeSample("aMC@NLO",Sample::Type::SUM,kRed,make_tuple("amc"+syear+"_nozptcor",1.));
//    samples["amcJet"]=MakeSample("aMC@NLO Jet-binned",Sample::Type::SUM,kBlue,make_tuple("amcJet"+syear+"_nozptcor",1.));
//    for(auto& [samplename,sample]:samples) sample.fillcolor=0;
//  }else if(mode==5){
//    samples["data"]=MakeSample("data",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.),make_tuple("amctt"+syear,-1.),make_tuple("vv"+syear,-1.),make_tuple("wjets"+syear,-1.),make_tuple("tt"+syear,-1.));
//    samples["amc"]=MakeSample("aMC@NLO",Sample::Type::SUM,kRed,make_tuple("amc"+syear,1.));
//    samples["genamc"]=MakeSample("aMC@NLO(Gen)",Sample::Type::SUM,kMagenta,make_tuple("genamc"+syear,1.));
//    samples["powheg"]=MakeSample("powheg",Sample::Type::SUM,kBlue,make_tuple("powheg"+schannel+syear,1.));
//    //samples["powheg(GEN)"]=MakeSample("powheg(GEN)",Sample::Type::SUM,kGreen,make_tuple("genpowheg"+schannel+syear,1.));
//  }else if(mode==10){ // with and without EfficiencySF
//    samples["data"]=MakeSample("data",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.));
//
//    if(syear=="2018") samples["sim"]=MakeSample("sim",Sample::Type::SUM,kRed,make_tuple("mg"+syear,1.),make_tuple("mgtt"+syear,1.),make_tuple("vv"+syear,1.),make_tuple("wjets"+syear,1.),make_tuple("tt"+syear,1.));
//    else samples["sim"]=MakeSample("sim",Sample::Type::SUM,kRed,make_tuple("amc"+syear,1.),make_tuple("amctt"+syear,1.),make_tuple("vv"+syear,1.),make_tuple("wjets"+syear,1.),make_tuple("tt"+syear,1.));
//
//    if(syear=="2018") samples["sim2"]=MakeSample("sim (w/o EffiSF)",Sample::Type::SYS,kBlue,make_tuple("mg"+syear,1.),make_tuple("mgtt"+syear,1.),make_tuple("vv"+syear,1.),make_tuple("wjets"+syear,1.),make_tuple("tt"+syear,1.));
//    else samples["sim2"]=MakeSample("sim (w/o EffiSF)",Sample::Type::SYS,kBlue,make_tuple("amc"+syear,1.),make_tuple("amctt"+syear,1.),make_tuple("vv"+syear,1.),make_tuple("wjets"+syear,1.),make_tuple("tt"+syear,1.));
//  }else if(mode==11){ // majority vs selective
//    samples["data"]=MakeSample("data (majority)",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.));
//    samples["data2"]=MakeSample("data (selective)",Sample::Type::SYS,kRed,make_tuple(schannel+syear,1.));
//  }else if(mode==12){ // PFISO vs trkISO
//    samples["data"]=MakeSample("data (PFISO)",Sample::Type::SUM,kBlack,make_tuple(schannel+syear,1.));
//    samples["data2"]=MakeSample("data (TrkISO)",Sample::Type::SYS,kRed,make_tuple(schannel+syear,1.));
//  }
//}
void AFBPlotter::SetupSystematics(){
  if(DEBUG)  cout<<"[SetupSystematics]"<<endl;
  systematics["RECOSF"]=MakeSystematic("RECOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_RECOSF_up _RECOSF_down");
  systematics["IDSF"]=MakeSystematic("IDSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_IDSF_up _IDSF_down");
  systematics["ISOSF"]=MakeSystematic("ISOSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_ISOSF_up _ISOSF_down");
  systematics["triggerSF"]=MakeSystematic("triggerSF",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_triggerSF_up _triggerSF_down");
  systematics["PUreweight"]=MakeSystematic("PUreweight",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_PUreweight_up _PUreweight_down");
  systematics["prefireweight"]=MakeSystematic("prefireweight",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_prefireweight_up _prefireweight_down");
  systematics["scale"]=MakeSystematic("scale",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_scale_up _scale_down");
  systematics["smear"]=MakeSystematic("smear",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL)+(1<<Sample::Type::BG),"_smear_up _smear_down");
  systematics["alphaS"]=MakeSystematic("alphaS",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL),"_alphaS_up _alphaS_down");
  systematics["scalevariation"]=MakeSystematic("scalevariation",Systematic::Type::ENVELOPE,(1<<Sample::Type::SIGNAL),"_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8");

  vector<TString> prefixes;
  for(int i=0;i<100;i++) prefixes.push_back(Form("_pdf%d",i));
  //FIXME
  //if(year==2017) systematics["pdf"]=MakeSystematic("pdf",Systematic::Type::HESSIAN,(1<<Sample::Type::SIGNAL),prefixes);
  //else if(year==2016) systematics["pdf"]=MakeSystematic("pdf",Systematic::Type::GAUSSIAN,(1<<Sample::Type::SIGNAL),prefixes);
  //else cout<<"###WARNING### [SetupSystematics] wrong year"<<endl;

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

double AFBPlotter::GetChi2(TH1* h1,TH1* h2){
  double chi2=0;
  for(int i=h1->GetXaxis()->GetFirst();i<h1->GetXaxis()->GetLast()+1;i++){
    double x1=h1->GetBinContent(i);
    double ex1=h1->GetBinError(i);
    double x2=h2?h2->GetBinContent(i):0.;
    double ex2=h2?h2->GetBinError(i):0.;
    chi2+=pow((x1-x2)/(ex1-ex2),2);
  }
  chi2/=h1->GetXaxis()->GetLast()-h1->GetXaxis()->GetFirst()+1;
  return chi2;
}
    
  
#endif
