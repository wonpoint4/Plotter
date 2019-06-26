#ifndef __PLOT_CC__
#define __PLOT_CC__
#include"Utils.h"
class Plot{
public:
  enum Type{UNDEFINE,Compare,Ratio,Diff,CompareAndRatio,CompareAndDiff,DoubleRatio,Collection};
  TString name;
  TString histname;
  TString sysname;
  Type type;
  int rebin=0;
  double xmin=0,xmax=0;
  TString option;
  vector<Plot> subplots;
  Plot(vector<TString> words);
  Plot(TString line=""):Plot(Split(line," ")){};
  ~Plot();
  void Print(std::ostream& out=cout) const;
};
void Plot::Print(std::ostream& out) const{
  if(DEBUG>3) cout<<"###DEBUG### [void Plot::Print(std::ostream& out) const]"<<endl;
  out<<"<Plot> ";
  out<<"name: "<<name<<" histname: "<<histname;
  if(sysname!="") out<<" sysname: "<<sysname;
  if(type!=Type::UNDEFINE) out<<" type: "<<type;
  out<<" rebin: "<<rebin<<" xmin: "<<xmin<<" xmax: "<<xmax<<" "<<option<<"</Plot>"<<endl;
}
Plot::~Plot(){
  if(DEBUG>3) cout<<"###DEBUG### [Plot::~Plot()]"<<endl;
}
Plot::Plot(vector<TString> words){
  if(DEBUG>3) cout<<"###DEBUG### [Plot::Plot(vector<TString> words)]"<<endl;
  int imax=words.size();
  if(imax==0) return;
  if(words[0]!="<Plot>") cout<<"###ERROR### [Plot::Plot] Wrong format"<<endl;
  int depth=1;
  for(int i=1;i<imax;i++){
    if(words[i]=="<Plot>") subplots.push_back(Plot(vector<TString>(words.begin()+i,words.end())));
    else if(words[i]=="</Plot>") depth--;
    else if(words[i]=="name:") name=words[++i];
    else if(words[i]=="histname:") histname=words[++i];
    else if(words[i]=="sysname:") sysname=words[++i];
    else if(words[i]=="type:") type=(Type)words[++i].Atoi();
    else if(words[i]=="rebin:") rebin=words[++i].Atoi();
    else if(words[i]=="xmin:") xmin=words[++i].Atof();
    else if(words[i]=="xmax:") xmax=words[++i].Atof();
    else option+=" "+words[i];
    this->Print();
    if(depth==0) break;
  }
}  
Plot::Plot(TString line="") : Plot(Split(line," ")){
  if(DEBUG>3) cout<<"###DEBUG### [Plot::Plot(TString line="")]"<<endl;
}

#endif
