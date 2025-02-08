#ifndef __UTILS_H__
#define __UTILS_H__
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TRegexp.h>
vector<TString> Split(TString s,TString del){
  TObjArray* array=s.Tokenize(del);
  vector<TString> out;
  for(const auto& obj:*array){
    out.push_back(((TObjString*)obj)->String());
  }
  array->Delete();
  return out;
}

TString Dirname(TString s){
  if(s.Last('/')!=-1) return s(0,s.Last('/'));
  else return ".";
}
TString Basename(TString s){
  if(s.Last('/')!=-1) return s(s.Last('/')+1,s.Length());
  else return s;
}
TString Replace(TString str,TString reg,TString repl){
  if(reg=="") return str;
  int extent;
  int start=str.Index(TRegexp(reg),&extent);
  if(start>=0) return str.Replace(start,extent,repl);
  else return str;
}
TString Strip(TString str,TString c){
  if(str.EndsWith(c)) str=str(0,str.Length()-c.Length());
  if(str.BeginsWith(c)) str=str(c.Length(),str.Length()-c.Length());
  return str;
}
TString Join(TString del,vector<TString> strs){
  TString out;
  for(const auto& s:strs) out+=s+del;
  return out(0,out.Length()-del.Length());
}
vector<double> VectorDouble(TString str){
  str=Strip(str,"{");
  str=Strip(str,"}");
  vector<TString> sdoubles=Split(str,",");
  vector<double> out;
  for(TString sdouble:sdoubles) out.push_back(sdouble.Atof());
  return out;
}
TString Str(int n,const double* array){
  TString rt="{";
  for(int i=0;i<n;i++){
    if(i==n-1)
      rt+=Form("%f",array[i]);
    else
      rt+=Form("%f,",array[i]);
  }
  rt+="}";
  return rt;
}
vector<int> Range(int s,int n,int h=1){
  vector<int> out;
  for(int i=s;i<n;i+=h) out.push_back(i);
  return out;
}
vector<int> Range(int n){
  return Range(0,n,1);
}
vector<TString> FormRange(TString form,vector<int> range){
  vector<TString> out;
  for(auto d:range) out.push_back(Form(form,d));
  return out;
}

#endif
