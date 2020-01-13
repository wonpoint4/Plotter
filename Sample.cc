#ifndef __SAMPLE_CC__
#define __SAMPLE_CC__
#include"Global.h"
#include"Style.cc"
class Sample{
public:
  enum Type{UNDEF,DATA,SIGNAL,BG,GEN,STACK,SUM,A,B,C,D};
  TString GetTypeString() const{
    switch(type){
    case UNDEF: return "UNDEF";
    case DATA: return "DATA";
    case SIGNAL: return "SIGNAL";
    case BG: return "BG";
    case GEN: return "GEN";
    case STACK: return "STACK";
    case SUM: return "SUM";
    case A: return "A";
    case B: return "B";
    case C: return "C";
    case D: return "D";
    default: return "###ERROR### Bad Sample::Type";
    }
  }
  TString title;
  Type type;
  Style style;
  Style styletotal;
  TString prefix;
  vector<tuple<TString,double,TString,TString>> files; //filename,type,weight,prefix,suffix
  vector<Sample> subs;

  Sample();
  Sample(TString title_,Sample::Type type_,int color_);
  Sample operator+(const Sample& sam);
  Sample operator+(const char* key);
  Sample operator+(const TString key);
  Sample operator+(const TRegexp reg);
  friend Sample operator%(const char* prefix,const Sample& sam);
  friend Sample operator%(const TString prefix,const Sample& sam);
  friend Sample operator%(const Sample& sam,const char* suffix);
  friend Sample operator%(const Sample& sam,const TString suffix);

  Sample operator*(double w);
  friend Sample operator*(double w,const Sample& sam);
  void SetStyle(int color);
  void Add(TRegexp sampleregexp,double weight=1.,TString prefix="",TString suffix="");
  void ApplyStyle(TH1* hist) const;
  void Print(bool detail=false) const;
  bool IsCollection() const;
};
map<TString,Sample> samples;

Sample::Sample(){
  type=(Sample::Type)UNDEF;
}
Sample::Sample(TString title_,Sample::Type type_,int color_){
  title=title_;
  type=type_;
  SetStyle(color_);
}
bool Sample::IsCollection() const{
  if(type==Sample::Type::STACK||type==Sample::Type::SUM) return true;
  else return false;
}
Sample Sample::operator+(const Sample& sam){
  Sample temp(*this);
  if(temp.IsCollection()&&sam.IsCollection())
    temp.subs.insert(temp.subs.end(),sam.subs.begin(),sam.subs.end());
  else if(temp.IsCollection()){
    temp.subs.push_back(sam);
    if(temp.type==Sample::Type::STACK) temp.subs.back().style.fillcolor=temp.subs.back().style.linecolor;
  }else if(sam.IsCollection())
    for(const auto& subsample:sam.subs)
      temp.files.insert(temp.files.end(),subsample.files.begin(),subsample.files.end());
  else
    temp.files.insert(temp.files.end(),sam.files.begin(),sam.files.end());
  return temp;
}
Sample Sample::operator+(const char* key){
  for(const auto& [k,sample]:samples){
    if(k==key) return (*this)+sample;
  }
  if(DEBUG>0) cout<<"###ERROR### [Sample operator+(const char* key,const Sample& sam)] no sample with key "<<key<<endl;
  exit(1);
}  
Sample Sample::operator+(const TString key){
  return (*this)+key.Data();
}
Sample Sample::operator+(const TRegexp reg){
  Sample temp(*this);
  for(auto const& [key,sample]:samples)
    if(key.Contains(reg)) temp=temp+sample;
  return temp;
}
Sample operator%(const char* prefix,const Sample& sam){
  Sample temp(sam);
  if(temp.type==Sample::Type::STACK||temp.type==Sample::Type::SUM)
    for(auto& sub:temp.subs) sub=prefix%sub;
  else
    for(auto& [file,w,pre,suf]:temp.files) pre=prefix+pre;
  return temp;
}  
Sample operator%(const TString prefix,const Sample& sam){
  return prefix.Data()%sam;
}  
Sample operator%(const Sample& sam,const char* suffix){
  Sample temp(sam);
  if(temp.type==Sample::Type::STACK||temp.type==Sample::Type::SUM)
    for(auto& sub:temp.subs) sub=sub%suffix;
  else
    for(auto& [file,w,pre,suf]:temp.files) suf=suf+suffix;
  return temp;
}  
Sample operator%(const Sample& sam,const TString suffix){
  return sam%suffix.Data();
}  
Sample Sample::operator*(double f){
  Sample temp(*this);
  if(temp.type==Sample::Type::STACK||temp.type==Sample::Type::SUM)
    for(auto& sub:temp.subs) sub=sub*f;
  else
    for(auto& [file,w,pre,suf]:temp.files) w*=f;
  return temp;
}
Sample operator*(double f,const Sample& sam){
  Sample temp(sam);
  if(temp.type==Sample::Type::STACK||temp.type==Sample::Type::SUM)
    for(auto& sub:temp.subs) sub=sub*f;
  else
    for(auto& [file,w,pre,suf]:temp.files) w*=f;
  return temp;
}
void Sample::SetStyle(int color){
  style=Style(color);
  
  if(type==Type::STACK){
    style.drawoption="e";
  }
}
void Sample::Add(TRegexp regexp,double weight,TString prefix,TString suffix){
  for(auto it=samples.begin();it!=samples.end();it++){
    if(it->first.Contains(regexp)) (*this)=(*this)+weight*(prefix%(it->second)%suffix);
  }
}
void Sample::ApplyStyle(TH1* hist) const {
  if(DEBUG>3) cout<<"###DEBUG### [Sample::ApplyStyle(TH1* hist)]"<<endl;
  style.Apply(hist);
  if(hist){
    hist->SetNameTitle(title,title);
  }
}
void Sample::Print(bool detail) const{
  cout<<"Title: "<<title<<" "<<"Type: "<<GetTypeString()<<endl;
  cout<<"Style: ";
  style.Print();
  for(const auto& [file,weight,prefix,suffix]:files){
    cout<<file<<" "<<weight<<" "<<prefix<<" "<<suffix<<endl;
  }
  for(const auto& sub:subs){
    sub.Print(detail);
  }
}

Sample MakeSample(TString title,Sample::Type type,int color,tuple<TString,double> subsample1,tuple<TString,double> subsample2=make_tuple("",0.),tuple<TString,double> subsample3=make_tuple("",0.),tuple<TString,double> subsample4=make_tuple("",0.),tuple<TString,double> subsample5=make_tuple("",0.),tuple<TString,double> subsample6=make_tuple("",0.),tuple<TString,double> subsample7=make_tuple("",0.)){
  Sample sample(title,type,color);
  if(get<0>(subsample1)!="") sample.Add(get<0>(subsample1),get<1>(subsample1),"","");
  if(get<0>(subsample2)!="") sample.Add(get<0>(subsample2),get<1>(subsample2),"","");
  if(get<0>(subsample3)!="") sample.Add(get<0>(subsample3),get<1>(subsample3),"","");
  if(get<0>(subsample4)!="") sample.Add(get<0>(subsample4),get<1>(subsample4),"","");
  if(get<0>(subsample5)!="") sample.Add(get<0>(subsample5),get<1>(subsample5),"","");
  if(get<0>(subsample6)!="") sample.Add(get<0>(subsample6),get<1>(subsample6),"","");
  if(get<0>(subsample7)!="") sample.Add(get<0>(subsample7),get<1>(subsample7),"","");
  return sample;
}
Sample MakeSample(TString title,Sample::Type type,int color,tuple<TString,double,TString,TString> subsample1=make_tuple("",0.,"",""),tuple<TString,double,TString,TString> subsample2=make_tuple("",0.,"",""),tuple<TString,double,TString,TString> subsample3=make_tuple("",0.,"",""),tuple<TString,double,TString,TString> subsample4=make_tuple("",0.,"",""),tuple<TString,double,TString,TString> subsample5=make_tuple("",0.,"",""),tuple<TString,double,TString,TString> subsample6=make_tuple("",0.,"",""),tuple<TString,double,TString,TString> subsample7=make_tuple("",0.,"","")){
  Sample sample(title,type,color);
  if(get<0>(subsample1)!="") sample.Add(get<0>(subsample1),get<1>(subsample1),get<2>(subsample1),get<3>(subsample1));
  if(get<0>(subsample2)!="") sample.Add(get<0>(subsample2),get<1>(subsample2),get<2>(subsample2),get<3>(subsample2));
  if(get<0>(subsample3)!="") sample.Add(get<0>(subsample3),get<1>(subsample3),get<2>(subsample3),get<3>(subsample3));
  if(get<0>(subsample4)!="") sample.Add(get<0>(subsample4),get<1>(subsample4),get<2>(subsample4),get<3>(subsample4));
  if(get<0>(subsample5)!="") sample.Add(get<0>(subsample5),get<1>(subsample5),get<2>(subsample5),get<3>(subsample5));
  if(get<0>(subsample6)!="") sample.Add(get<0>(subsample6),get<1>(subsample6),get<2>(subsample6),get<3>(subsample6));
  if(get<0>(subsample7)!="") sample.Add(get<0>(subsample7),get<1>(subsample7),get<2>(subsample7),get<3>(subsample7));
  return sample;
}
Sample MakeSample(TString title,Sample::Type type,int color,TString key,double weight=1.,TString prefix="",TString suffix=""){
  Sample sample(title,type,color);
  sample.Add(key,weight,prefix,suffix);
  return sample;
}
Sample MakeSample(TString title,Sample::Type type,int color,TRegexp regexp,double weight=1.,TString prefix="",TString suffix=""){
  Sample sample(title,type,color);
  sample.Add(regexp,weight,prefix,suffix);
  return sample;
}

#endif
