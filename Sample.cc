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
  map<TString,TString> replace;
  vector<tuple<TString,double,TString,TString>> files; //filename,type,weight,prefix,suffix
  vector<Sample> subs;

  Sample(TString title_="",Sample::Type type_=Sample::Type::UNDEF,int color_=-1,int marker_=-1);
  Sample operator+(const Sample& sam);
  Sample operator+(const char* key);
  Sample operator+(const TString key);
  Sample operator+(const TRegexp reg);
  Sample operator-(const Sample& sam);
  Sample operator-(const char* key);
  Sample operator-(const TString key);
  Sample operator-(const TRegexp reg);
  friend Sample operator%(const char* prefix,const Sample& sam);
  friend Sample operator%(const TString prefix,const Sample& sam);
  friend Sample operator%(const Sample& sam,const char* suffix);
  friend Sample operator%(const Sample& sam,const TString suffix);

  Sample operator*(double w);
  friend Sample operator*(double w,const Sample& sam);
  void SetStyle(int color_,int marker_=-1);
  void SetType(int type_);
  void Add(TRegexp sampleregexp,double weight=1.,TString prefix="",TString suffix="");
  void ApplyStyle(TH1* hist) const;
  void Print(bool detail=false,TString prefix="") const;
  bool IsCollection() const;
};
map<TString,Sample> samples;

Sample::Sample(TString title_,Sample::Type type_,int color_,int marker_){
  title=title_;
  type=type_;
  SetStyle(color_,marker_);
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

Sample Sample::operator-(const Sample& sam){
  return (*this)+(-1.)*sam;
}
Sample Sample::operator-(const char* key){
  for(const auto& [k,sample]:samples){
    if(k==key) return (*this)-sample;
  }
  if(DEBUG>0) cout<<"###ERROR### [Sample operator-(const char* key,const Sample& sam)] no sample with key "<<key<<endl;
  exit(1);
}  
Sample Sample::operator-(const TString key){
  return (*this)-key.Data();
}
Sample Sample::operator-(const TRegexp reg){
  Sample temp(*this);
  for(auto const& [key,sample]:samples)
    if(key.Contains(reg)) temp=temp-sample;
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
void Sample::SetStyle(int color_,int marker_){
  style=Style(color_,marker_);
  
  if(type==Type::STACK){
    style.drawoption="e";
  }
}
void Sample::SetType(int type_){
  if(type==Type::STACK){
    for(auto& sub:subs){
      sub.SetType(type_);
    }
  }else type=(Type)type_;
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
void Sample::Print(bool detail,TString pre) const{
  if(detail){
    cout<<pre<<"Title:"<<title<<" Type:"<<GetTypeString()<<" ";
    style.Print();
    for(const auto& [file,weight,prefix,suffix]:files){
      cout<<pre<<"  "<<file<<" "<<weight<<" "<<prefix<<" "<<suffix<<" "<<gSystem->GetFromPipe("stat -c %.19y "+file)<<endl;
    }
    for(const auto& sub:subs){
      sub.Print(detail,pre+"  ");
    }
  }else cout<<pre<<"Title: "<<title<<" "<<"Type: "<<GetTypeString()<<endl;
}
#endif
