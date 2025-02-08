#include"SKFlatPlotter.cc"
#ifndef __Chi2Plotter_CC__
#define __Chi2Plotter_CC__
class Chi2Plotter:public SKFlatPlotter{
public:
  Chi2Plotter(TString mode_="data ^mi+tau_mi+vv+wjets+tt+st+qcdss+aa",TString Analyzer_="Chi2Analyzer");
  virtual void SetupSystematics();
};
Chi2Plotter::Chi2Plotter(TString mode_,TString Analyzer_){
  Setup(mode_,Analyzer_);
}
void Chi2Plotter::SetupSystematics(){
  SKFlatPlotter::SetupSystematics();

  vector<int> nmem={0,1,1,1,1,1,1,2,2,1,1,2,2,2,1,1,1,1,1};
  for(int i=1;i<19;i++){
    for(int j=0;j<nmem[i];j++){
      AddSystematic(Form("efficiency_electron_s%dm%d",i,j),Form("eff. e s%dm%d",i,j),Systematic::Type::CORRELATED,TString()+Form(" _electronRECOSF_s%dm%d",i,j)+Form(" _electronIDSF_s%dm%d",i,j)+Form(" _triggerSF_s%dm%d",i,j),"sim");
      AddSystematic(Form("efficiency_muon_s%dm%d",i,j),Form("eff. mu s%dm%d",i,j),Systematic::Type::CORRELATED,TString()+Form(" _muonTrackingSF_s%dm%d",i,j)+Form(" _muonRECOSF_s%dm%d",i,j)+Form(" _muonIDSF_s%dm%d",i,j)+Form(" _triggerSF_s%dm%d",i,j),"sim");
    }
    //AddSystematic(Form("efficiency_electron_s%d",i),Form("eff. e s%d",i),Systematic::Type::ENVELOPE,Join(" ",FormRange(TString()+Form("efficiency_electron_s%d",i)+"s%d",Range(nmem[i]))),"sim");
    //AddSystematic(Form("efficiency_muon_s%d",i),Form("eff. mu s%d",i),Systematic::Type::ENVELOPE,Join(" ",FormRange(TString()+Form("efficiency_muon_s%d",i)+"s%d",Range(nmem[i]))),"sim");
  }

  AddSystematic("efficiencySF_correlated","eff. unc.",Systematic::Type::MULTI,"efficiencySF_stat triggerSF_mode1 triggerSF_interpolation "+Join(" ",FormRange("efficiency_electron_s%dm0",Range(1,18)))+" "+Join(" ",FormRange("efficiency_muon_s%dm0",Range(1,17))));
  AddSystematic("efficiencySF_residual_correlated","eff. residual",Systematic::Type::MULTI,"efficiencySF_stat triggerSF_mode1 triggerSF_interpolation "+Join(" ",FormRange("efficiency_electron_s%dm0",Range(1,19)))+" "+Join(" ",FormRange("efficiency_muon_s%dm0",Range(1,18))));

  AddSystematic("sys_noresidual","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight z0weight btag CFSF efficiencySF qcdss JEC lumi xsec electronenergy muonmomentum");
  AddSystematic("totalsys_noresidual","syst. unc.",Systematic::Type::MULTI,"sys_noresidual dytheory tttheory");
  AddSystematic("totalsys_table_noresidual","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight z0weight btag CFSF efficiencySF qcdss JEC lumi xsec electronenergy muonmomentum dytheory tttheory");

  AddSystematic("sys_correlated","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight z0weight btag CFSF efficiencySF_residual_correlated qcdss JEC lumi xsec electronenergy muonmomentum");
  AddSystematic("totalsys_correlated","syst. unc.",Systematic::Type::MULTI,"sys_correlated dytheory tttheory");
  AddSystematic("totalsys_table_correlated","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight z0weight btag CFSF efficiencySF_residual_correlated qcdss JEC lumi xsec electronenergy muonmomentum dytheory tttheory");

  AddSystematic("sys_noresidual_correlated","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight z0weight btag CFSF efficiencySF_correlated qcdss JEC lumi xsec electronenergy muonmomentum");
  AddSystematic("totalsys_noresidual_correlated","syst. unc.",Systematic::Type::MULTI,"sys_noresidual_correlated dytheory tttheory");
  AddSystematic("totalsys_table_noresidual_correlated","syst. unc.",Systematic::Type::MULTI,"PUweight prefireweight z0weight btag CFSF efficiencySF_correlated qcdss JEC lumi xsec electronenergy muonmomentum dytheory tttheory");

}
#endif
