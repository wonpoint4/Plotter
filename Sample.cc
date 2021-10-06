#ifndef __SAMPLE_CC__
#define __SAMPLE_CC__
#include <TPRegexp.h>
#include <TObjString.h>
#include <TSystem.h>
#include"Global.h"
#include"Utils.h"
#include"Style.cc"
class Sample{
public:
  TString title;
  vector<TString> tags;
  TString type;
  vector<Style> styles;
  map<TString,TString> replace;
  double weight=1.;
  vector<Sample> subs;

  Sample(TString title_="",TString tag="",Style style0=Style(),Style style1=Style(),Style style2=Style(),Style style3=Style(),Style style4=Style(),Style style5=Style());
  Sample(TString title_,TString tag,int color,int marker=-1,int fill=-1,TString drawoption="");
  Sample operator+(const Sample& sam);
  Sample operator+(const char* key);
  Sample operator+(const TString key);
  Sample operator+(const TRegexp reg);
  template<class T> Sample operator+=(T sam){*this=(*this)+sam;return *this;}
  Sample operator-(const Sample& sam);
  Sample operator-(const char* key);
  Sample operator-(const TString key);
  Sample operator-(const TRegexp reg);
  template<class T> Sample operator-=(T sam){*this=(*this)-sam;return *this;}
  friend Sample operator%(const char* hprefix,const Sample& sam);
  friend Sample operator%(const TString hprefix,const Sample& sam);
  friend Sample operator%(const Sample& sam,const char* suffix);
  friend Sample operator%(const Sample& sam,const TString suffix);
  Sample operator*(double w) const;
  friend Sample operator*(double w,const Sample& sam);
  template<class T> Sample operator*=(T w){*this=(*this)*w;return *this;}
  void Add(TRegexp sampleregexp,double weight=1.,TString hprefix="",TString suffix="");
  void ApplyStyle(TH1* hist,int index=0) const;
  static bool CheckAttributes(const Sample& sam1, const Sample& sam2);
  TString GetHistPrefix() const;
  TString GetPrefix() const;
  TString GetSuffix() const;
  bool HasTag(TString tag,bool recursive=true) const;
  bool IsCollection() const;
  bool IsStack() const;
  bool IsSum() const;
  bool IsSample() const;
  bool IsFile() const;
  void Print(bool detail=false,TString pre="") const;
  TString ReplaceToString() const;
  void SetHistPrefix(TString hprefix);
  void SetPrefix(TString prefix);
  void SetSuffix(TString suffix);
  void SetStyle(int color,int marker=-1,int fill=-1,TString drawoption="");
  void SetTags(TString tag);
  void SetType(TString type_);
  void TurnOffFillColor();
  void TurnOnFillColor();
};
map<TString,Sample> samples;

Sample::Sample(TString title_,TString tag,Style style0,Style style1,Style style2,Style style3,Style style4,Style style5){
  title=title_;
  SetTags(tag);
  styles={style0,style1,style2,style3,style4,style5};
}
Sample::Sample(TString title_,TString tag,int color,int marker,int fill,TString drawoption){
  title=title_;
  SetTags(tag);
  SetStyle(color,marker,fill,drawoption);
}
Sample Sample::operator+(const Sample& sam){
  Sample temp(*this);
  if(temp.IsCollection()&&sam.IsCollection()){
    temp.subs.push_back(sam);
    if(temp.IsStack()){
      if(temp.subs.back().IsStack()) temp.subs.back().SetType("SUM");
      temp.subs.back().TurnOnFillColor();
    }
  }else if(temp.IsCollection()&&sam.IsSample()){
    temp.subs.push_back(sam);
    if(temp.IsStack()) temp.subs.back().TurnOnFillColor();
  }else if(temp.IsCollection()&&sam.IsFile()){
    Sample sample(sam.title,"SAMPLE");
    sample=sample+sam;
    temp=temp+sample;
  }else if(temp.IsSample()&&sam.IsSample()){
    if(!CheckAttributes(temp,sam))
      PWarning("Some attributes of "+sam.title+" will be ignored");
    temp.subs.insert(temp.subs.end(),sam.subs.begin(),sam.subs.end());
  }else if(temp.IsSample()&&sam.IsFile()){ 
    temp.subs.push_back(sam);
  }else{
    PError("[Sample::operator+] Cannot add samples; "+temp.title+"("+temp.type+") + "+sam.title+"("+sam.type+")");
  }
  return temp;
}
Sample Sample::operator+(const char* key){
  for(const auto& [k,sample]:samples){
    if(k==key) return (*this)+sample;
  }
  PError((TString)"[Sample::operator+(const char* key,const Sample& sam)] no sample with key "+key);
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
  PError((TString)"[Sample operator-(const char* key,const Sample& sam)] no sample with key "+key);
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
Sample operator%(const char* hprefix,const Sample& sam){
  Sample temp(sam);
  temp.SetHistPrefix(hprefix+temp.GetHistPrefix());
  return temp;
}  
Sample operator%(const TString hprefix,const Sample& sam){
  return hprefix.Data()%sam;
}  
Sample operator%(const Sample& sam,const char* suffix){
  Sample temp(sam);
  temp.replace["$"]=temp.replace["$"]+TString(suffix);
  return temp;
}  
Sample operator%(const Sample& sam,const TString suffix){
  return sam%suffix.Data();
}  
Sample Sample::operator*(double f) const {
  Sample temp(*this);
  temp.weight*=f;
  return temp;
}
Sample operator*(double f,const Sample& sam){
  return sam*f;
}
void Sample::Add(TRegexp regexp,double weight,TString hprefix,TString suffix){
  for(auto it=samples.begin();it!=samples.end();it++){
    if(it->first.Contains(regexp)) (*this)=(*this)+weight*(hprefix%(it->second)%suffix);
  }
}
void Sample::ApplyStyle(TH1* hist,int index) const {
  PAll("[Sample::ApplyStyle(TH1* hist,int index=0)]");
  int i=index;
  if(i<0) i=0;
  if(i>=styles.size()) i=styles.size()-1;
  styles[i].Apply(hist);

  if(hist&&index==0){
    hist->SetName(title);
  }
}
bool Sample::CheckAttributes(const Sample& sam1,const Sample& sam2){
  bool ret=true;
  if(sam1.weight!=sam2.weight){
    PWarning("[Sample::CheckAttributes] "+sam1.title+Form("%f",sam1.weight)+" != "+sam2.title+Form("%f",sam2.weight));
    ret=false;
  }
  if(sam1.replace!=sam2.replace){
    PWarning("[Sample::CheckAttributes] "+sam1.title+" "+sam1.ReplaceToString()+" != "+sam2.title+" "+sam2.ReplaceToString());
    ret=false;
  }
  return ret;
}
TString Sample::GetHistPrefix() const {
  auto it=replace.find("/([^/]*)$");
  if(it!=replace.end()){
    TObjArray* array=TPRegexp("/(.*)\\$1$").MatchS(it->second);
    TString hprefix=((TObjString*)array->At(1))->GetString();
    array->Delete();
    return hprefix;
  }else return "";
}
TString Sample::GetPrefix() const {
  auto it=replace.find("^");
  if(it!=replace.end()) return it->second;
  else return "";
}
TString Sample::GetSuffix() const {
  auto it=replace.find("$");
  if(it!=replace.end()) return it->second;
  else return "";
}
bool Sample::HasTag(TString tag,bool recursive) const {
  if(tag=="") return true;
  if(IsCollection()&&recursive){
    if(HasTag(tag,false)) return true;
    for(const auto& sub:subs)
      if(sub.HasTag(tag)) return true;
    return false;
  }else{
    if(tag.Contains(",")||tag.Contains(" ")){
      tag.ReplaceAll(","," ");
      vector<TString> this_tags=Split(tag," ");
      for(TString t:this_tags)
	if(!HasTag(t,false)) return false;
      return true;
    }else{
      if(find(tags.begin(),tags.end(),tag)!=tags.end()) 
	return true;
      return false;
    }
  }
}
bool Sample::IsCollection() const {
  if(IsStack()||IsSum()) return true;
  else return false;
}
bool Sample::IsStack() const {
  if(type=="STACK") return true;
  return false;
}
bool Sample::IsSum() const {
  if(type=="SUM") return true;
  return false;
}
bool Sample::IsSample() const{
  if(type=="SAMPLE") return true;
  return false;
}
bool Sample::IsFile() const {
  if(type=="FILE") return true;
  return false;
}
void Sample::Print(bool detail,TString pre) const{
  if(type=="FILE"){
    cout<<pre<<title<<" "<<gSystem->GetFromPipe("stat -c %.19y "+title)<<endl;
    return;
  }
  if(detail){
    TString weightstring="";
    if(weight==1) weightstring="+";
    else if(weight==-1) weightstring="-";
    else weightstring=Form("%+.1f ",weight);
    cout<<pre<<weightstring<<"Title:"<<title<<" Type:"<<type<<" Tags:"<<Join(",",tags)<<" Replace:"<<ReplaceToString()<<" ";
    styles[0].Print();
    for(const auto& sub:subs){
      sub.Print(detail,pre+"  ");
    }
  }else cout<<pre<<"Title: "<<title<<" "<<"Type:"<<type<<endl;
}
TString Sample::ReplaceToString() const {
  TString out;
  for(const auto& [reg,newstr]:replace){
    out+="{'"+reg+"'->'"+newstr+"'} ";
  }
  return out;
}
void Sample::SetHistPrefix(TString hprefix){
  replace["/([^/]*)$"]="/"+hprefix+"$1";
}
void Sample::SetPrefix(TString prefix){
  replace["^"]=prefix;
}
void Sample::SetSuffix(TString suffix){
  replace["$"]=suffix;
}
void Sample::SetStyle(int color,int marker,int fill,TString drawoption){
  if(styles.size()==0) styles.push_back(Style());
  styles[0]=Style(color,marker,fill,drawoption);
  
  if(IsStack()){
    styles[0].drawoption="e";
  }
}
void Sample::SetTags(TString tag){
  tags.clear();
  tag.ReplaceAll(","," ");
  vector<TString> tags_=Split(tag," ");
  for(auto t:tags_){
    if(t=="STACK"||t=="SUM"||t=="SAMPLE"||t=="FILE"){
      SetType(t);
      continue;
    }
    tags.push_back(t);
  }
}
void Sample::SetType(TString type_){
  type_.ToUpper();
  if(type_!="STACK"&&type_!="SUM"&&type_!="SAMPLE"&&type_!="FILE")
    PError("[Sample::SetType] Unknown type "+type_);
  type=type_;
}
void Sample::TurnOffFillColor(){
  if(!IsStack()) styles[0].fillcolor=-1;
  else
    for(auto& sub:subs)
      sub.TurnOffFillColor();
}  
void Sample::TurnOnFillColor(){
  if(!IsStack()) styles[0].fillcolor=styles[0].linecolor;
  else
    for(auto& sub:subs)
      sub.TurnOnFillColor();
}
#endif
