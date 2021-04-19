#ifndef __PLOT_CC__
#define __PLOT_CC__
#include "Global.h"
#include"Utils.h"
class Plot{
public:
  enum Type{UNDEF,Compare,Ratio,Diff,Sig,CompareAndRatio,CompareAndDiff,CompareAndSig,DoubleRatio,Collection};
  TString name,title;
  TString histname;
  TString sysname;
  TString classname;
  TString suffix;
  TString xtitle,ytitle;
  TString project;
  int varibit=0;
  Type type=Type::CompareAndRatio;
  int rebin=0;
  double xmin=0,xmax=0,ymin=0,ymax=0;
  double Xmin=0,Xmax=0,Ymin=0,Ymax=0,Zmin=0,Zmax=0,Umin=0,Umax=0;
  TString option;
  vector<Plot> subplots;
  Plot GetSubPlot(int ipad);
  Plot(vector<TString> words);
  Plot(TString line=""):Plot(Split(line," ")){};
  ~Plot();
  void Print(std::ostream& out=cout) const;
  void SetOption(TString option_);
  void RemoveOption(TString option_);
  vector<TString> SplitOptions(TString option_);
  bool IsMultiPad() const;
  Plot operator-(const char* opt);
  Plot operator+(const char* opt);
  Plot operator/(Plot p);
};
Plot Plot::operator-(const char* opt){
  Plot temp(*this);
  temp.RemoveOption(opt);
  return temp;
}
Plot Plot::operator+(const char* opt){
  Plot temp(*this);
  temp.SetOption(opt);
  return temp;
}
Plot Plot::operator/(Plot p){
  Plot temp;
  temp.SetOption(Form("name:%s histname:%s type:%d",((*this).name+"%"+p.name).Data(),((*this).name+"%"+p.name).Data(),Type::DoubleRatio));
  temp.subplots.push_back(*this);
  temp.subplots.push_back(p);
  return temp;
}
void Plot::SetOption(TString option_){
  for(const auto& opt:SplitOptions(option_)){
    if(opt.Contains(TRegexp("^name:"))) name=Strip(opt(5,999),"'");
    else if(opt.Contains(TRegexp("^title:"))) title=Strip(opt(6,999),"'");
    else if(opt.Contains(TRegexp("^histname:"))) histname=Strip(opt(9,999),"'"); 
    else if(opt.Contains(TRegexp("^type:"))) type=(Type)TString(opt(5,999)).Atoi();
    else if(opt.Contains(TRegexp("^rebin:"))) rebin=TString(opt(6,999)).Atoi();
    else if(opt.Contains(TRegexp("^xmin:"))) xmin=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^xmax:"))) xmax=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^ymin:"))) ymin=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^ymax:"))) ymax=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^sysname:"))) sysname=Strip(opt(8,999),"'");
    else if(opt.Contains(TRegexp("^suffix:"))) suffix=Strip(opt(7,999),"'");
    else if(opt.Contains(TRegexp("^varibit:"))) varibit=TString(opt(8,999)).Atoi();
    else if(opt.Contains(TRegexp("^xtitle:"))) xtitle=Strip(opt(7,999),"'");
    else if(opt.Contains(TRegexp("^ytitle:"))) ytitle=Strip(opt(7,999),"'");
    else if(opt.Contains(TRegexp("^Xmin:"))) Xmin=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^Xmax:"))) Xmax=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^Ymin:"))) Ymin=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^Ymax:"))) Ymax=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^Zmin:"))) Zmin=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^Zmax:"))) Zmax=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^Umin:"))) Umin=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^Umax:"))) Umax=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^classname:"))) classname=Strip(opt(10,999),"'");
    else if(opt.Contains(TRegexp("^project:"))) project=Strip(opt(8,999),"'");
    else option+=" "+opt;
  }
}   
void Plot::RemoveOption(TString option_){
  vector<TString> options=Split(option," ");
  vector<TString> options_remove=Split(option_," ");
  for(const auto& remove:options_remove){
    if(remove=="type") type=Type::UNDEF;
    else if(remove=="rebin") rebin=0;
    else if(remove=="xmin") xmin=0;
    else if(remove=="xmax") xmax=0;
    else if(remove=="ymin") ymin=0;
    else if(remove=="ymax") ymax=0;
    else if(remove=="Xmin") Xmin=0;
    else if(remove=="Xmax") Xmax=0;
    else if(remove=="Ymin") Ymin=0;
    else if(remove=="Ymax") Ymax=0;
    else if(remove=="Zmin") Zmin=0;
    else if(remove=="Zmax") Zmax=0;
    else if(remove=="Umin") Umin=0;
    else if(remove=="Umax") Umax=0;
    else if(remove=="sysname") sysname="";
    else if(remove=="suffix") suffix="";
    else if(remove=="varibit") varibit=0;
    else if(remove=="title") title="";
    else if(remove=="xtitle") xtitle="";
    else if(remove=="ytitle") ytitle="";
    else{
      for(int i=0;i<options.size();i++){
        if(options[i].Contains(TRegexp("^"+remove))){
          options.erase(options.begin()+i);
          i--;
        }
      }
    }
  }
  TString newoption;
  for(const auto& opt:options) newoption+=opt+" ";
  option=newoption;
}
vector<TString> Plot::SplitOptions(TString option_){
  vector<TString> options_raw=Split(option_," ");
  vector<TString> options_out;
  for(const auto& opt:options_raw){
    if(options_out.size()&&options_out.back().CountChar('\'')%2==1) options_out.back()+=" "+opt;
    else options_out.push_back(opt);
  }
  return options_out;
}
    
  
void Plot::Print(std::ostream& out) const{
  out<<"<Plot> ";
  out<<"name:"<<name<<" histname:"<<histname;
  if(type!=Type::UNDEF) out<<" type:"<<type;
  if(rebin!=0) out<<" rebin:"<<rebin;
  if(xmin!=0) out<<" xmin:"<<xmin;
  if(xmax!=0) out<<" xmax:"<<xmax;
  if(ymin!=0) out<<" ymin:"<<ymin;
  if(ymax!=0) out<<" ymax:"<<ymax;
  if(Xmin!=0) out<<" Xmin:"<<Xmin;
  if(Xmax!=0) out<<" Xmax:"<<Xmax;
  if(Ymin!=0) out<<" Ymin:"<<Ymin;
  if(Ymax!=0) out<<" Ymax:"<<Ymax;
  if(Zmin!=0) out<<" Zmin:"<<Zmin;
  if(Zmax!=0) out<<" Zmax:"<<Zmax;
  if(Umin!=0) out<<" Umin:"<<Umin;
  if(Umax!=0) out<<" Umax:"<<Umax;
  if(sysname!="") out<<" sysname:"<<sysname;
  if(suffix!="") out<<" suffix:"<<suffix;
  if(varibit!=0) out<<" varibit:"<<varibit;
  if(title!=name) out<<" title:"<<title;
  if(xtitle!="") out<<" xtitle:"<<xtitle;
  if(ytitle!="") out<<" ytitle:"<<ytitle;
  out<<" "<<option;
  for(const auto& sub:subplots){
    stringstream temp;
    sub.Print(temp);
    temp.seekp(-1, std::ios_base::end);
    temp<<' ';
    out<<temp.str();
  }
  out<<" </Plot>\n";
}
Plot::~Plot(){
}
Plot::Plot(vector<TString> words){
  int imax=words.size();
  if(imax==0) return;
  if(words[0]!="<Plot>") PError("[Plot::Plot] Wrong format");
  vector<TString> subplot_words;
  int depth=0;
  for(int i=0;i<imax;i++){
    if(words[i]=="<Plot>"){
      depth++;
      if(depth>1) subplot_words.push_back(words[i]);
    }else if(words[i]=="</Plot>"){
      depth--;
      if(depth>=1){
	subplot_words.push_back(words[i]);
	if(depth==1){
	  Plot temp(subplot_words);
	  subplots.push_back(temp);
	  subplot_words.clear();
	}
      }else if(depth==0) break;
    }else{
      if(depth==1) SetOption(words[i]);
      else if(depth>1) subplot_words.push_back(words[i]);
    }      
  }
  //this->Print();
}  
Plot::Plot(TString line="") : Plot(Split(line," ")){
}
Plot Plot::GetSubPlot(int i){
  Plot sub=*this;
  sub.option="";
  for(const auto& opt:SplitOptions(option)){
    if(opt.Contains(TRegexp("^[0-9]:"))){
      if(opt.BeginsWith(Form("%d:",i))) sub.SetOption(Replace(opt,"^[0-9]:",""));
    }else sub.SetOption(opt);
  }
  return sub;
}
bool Plot::IsMultiPad() const {
  if(type==Type::CompareAndRatio||type==Type::CompareAndDiff||type==Type::CompareAndSig) return true;
  else return false;
}
#endif
