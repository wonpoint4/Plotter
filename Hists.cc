#ifndef __HISTS_CC__
#define __HISTS_CC__
#include"TMatrixD.h"
class Hists:public vector<TH1*>{
public:
  TMatrixD cov;
  map<TString,TMatrixD> subcovs;
};
#endif
  
