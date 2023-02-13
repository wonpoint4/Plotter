#ifndef __PLOT_CC__
#define __PLOT_CC__
#include <sstream>
#include "Global.h"
#include "Utils.h"
class Plot{
public:
  enum Type{UNDEF,Compare,Ratio,Diff,Sig,CompareAndRatio,CompareAndDiff,CompareAndSig,DoubleRatio,DoubleDiffAFB,Multi};
  TString name,title;
  TString histname;
  TString sysname;
  TString classname;
  TString replace_old;
  TString replace_new;
  TString replace_tag;
  TString xtitle,ytitle;
  TString project;
  TString era;
  TString lumi;
  TString save;
  vector<tuple<double,double,TString>> texts;
  Type type=Type::CompareAndRatio;
  TString rebin="";
  double xmin=0,xmax=0,ymin=0,ymax=0;
  double Xmin=0,Xmax=0,Ymin=0,Ymax=0,Zmin=0,Zmax=0,Umin=0,Umax=0;
  double blind_xmin=0,blind_xmax=0;
  vector<pair<double,TString>> scales;
  TString option;
  vector<Plot> subplots;
  bool root=false;
  TPad* pad=NULL;
  vector<vector<TH1*>> hists;

  Plot(vector<TString> words);
  Plot(TString line=""):Plot(SplitOptions(line)){};
  ~Plot();
  Plot operator-(const char* opt) const;
  Plot operator+(const char* opt) const;
  Plot operator/(Plot p) const;
  Plot GetSubPlot(int ipad);
  bool IsMultiPad() const;
  void Print(std::ostream& out=cout) const;
  void RemoveOption(TString option_);
  void SetOption(TString option_);
  vector<TString> SplitOptions(TString option_,TString del=" ");
};
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
Plot::~Plot(){
}
Plot Plot::operator-(const char* opt) const{
  Plot temp(*this);
  temp.RemoveOption(opt);
  return temp;
}
Plot Plot::operator+(const char* opt) const{
  Plot temp(*this);
  temp.SetOption(opt);
  return temp;
}
Plot Plot::operator/(Plot p) const{
  Plot temp;
  temp.SetOption(Form("name:%s histname:%s type:%d",((*this).name+"%"+p.name).Data(),((*this).name+"%"+p.name).Data(),Type::DoubleRatio));
  temp.subplots.push_back(*this);
  temp.subplots.push_back(p);
  return temp;
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
void Plot::Print(std::ostream& out) const{
  out<<"<Plot> ";
  out<<"name:"<<name<<" histname:"<<histname;
  if(type!=Type::UNDEF) out<<" type:"<<type;
  if(rebin!="") out<<" rebin:"<<rebin;
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
  if(replace_old!=""&&replace_new!=""){
    if(replace_tag!="") out<<" replace:"<<replace_old<<"->"<<replace_new<<":"<<replace_tag;
    else out<<" replace:"<<replace_old<<"->"<<replace_new;
  }
  if(title!=name) out<<" title:"<<title;
  if(xtitle!="") out<<" xtitle:"<<xtitle;
  if(ytitle!="") out<<" ytitle:"<<ytitle;
  if(blind_xmin&&blind_xmax) out<<" blind:"<<blind_xmin<<","<<blind_xmax;
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
void Plot::RemoveOption(TString option_){
  vector<TString> options=SplitOptions(option);
  vector<TString> options_remove=SplitOptions(option_);
  for(const auto& remove:options_remove){
    if(remove=="type") type=Type::UNDEF;
    else if(remove=="rebin") rebin="";
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
    else if(remove=="era") era="";
    else if(remove=="lumi") lumi="";
    else if(remove=="sysname") sysname="";
    else if(remove=="save") save="";
    else if(remove=="scale") scales={};
    else if(remove=="text") texts={};
    else if(remove=="replace"){
      replace_old="";
      replace_new="";
      replace_tag="";
    }
    else if(remove=="title") title="";
    else if(remove=="xtitle") xtitle="";
    else if(remove=="ytitle") ytitle="";
    else if(remove=="blind"){
      blind_xmin=0;
      blind_xmax=0;
    }
    else{
      for(int i=0,n=options.size();i<n;i++){
        if(options[i].Contains(TRegexp("^"+remove))){
          options.erase(options.begin()+i);
          i--;n--;
        }
      }
    }
  }
  TString newoption;
  for(const auto& opt:options) newoption+=opt+" ";
  option=newoption;
}
void Plot::SetOption(TString option_){
  for(const auto& opt:SplitOptions(option_)){
    if(opt.Contains(TRegexp("^name:"))) name=Strip(opt(5,999),"'");
    else if(opt.Contains(TRegexp("^title:"))) title=Strip(opt(6,999),"'");
    else if(opt.Contains(TRegexp("^histname:"))) histname=Strip(opt(9,999),"'"); 
    else if(opt.Contains(TRegexp("^type:"))) type=(Type)TString(opt(5,999)).Atoi();
    else if(opt.Contains(TRegexp("^rebin:"))) rebin=TString(opt(6,999));
    else if(opt.Contains(TRegexp("^xmin:"))) xmin=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^xmax:"))) xmax=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^ymin:"))) ymin=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^ymax:"))) ymax=TString(opt(5,999)).Atof();
    else if(opt.Contains(TRegexp("^sysname:"))) sysname=Strip(opt(8,999),"'");
    else if(opt.Contains(TRegexp("^save:"))) save=Strip(opt(5,999),"'");
    else if(opt.Contains(TRegexp("^text:"))){
      vector<TString> words=SplitOptions(opt(5,999),",");
      if(words.size()==3){
	texts.push_back(make_tuple(words[0].Atof(),words[1].Atof(),Strip(words[2],"'")));
      }else{
	PError("Wrong syntax for text option "+opt);
      }
    }
    else if(opt.Contains(TRegexp("^suffix:"))){
      replace_old="$";
      replace_new=Strip(opt(7,999),"'");
      if(replace_new.Contains(":")){
	replace_tag=replace_new(replace_new.Index(":")+1,replace_new.Length());
	replace_new=replace_new(0,replace_new.Index(":"));
      }
    }
    else if(opt.Contains(TRegexp("^scale:"))){
      TString scale=TString(opt(6,999));
      TString tag="";
      if(scale.Contains(":")){
	tag=scale(scale.Index(":")+1,scale.Length());
	scale=scale(0,scale.Index(":"));
      }else{
	PError("Wrong syntax for scale option "+opt);
      }	
      scales.push_back(make_pair(scale.Atof(),tag));
    }
    else if(opt.Contains(TRegexp("^replace:"))){
      vector<TString> replacev=Split(opt(8,999),"->");
      if(replacev.size()!=2) PError("Wrong syntax for replace option "+opt);
      else{
	replace_old=replacev.at(0);
	replace_new=replacev.at(1);
	if(replace_new.Contains(":")){
	  replace_tag=replace_new(replace_new.Index(":")+1,replace_new.Length());
	  replace_new=replace_new(0,replace_new.Index(":"));
	}
      }
    }
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
    else if(opt.Contains(TRegexp("^era:"))){
      era=Strip(opt(4,999),"'");
      if(era=="2016a") era="2016preVFP";
      if(era=="2016b") era="2016postVFP";
    }
    else if(opt.Contains(TRegexp("^lumi:"))) lumi=TString(opt(5,999));
    else if(opt.Contains(TRegexp("^blind"))){
      if(opt.Contains(TRegexp("^blind:"))){
	vector<TString> blindv=Split(opt(6,999),",");
	if(blindv.size()!=2) PError("Wrong syntax for blind option "+opt);
	else{
	  blind_xmin=blindv.at(0).Atof();
	  blind_xmax=blindv.at(1).Atof();
	}
      }else{
	blind_xmin=-1.;
	blind_xmax=-1.;
      }
    }
    else option+=" "+opt;
  }
}   
vector<TString> Plot::SplitOptions(TString option_,TString del){
  vector<TString> options_raw=Split(option_,del);
  vector<TString> options_out;
  for(const auto& opt:options_raw){
    if(options_out.size()&&options_out.back().CountChar('\'')%2==1) options_out.back()+=del+opt;
    else options_out.push_back(opt);
  }
  return options_out;
}  
#endif
