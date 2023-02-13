#ifndef __SYSTEMATIC_CC__
#define __SYSTEMATIC_CC__
#include "Utils.h"
class Systematic{
public:
  enum Type{UNDEF,ENVELOPE,GAUSSIAN,HESSIAN,MULTI};
  TString title;
  Type type=Type::UNDEF;
  vector<TString> variations;
  vector<TString> keys;
  TString tag;

  Systematic(TString title_="",Type type_=Type::UNDEF,vector<TString> variations_={},TString tag_="");
  ~Systematic();
  TString GetTypeString() const;
  void Print() const;
};
Systematic::Systematic(TString title_,Type type_,vector<TString> variations_,TString tag_){
  title=title_;
  type=type_;
  tag=tag_;
  if(type==Systematic::Type::MULTI) keys=variations_;
  else{
    for(const TString& variation:variations_){
      if(variation.Contains(":")) variations.push_back(variation+":"+tag);
      else if(Split(variation,"->").size()==2) variations.push_back(TString("replace:")+variation+":"+tag);
      else variations.push_back(TString("replace:$->")+variation+":"+tag);
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
  else for(const auto& variation:variations) cout<<variation<<" ";
  cout<<"\n"<<endl;
}
#endif
