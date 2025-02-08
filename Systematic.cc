#ifndef __SYSTEMATIC_CC__
#define __SYSTEMATIC_CC__
#include "Utils.h"
class Systematic{
public:
  enum Type{UNDEF,ENVELOPE,GAUSSIAN,HESSIAN,CORRELATED,MULTI};
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
    for(TString variation:variations_){
      if(!variation.BeginsWith("replace:")&&!variation.BeginsWith("scale:")){
	if(variation.Contains("->")){
	  variation="replace:"+variation;
	}else{
	  variation="replace:$->"+variation;
	}
      }
      vector<TString> words=Split(variation,":");
      if(words.size()==3){
	variations.push_back(variation);
      }else if(words.size()==2){
	if(tag!="")
	  variations.push_back(variation+":"+tag);
	else
	  variations.push_back(variation);
      }else{
	cout<<"### ERROR ### [Systematic::Systematic] wrong variation "<<variation<<endl;
      }
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
  case CORRELATED: return "CORRELATED";
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
