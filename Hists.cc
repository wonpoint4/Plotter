#ifndef __HISTS_CC__
#define __HISTS_CC__
#include"TMatrixD.h"
class Hists:public vector<TH1*>{
public:
  map<TString,TH1*> variations;
  const Sample* entity=NULL;
  TMatrixD cov;
  map<TString,TMatrixD> subcovs;
  TH1* operator[](int idx) const {
    return vector<TH1*>::operator[](idx);
  }
  const TH1* operator[](TString suffix) const {
    auto it=variations.find(suffix);
    if(it==variations.end()){
      return this->at(0);
    }else if(!it->second){
      return this->at(0);      
    }
    return it->second;
  }
  Hists Clone() const {
    Hists hists;
    hists.entity=entity;
    for(const auto& h: *this){
      hists.push_back(GetTH1(h));
    }
    for(const auto& [suffix,h]: this->variations){
      if(h) hists.variations[suffix]=(TH1*)h->Clone();
      else hists.variations[suffix]=NULL;
    }
    return hists;
  }
  void Divide(const Hists& hists){
    TH1* base=GetTH1(hists.at(0));
    if(base){
      at(0)->Divide(base);
      delete base;
    }
    set<TString> suffixes;
    for(const auto& [suffix,h]:variations) suffixes.insert(suffix);
    for(const auto& [suffix,h]:hists.variations) suffixes.insert(suffix);
    for(const TString& suffix:suffixes){
      TH1* h=variations[suffix];
      if(!h){
	h=(TH1*)at(0)->Clone();
      }
      h->Divide(hists[suffix]);
      variations[suffix]=h;
    }
  }
  void Divide(const TH1* hist){
    at(0)->Divide(hist);
    set<TString> suffixes;
    for(const auto& [suffix,h]:variations) suffixes.insert(suffix);
    for(const TString& suffix:suffixes){
      TH1* h=variations[suffix];
      if(!h){
	h=(TH1*)at(0)->Clone();
      }
      h->Divide(hist);
      variations[suffix]=h;
    }
  }
  static TH1* GetTH1(const TH1* h){
    TH1* hh=NULL;
    if(h){
      if(h->InheritsFrom("THStack")){
	TIter iter(((THStack*)h)->GetHists(),false);
	while(TH1* this_hist=(TH1*)iter()){
	  if(hh) hh->Add((TH1*)this_hist);
	  else hh=(TH1*)this_hist->Clone();
	}
	hh->SetName(h->GetName());
	hh->SetTitle(h->GetTitle());
	hh->SetStats(0);
      }else{
	hh=(TH1*)h->Clone();
      }
    }
    return hh;
  }
};


#endif
  
