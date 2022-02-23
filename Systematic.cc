#ifndef __SYSTEMATIC_CC__
#define __SYSTEMATIC_CC__
#include "Utils.h"
class Systematic{
public:
  enum Type{UNDEF,ENVELOPE,GAUSSIAN,HESSIAN,MULTI};
  TString title;
  Type type=Type::UNDEF;
  vector<pair<TString,TString>> replaces;
  vector<TString> keys;
  TString tag;

  Systematic(TString title_="",Type type_=Type::UNDEF,vector<TString> includes={},TString tag_="");
  ~Systematic();
  TString GetTypeString() const;
  void Print() const;
};
Systematic::Systematic(TString title_,Type type_,vector<TString> includes,TString tag_){
  title=title_;
  type=type_;
  tag=tag_;
  if(type==Systematic::Type::MULTI) keys=includes;
  else{
    for(const TString& replace:includes){
      if(Split(replace,"->").size()!=2) replaces.push_back(make_pair("$",replace));
      else replaces.push_back(make_pair(Split(replace,"->").at(0),Split(replace,"->").at(1)));
    }
  }
}
Systematic::~Systematic(){}

TString Systematic::GetTypeString() const{
  switch(type){
  case UNDEF: return "UNDEF";
  case ENVELOPE: return "ENVELOPE";
  case GAUSSIAN: return "GAUSSIAN";
  case HESSIAN: return "HESSIAN";
  case MULTI: return "MULTI";
  default: return "###ERROR### Bad Systematic::Type";
  }
}
void Systematic::Print() const{
  cout<<"------------------------"<<endl;
  cout<<title<<" "<<GetTypeString()<<" tag:"<<tag<<endl;
  if(type==MULTI) for(const auto& key:keys) cout<<key<<" ";
  else for(const auto& replace:replaces) cout<<replace.first<<"->"<<replace.second<<" ";
  cout<<"\n"<<endl;
}
#endif
