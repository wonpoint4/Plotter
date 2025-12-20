#include"Plotter.cc"
#ifndef __SKFLATPLOTTER_CC__
#define __SKFLATPLOTTER_CC__
class SKFlatPlotter:public Plotter{
public:
  SKFlatPlotter(TString mode_="",TString Analyzer_="");

  TString Analyzer;
  TString AnalyzerSuffix;
  TString SkimTree=".*";

  virtual int Setup(TString mode_,TString Analyzer_);
  virtual TString GetSetupStringForCondor();
  virtual void SetAnalyzer(TString Analyzer_);
  virtual void SetupSamples();
  virtual void SetupSystematics();
};
SKFlatPlotter::SKFlatPlotter(TString mode_,TString Analyzer_){
  Setup(mode_,Analyzer_);
}
void SKFlatPlotter::SetAnalyzer(TString Analyzer_){
  if(Analyzer_=="") return;
  int pos=Analyzer_.Index("_backup");
  if(pos<0) pos=Analyzer_.Length();
  Analyzer=Analyzer_(0,pos);
  AnalyzerSuffix=Analyzer_(pos,Analyzer_.Length());
  return;
}
void SKFlatPlotter::SetupSamples(){

  for(TString era:{"2016preVFP","2016postVFP","2017","2018"}){
    samples["muon"+era]=Sample("data (#mu#mu)","SAMPLE data",kBlack,20)+TRegexp(era+"/.*DATA/"+Analyzer+SkimTree+"_DoubleMuon_[A-Z]")+TRegexp(era+"/.*DATA/"+Analyzer+SkimTree+"_SingleMuon_[A-Z]");
    samples["electron"+era]=Sample("data (ee)","SAMPLE data",kBlack,20)+TRegexp(era+"/.*DATA/"+Analyzer+SkimTree+"_.*EG.*_[A-Z]")+TRegexp(era+"/.*DATA/"+Analyzer+SkimTree+"_SingleElectron_[A-Z]");
    samples["ww"+era]=Sample("WW","SAMPLE sim ww "+era,kOrange)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_WW_pythia$");
    samples["wz"+era]=Sample("WZ","SAMPLE sim wz "+era,kOrange)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_WZ_pythia$");
    samples["zz"+era]=Sample("ZZ","SAMPLE sim zz "+era,kOrange)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_ZZ_pythia$");
    samples["wjets"+era]=Sample("W","SAMPLE sim wjets "+era,kYellow)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_WJets_Sherpa$");
    samples["wjets_mg"+era]=Sample("W (Madgraph)","SAMPLE sim wjets "+era,kYellow)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_WJets_MG$");
    samples["ttll"+era]=Sample("t#bar{t}#rightarrowll","SAMPLE sim tt ttll "+era,kMagenta+1)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_TTLL_powheg$");
    samples["ttlj"+era]=Sample("t#bar{t}#rightarrowlj","SAMPLE sim tt ttlj "+era,kMagenta+2)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_TTLJ_powheg$");
    samples["ttjj"+era]=Sample("t#bar{t}#rightarrowjj","SAMPLE sim tt ttjj "+era,kMagenta+3)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_TTJJ_powheg$");
    samples["tw"+era]=Sample("tW","SAMPLE sim tw "+era,kMagenta+10)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_SingleTop_tW_.*top_NoFullyHad$");
    samples["sts"+era]=Sample("Single top (s-ch)","SAMPLE sim st sts "+era,kMagenta+9)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_SingleTop_sch*");
    samples["stt"+era]=Sample("Single top (t-ch)","SAMPLE sim st stt "+era,kMagenta+8)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_SingleTop_tch*");
    samples["aa"+era]=Sample("#gamma#gamma#rightarrowll","SAMPLE sim aa "+era,kYellow+1)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_GGToLL$");
    samples["qcd"+era]=Sample("QCD multi-jets", "SAMPLE sim qcd "+era,kCyan)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_QCD_bEnriched_HT*");

    samples["amc"+era]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy "+era,kRed)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_DYJets$");
    samples["mi"+era]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy "+era,kRed)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_DYJetsTo.*MiNNLO$");
    samples["miBug"+era]=Sample("#gamma*/Z#rightarrowll (Default)","SAMPLE sim dy "+era,kRed)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_DYJetsToMuMu.*MiNNLO$");
    samples["miFix"+era]=Sample("#gamma*/Z#rightarrowll (H2Fix)","SAMPLE sim dy "+era,kGreen+2)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_DYJetsTo.*MiNNLO_H2ErratumFix_PDFExt$");
    // for(auto& sub:samples["mi"].subs){
    //   if(sub.title.Contains("DYJetsToEE")&&sub.title.Contains("2017"))
    //     sub.weight=0.99;
    // }
    samples["mg"+era]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy "+era,kRed)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_DYJets_MG$");
    samples["amcJet"+era]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy "+era,kRed)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_.*DY[0-9]Jets$");
    samples["amcPt"+era]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy "+era,kRed)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
    samples["amcM"+era]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy "+era,kRed)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_.*DYJets_M-[0-9]*to[0-9Inf]*$");
    samples["mg"+era]=Sample("#gamma*/Z#rightarrowll","SAMPLE sim dy "+era,kRed)+TRegexp(era+"/.*"+Analyzer+SkimTree+"_.*DYJets_MG$");
  }

  samples["data"]=Sample("data","SUM data",kBlack,20)+"muon2016preVFP"+"muon2016postVFP"+"muon2017"+"muon2018"+"electron2016preVFP"+"electron2016postVFP"+"electron2017"+"electron2018";
  samples["ww"]=Sample("WW","SUM",kOrange)+"ww2016preVFP"+"ww2016postVFP"+"ww2017"+"ww2018";
  samples["wz"]=Sample("WZ","SUM",kOrange)+"wz2016preVFP"+"wz2016postVFP"+"wz2017"+"wz2018";
  samples["zz"]=Sample("ZZ","SUM",kOrange)+"zz2016preVFP"+"zz2016postVFP"+"zz2017"+"zz2018";
  samples["wjets"]=Sample("W","SUM",kYellow)+"wjets2016preVFP"+"wjets2016postVFP"+"wjets2017"+"wjets2018";
  samples["wjets_mg"]=Sample("W (Madgraph)","SUM",kYellow)+"wjets_mg2016preVFP"+"wjets_mg2016postVFP"+"wjets_mg2017"+"wjets_mg2018";
  samples["ttll"]=Sample("t#bar{t}#rightarrowll","SUM",kMagenta+1)+"ttll2016preVFP"+"ttll2016postVFP"+"ttll2017"+"ttll2018";
  samples["ttlj"]=Sample("t#bar{t}#rightarrowlj","SUM",kMagenta+2)+"ttlj2016preVFP"+"ttlj2016postVFP"+"ttlj2017"+"ttlj2018";
  samples["ttjj"]=Sample("t#bar{t}#rightarrowjj","SUM",kMagenta+3)+"ttjj2016preVFP"+"ttjj2016postVFP"+"ttjj2017"+"ttjj2018";
  samples["tw"]=Sample("tW","SUM",kMagenta+10)+"tw2016preVFP"+"tw2016postVFP"+"tw2017"+"tw2018";
  samples["sts"]=Sample("Single top (s-ch)","SUM",kMagenta+9)+"sts2016preVFP"+"sts2016postVFP"+"sts2017"+"sts2018";
  samples["stt"]=Sample("Single top (t-ch)","SUM",kMagenta+8)+"stt2016preVFP"+"stt2016postVFP"+"stt2017"+"stt2018";
  samples["aa"]=Sample("#gamma#gamma#rightarrowll","SUM",kYellow+1)+"aa2016preVFP"+"aa2016postVFP"+"aa2017"+"aa2018";
  samples["qcd"]=Sample("QCD multi-jets", "SUM",kCyan)+"qcd2016preVFP"+"qcd2016postVFP"+"qcd2017"+"qcd2018";
  
  samples["amc"]=Sample("#gamma*/Z#rightarrowll","SUM",kRed)+"amc2016preVFP"+"amc2016postVFP"+"amc2017"+"amc2018";
  samples["mi"]=Sample("#gamma*/Z#rightarrowll","SUM",kRed)+"mi2016preVFP"+"mi2016postVFP"+"mi2017"+"mi2018";
  samples["miBug"]=Sample("#gamma*/Z#rightarrowll (Default)","SUM sim dy",kRed)+"miBug2016preVFP"+"miBug2016postVFP"+"miBug2017"+"miBug2018";
  samples["miFix"]=Sample("#gamma*/Z#rightarrowll (H2Fix)","SUM sim dy",kGreen+2)+"miFix2016preVFP"+"miFix2016postVFP"+"miFix2017"+"miFix2018";
  // for(auto& sub:samples["mi"].subs){
  //   if(sub.title.Contains("DYJetsToEE")&&sub.title.Contains("2017"))
  //     sub.weight=0.99;
  // }
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SUM",kRed)+"mg2016preVFP"+"mg2016postVFP"+"mg2017"+"mg2018";
  samples["amcJet"]=Sample("#gamma*/Z#rightarrowll","SUM",kRed)+"amcJet2016preVFP"+"amcJet2016postVFP"+"amcJet2017"+"amcJet2018";
  samples["amcPt"]=Sample("#gamma*/Z#rightarrowll","SUM",kRed)+"amcPt2016preVFP"+"amcPt2016postVFP"+"amcPt2017"+"amcPt2018";
  samples["amcM"]=Sample("#gamma*/Z#rightarrowll","SUM",kRed)+"amcM2016preVFP"+"amcM2016postVFP"+"amcM2017"+"amcM2018";
  samples["mg"]=Sample("#gamma*/Z#rightarrowll","SUM",kRed)+"mg2016preVFP"+"mg2016postVFP"+"mg2017"+"mg2018";

  samples["st"]=Sample("Single top","SUM",kMagenta+3)+"sts"+"stt";
  samples["tt"]=Sample("t#bar{t}","SUM",kMagenta)+"ttll"+"ttlj"+"ttjj";
  samples["tttw"]=Sample("t#bar{t}, tW","SUM",kMagenta)+"tt"+"tw";
  samples["ttst"]=Sample("t#bar{t}, single top","SUM",kMagenta)+"tt"+"st";
  samples["vv"]=Sample("Diboson","SUM",kOrange)+"ww"+"wz"+"zz";

  TString dysamples[]={"amc","amcJet","amcPt","amcM","mg","mi", "miBug", "miFix"};
  for(auto dysample:dysamples){
    samples["bx_"+dysample]="bx_"%(Sample("#gamma*/Z#rightarrowll (b init)","SUM",kRed-5)+dysample); //temp
    samples["tau_"+dysample]="tau_"%(Sample("#gamma*/Z#rightarrow#tau#tau","SUM dytau",kGreen)+dysample);
    samples["tau_"+dysample].RemoveTag("dy");

    samples["dyb_"+dysample] = "dyb_" %(Sample("DY+b", "SUM sim dy", TColor::GetColor("#5790fc")) + dysample);
    samples["dybbar_"+dysample]= "dybbar_" %(Sample("DY+#bar{b}", "SUM sim dy", TColor::GetColor("#f89c20")) + dysample);
    samples["dyc_"+dysample] = "dyc_" %(Sample("DY+c", "SUM sim dy", kYellow) + dysample);
    samples["dycbar_"+dysample] = "dycbar_" %(Sample("DY+#bar{c}", "SUM sim dy", kRed-5) + dysample);
    samples["dyudsg_"+dysample] = "dyudsg_" %(Sample("DY+udsg", "SUM sim dy", kRed-10) + dysample);

    samples["bG_"+dysample] = "[bB]G_" %(Sample("bg#rightarrowZb", "SUM sim dy", TColor::GetColor("#5790fc")) + dysample);
    samples["cG_"+dysample] = "[cC]G_" %(Sample("cg#rightarrowZc", "SUM sim dy", TColor::GetColor("#f89c20")) + dysample);
    samples["sG_"+dysample] = "[sS]G_" %(Sample("sg#rightarrowZs", "SUM sim dy", kGreen+2) + dysample);
    samples["uG_"+dysample] = "[uU]G_" %(Sample("ug#rightarrowZu", "SUM sim dy", kBlue) + dysample);
    samples["dG_"+dysample] = "[dD]G_" %(Sample("dg#rightarrowZd", "SUM sim dy", kRed) + dysample);

    samples["lhe_"+dysample]="lhe_"%(Sample("#gamma*/Z#rightarrowll (LHE)","SUM",kBlue)+dysample);
    samples["gen_"+dysample]="gen_"%(Sample("#gamma*/Z#rightarrowll (GEN)","SUM",kGreen)+dysample);
    samples["genfid_"+dysample]="genfid_"%(Sample("#gamma*/Z#rightarrowll (GEN fiducial)","SUM",kMagenta)+dysample);
    samples["truth_"+dysample]="truth_"%(Sample("#gamma*/Z#rightarrowll (truth)","SUM",kCyan)+dysample);
    samples["ss_"+dysample]="ss_"%(Sample("QCD multi-jets","SUM qcd",kCyan)+"data"-dysample-("tau_"+dysample)-"vv"-"wjets"-"tt"-"st"-"aa");
    samples["fake_"+dysample]="fake_"%(Sample("QCD multi-jets","SUM",kOrange)+"data"-dysample-("tau_"+dysample)-"vv"-"wjets"-"tt"-"st"-"aa");
  }

  samples["bG_miold"] = "[bB]G_" %(Sample("bg#rightarrowZb (Default)", "SUM sim dy", TColor::GetColor("#5790fc")) + "miBug2018");
  samples["bG_minew"] = "[bB]G_" %(Sample("bg#rightarrowZb (H2Fix)", "SUM sim dy", TColor::GetColor("#f89c20")) + "miFix2018");

  samples["ss"] = samples["ss_mi"];
  samples["amcPt_stack"]=Sample("DY Pt-binned","STACK",kBlue)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_Pt-[0-9]*To[0-9Inf]*$");
  for(auto& sub:samples["amcPt_stack"].subs) sub.title=sub.title(TRegexp("Pt-[0-9]*To[0-9Inf]*"));
  samples["amcM_stack"]=Sample("DY M-binned","STACK",kBlue)+TRegexp("/"+Analyzer+SkimTree+"_.*DYJets_M-[0-9]*to[0-9Inf]*$");
  for(auto& sub:samples["amcM_stack"].subs) sub.title=sub.title(TRegexp("M-[0-9]*to[0-9Inf]*"));

  samples["dyall"] = Sample("DY bkgs", "SUM", TColor::GetColor("#e42536")) + "mi" + "dyc_mi" + "dycbar_mi";
  samples["ttall"] = Sample("top bkgs", "SUM", TColor::GetColor("#964a8b")) + "tt" + "st" + "tw";
  samples["ss_won"] = "ss_"%(Sample("QCD multi-jets","SUM qcd",kCyan)+"data"-"dyb_mi"-"dybbar_mi"-"dyall"-"ttall"-"wjets"-"vv"-"aa");
  samples["ewkall"] = Sample("Others", "SUM", TColor::GetColor("#9c9ca1")) + "wjets" + "vv" + "ss_won" + "aa";

  // For ttljPlotter
  samples["ttlj_2b"] = Sample("TTLJ", "SAMPLE sim tt ttlj", kBlue) + TRegexp("/.*" + Analyzer + SkimTree + "_TTLJ_powheg$");
  samples["ttll_2b"] = Sample("TTLL", "SAMPLE sim tt ttll", kMagenta+1) + TRegexp("/.*" + Analyzer + SkimTree + "_TTLL_powheg$");
  samples["ttjj_2b"] = Sample("TTJJ", "SAMPLE sim tt ttjj", kMagenta+3) + TRegexp("/.*" + Analyzer + SkimTree + "_TTJJ_powheg$");
  samples["tw_2b"] = Sample("tW", "SAMPLE sim tw", kRed-6) + TRegexp("/.*" + Analyzer + SkimTree + "_SingleTop_tW_.*top_NoFullyHad$");
  samples["sts_2b"] = Sample("Single top (s-ch)", "SAMPLE sim st sts", kMagenta+9) + TRegexp("/.*" + Analyzer + SkimTree + "_SingleTop_sch*");
  samples["stt_2b"] = Sample("Single top (t-ch)", "SAMPLE sim st stt", kMagenta+8) + TRegexp("/.*" + Analyzer + SkimTree + "_SingleTop_tch*");
  samples["qcd_2b"] = Sample("QCD multi-jets", "SAMPLE sim qcd", TColor::GetColor("#92dadd")) + TRegexp("/.*" + Analyzer + SkimTree + "_QCD_bEnriched_HT*");
  samples["wjets_2b"] = Sample("WJets", "SAMPLE sim wjets", kYellow) + TRegexp("/.*" + Analyzer + SkimTree + "_WJets_Sherpa$");
  samples["wjets_mg_2b"] = Sample("WJets (Madgraph)", "SAMPLE sim wjets", kYellow) + TRegexp("/.*" + Analyzer + SkimTree + "_WJets_MG$");
  samples["mi_2b"] = Sample("DYJets", "SAMPLE sim dy", kGreen + 2) + TRegexp("/.*" + Analyzer + SkimTree + "_DYJetsTo.*MiNNLO$");
  samples["ww_2b"] = Sample("WW", "SAMPLE sim ww", kOrange) + TRegexp("/.*" + Analyzer + SkimTree + "_WW_pythia$");
  samples["wz_2b"] = Sample("WZ", "SAMPLE sim wz", kOrange) + TRegexp("/.*" + Analyzer + SkimTree + "_WZ_pythia$");
  samples["zz_2b"] = Sample("ZZ", "SAMPLE sim zz", kOrange) + TRegexp("/.*" + Analyzer + SkimTree + "_ZZ_pythia$");
  samples["aa_2b"] = Sample("#gamma#gamma#rightarrowll", "SAMPLE sim aa", kYellow + 1) + TRegexp("/.*" + Analyzer + SkimTree + "_GGToLL$");

  samples["correct_ttlj"] = "Correct_" %(Sample("TTLJ (Correct b-pair)", "SAMPLE sim tt ttlj", TColor::GetColor("#3f90da")) + "ttlj_2b");
  samples["wrong_ttlj"] = "Wrong_" %(Sample("TTLJ (Opposite b-pair)", "SAMPLE sim tt ttlj", TColor::GetColor("#ffa90e")) + "ttlj_2b");
  samples["unmatched_ttlj"] = "UnMatched_" %(Sample("TTLJ (b-pair not Matched)", "SAMPLE sim tt ttlj", TColor::GetColor("#bd1f01")) + "ttlj_2b");
  samples["st_2b"] = Sample("Single top", "SUM", kMagenta) + "sts_2b" + "stt_2b";
  samples["vv_2b"] = Sample("Diboson", "SUM", kOrange) + "ww_2b" + "wz_2b" + "zz_2b";
  samples["stqcd_2b"] = Sample("Single top,  QCD multi-jets", "SUM", kCyan) + "st_2b" + "qcd_2b";

  samples["ttall_2b"] = Sample("TT bkgs", "SUM", TColor::GetColor("#94a4a2")) + "ttll_2b" + "ttjj_2b";
  samples["stall_2b"] = Sample("single top bkgs", "SUM", TColor::GetColor("#832db6")) + "tw_2b" + "st_2b";
  samples["ewkall_2b"] = Sample("EWK bkgs", "SUM", TColor::GetColor("#e76300")) + "wjets_2b" + "mi_2b" + "vv_2b" + "aa_2b";

  samples["correct0_ttlj"] = "Correct0_" %(Sample("TTLJ (Correct b-pair) in A", "SAMPLE sim tt ttlj", kGreen) + "ttlj_2b");
  samples["wrong0_ttlj"] = "Wrong0_" %(Sample("TTLJ (Opposite b-pair) in A", "SAMPLE sim tt ttlj", kBlue) + "ttlj_2b");
  samples["unmatched0_ttlj"] = "UnMatched0_" %(Sample("TTLJ (b-pair not Matched) in A", "SAMPLE sim tt ttlj", kRed) + "ttlj_2b");
  samples["correct1_ttlj"] = "Correct1_" %(Sample("TTLJ (Correct b-pair) not in A", "SAMPLE sim tt ttlj", kGreen + 2) + "ttlj_2b");
  samples["wrong1_ttlj"] = "Wrong1_" %(Sample("TTLJ (Opposite b-pair) not in A", "SAMPLE sim tt ttlj", kBlue + 2) + "ttlj_2b");
  samples["unmatched1_ttlj"] = "UnMatched1_" %(Sample("TTLJ (b-pair not Matched) not in A", "SAMPLE sim tt ttlj", kRed + 2) + "ttlj_2b");

  samples["qcdss_ee"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eEl]&&!0bjet&&!nbjet qcdss_ee",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.043;
  samples["qcdss_ee_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eEl]&&0bjet qcdss_ee_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.044;
  samples["qcdss_ee_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][eEl]&&nbjet qcdss_ee_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.007;
  samples["qcdss_mm"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mMu]&&!0bjet&&!nbjet qcdss_mm",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*2.252;
  samples["qcdss_mm_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mMu]&&0bjet qcdss_mm_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*2.284;
  samples["qcdss_mm_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][mMu]&&nbjet qcdss_mm_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.192;
  samples["qcdss_em"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&!0bjet&&!nbjet qcdss_em",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_em_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&0bjet qcdss_em_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_em_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[eE][mM]&&nbjet qcdss_em_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_me"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&!0bjet&&!nbjet qcdss_me",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_me_0bjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&0bjet qcdss_me_0bjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  samples["qcdss_me_nbjet"]="ss_"%(Sample("QCD multi-jet (SS-method)","SUM filter:[mM][eE]&&nbjet qcdss_me_nbjet",kCyan)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa")*1.000;
  for(auto& [key,sample]:samples){
    if(!key.Contains("qcdss_")) continue;
    for(auto& sub:sample.subs){
      if(!sub.HasTag("data")) sub.AddTag("qcdss_prompt");
    }
  }

  samples["qcdss"]=Sample("QCD multi-jet","SUM",Style(kCyan),Style(kCyan,-1,3004,"e2"))+"qcdss_ee"+"qcdss_ee_0bjet"+"qcdss_ee_nbjet"+"qcdss_mm"+"qcdss_mm_0bjet"+"qcdss_mm_nbjet"+"qcdss_em"+"qcdss_em_0bjet"+"qcdss_em_nbjet"+"qcdss_me"+"qcdss_me_0bjet"+"qcdss_me_nbjet";

  samples["fake_mi"]="fake_"%(Sample("QCD multi-jet (TF method)","SUM",Style(kRed),Style(kRed,-1,3005,"e2"))+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["qcdtf"]=samples["fake_mi"];
  samples["fakecpt_mi"]="fakecpt_"%(Sample("QCD multi-jet (TF, cone pT)","SUM",kBlue)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["fakempt_mi"]="fakempt_"%(Sample("QCD multi-jet (TF, modified pT)","SUM",kGreen)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");
  samples["fakelj_mi"]="fakelj_"%(Sample("QCD multi-jet (TF, lj)","SUM",kMagenta)+"data"-"mi"-"tau_mi"-"vv"-"wjets"-"ttst"-"aa");

  samples["zp"]=Sample("Z'","SAMPLE sim",kRed)+TRegexp("/"+Analyzer+SkimTree+"_ZPrime$");
  samples["zpsb"]=Sample("Z'","SAMPLE sim",kGreen+2)+TRegexp("/"+Analyzer+SkimTree+"_ZPrimeToMuMuSB$");
  samples["bbllpll"]=Sample("bbll (#eta_{LL})","SAMPLE sim",kBlue)+TRegexp("/"+Analyzer+SkimTree+"_bbll_M-[0-9]*To[0-9Inf]*_Lambda-10TeV_posEtaLL$");
  samples["bbllplr"]=Sample("bbll (#eta_{LR})","SAMPLE sim",kRed)+TRegexp("/"+Analyzer+SkimTree+"_bbll_M-[0-9]*To[0-9Inf]*_Lambda-10TeV_posEtaLR$");
  samples["bbllprl"]=Sample("bbll (#eta_{RL})","SAMPLE sim",kGreen+1)+TRegexp("/"+Analyzer+SkimTree+"_bbll_M-[0-9]*To[0-9Inf]*_Lambda-10TeV_posEtaRL$");
  samples["bbllprr"]=Sample("bbll (#eta_{RR})","SAMPLE sim",kCyan+1)+TRegexp("/"+Analyzer+SkimTree+"_bbll_M-[0-9]*To[0-9Inf]*_Lambda-10TeV_posEtaRR$");
}

int SKFlatPlotter::Setup(TString mode_,TString Analyzer_){
  Reset();
  SetAnalyzer(Analyzer_);
  if(Analyzer!="") ScanFiles((TString)getenv("SKFlatOutputDir")+getenv("SKFlatV")+"/"+Analyzer+AnalyzerSuffix+"/");
  SetupSamples();
  SetupEntries(mode_);
  SetupSystematics();

  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[Setup] nentries: "<<entries.size()<<endl;
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[Setup] nsys: "<<systematics.size()<<endl;

  return 1;
}
TString SKFlatPlotter::GetSetupStringForCondor(){
  return "Setup\\(\\\\\\\""+mode+"\\\\\\\",\\\\\\\""+Analyzer+AnalyzerSuffix+"\\\\\\\"\\)";
}
void SKFlatPlotter::SetupSystematics(){
  if(Verbosity>VERBOSITY::WARNING) std::cout<<"[SetupSystematics]"<<endl;

  // SYS
  AddSystematic("JES_data_2016a", "JES (data 2016a)", Systematic::Type::ENVELOPE, "_jet_scale_up _jet_scale_down", "data&&2016preVFP");
  AddSystematic("JES_data_2016b", "JES (data 2016b)", Systematic::Type::ENVELOPE, "_jet_scale_up _jet_scale_down", "data&&2016postVFP");
  AddSystematic("JES_data_2017", "JES (data 2017)", Systematic::Type::ENVELOPE, "_jet_scale_up _jet_scale_down", "data&&2017");
  AddSystematic("JES_data_2018", "JES (data 2018)", Systematic::Type::ENVELOPE, "_jet_scale_up _jet_scale_down", "data&&2018");
  AddSystematic("JES_data", "JES (data)", Systematic::Type::MULTI, "JES_data_2016a JES_data_2016b JES_data_2017 JES_data_2018");
  AddSystematic("JES_sim_2016a", "JES (sim. 2016a)", Systematic::Type::ENVELOPE, "_jet_scale_up _jet_scale_down", "sim&&2016preVFP");
  AddSystematic("JES_sim_2016b", "JES (sim. 2016b)", Systematic::Type::ENVELOPE, "_jet_scale_up _jet_scale_down", "sim&&2016postVFP");
  AddSystematic("JES_sim_2017", "JES (sim. 2017)", Systematic::Type::ENVELOPE, "_jet_scale_up _jet_scale_down", "sim&&2017");
  AddSystematic("JES_sim_2018", "JES (sim. 2018)", Systematic::Type::ENVELOPE, "_jet_scale_up _jet_scale_down", "sim&&2018");
  AddSystematic("JES_sim", "JES (sim.)", Systematic::Type::MULTI, "JES_sim_2016a JES_sim_2016b JES_sim_2017 JES_sim_2018");
  AddSystematic("JES", "JES", Systematic::Type::MULTI, "JES_data JES_sim");

  AddSystematic("JER_2016a", "JER (2016a)", Systematic::Type::ENVELOPE, "_jet_smear_up _jet_smear_down", "sim&&2016preVFP");
  AddSystematic("JER_2016b", "JER (2016b)", Systematic::Type::ENVELOPE, "_jet_smear_up _jet_smear_down", "sim&&2016postVFP");
  AddSystematic("JER_2017", "JER (2017)", Systematic::Type::ENVELOPE, "_jet_smear_up _jet_smear_down", "sim&&2017");
  AddSystematic("JER_2018", "JER (2018)", Systematic::Type::ENVELOPE, "_jet_smear_up _jet_smear_down", "sim&&2018");
  AddSystematic("JER", "JER", Systematic::Type::MULTI, "JER_2016a JER_2016b JER_2017 JER_2018");

  AddSystematic("prefireweight", "prefiring unc.", Systematic::Type::ENVELOPE, "_prefireweight_up _prefireweight_down", "sim");
  AddSystematic("PUweight", "PU", Systematic::Type::ENVELOPE, "_PUweight_up _PUweight_down", "sim");
  AddSystematic("PUIDSF", "PUID SF", Systematic::Type::ENVELOPE, "_PUjetSF_up _PUjetSF_down", "sim");

  AddSystematic("btagSF_hcorr", "b tagging SF (heavy, correlated)", Systematic::Type::ENVELOPE, "_btagSF_hcorr", "sim");
  AddSystematic("btagSF_huncorr2016a", "b tagging SF (heavy, uncorrelated 2016a)", Systematic::Type::ENVELOPE, "_btagSF_huncorr", "sim&&2016preVFP");
  AddSystematic("btagSF_huncorr2016b", "b tagging SF (heavy, uncorrelated 2016b)", Systematic::Type::ENVELOPE, "_btagSF_huncorr", "sim&&2016postVFP");
  AddSystematic("btagSF_huncorr2017", "b tagging SF (heavy, uncorrelated 2017)", Systematic::Type::ENVELOPE, "_btagSF_huncorr", "sim&&2017");
  AddSystematic("btagSF_huncorr2018", "b tagging SF (heavy, uncorrelated 2018)", Systematic::Type::ENVELOPE, "_btagSF_huncorr", "sim&&2018");
  AddSystematic("btagSF_h", "b tagging SF (heavy)", Systematic::Type::MULTI, "btagSF_hcorr btagSF_huncorr2016a btagSF_huncorr2016b btagSF_huncorr2017 btagSF_huncorr2018");
  AddSystematic("btagSF_lcorr", "b tagging SF (light, correlated)", Systematic::Type::ENVELOPE, "_btagSF_lcorr", "sim");
  AddSystematic("btagSF_luncorr2016a", "b tagging SF (light, uncorrelated 2016a)", Systematic::Type::ENVELOPE, "_btagSF_luncorr", "sim&&2016preVFP");
  AddSystematic("btagSF_luncorr2016b", "b tagging SF (light, uncorrelated 2016b)", Systematic::Type::ENVELOPE, "_btagSF_luncorr", "sim&&2016postVFP");
  AddSystematic("btagSF_luncorr2017", "b tagging SF (light, uncorrelated 2017)", Systematic::Type::ENVELOPE, "_btagSF_luncorr", "sim&&2017");
  AddSystematic("btagSF_luncorr2018", "b tagging SF (light, uncorrelated 2018)", Systematic::Type::ENVELOPE, "_btagSF_luncorr", "sim&&2018");
  AddSystematic("btagSF_l", "b tagging SF (heavy)", Systematic::Type::MULTI, "btagSF_lcorr btagSF_luncorr2016a btagSF_luncorr2016b btagSF_luncorr2017 btagSF_luncorr2018");
  AddSystematic("btagSF", "b tagging SF", Systematic::Type::MULTI, "btagSF_h btagSF_l");

  AddSystematic("bcharge0SF", "b charge SF (0-bin)", Systematic::Type::HESSIAN, "_bChargeSF1_up0 _bChargeSF1_down0", "sim");
  AddSystematic("bcharge1SF", "b charge SF (1-bin)", Systematic::Type::HESSIAN, "_bChargeSF1_up1 _bChargeSF1_down1", "sim");
  AddSystematic("bcharge2SF", "b charge SF (2-bin)", Systematic::Type::HESSIAN, "_bChargeSF1_up2 _bChargeSF1_down2", "sim");
  AddSystematic("bcharge3SF", "b charge SF (3-bin)", Systematic::Type::HESSIAN, "_bChargeSF1_up3 _bChargeSF1_down3", "sim");
  AddSystematic("bcharge4SF", "b charge SF (4-bin)", Systematic::Type::HESSIAN, "_bChargeSF1_up4 _bChargeSF1_down4", "sim");
  AddSystematic("bcharge5SF", "b charge SF (5-bin)", Systematic::Type::HESSIAN, "_bChargeSF1_up5 _bChargeSF1_down5", "sim");
  AddSystematic("bchargeSF", "b charge SF", Systematic::Type::MULTI, "bcharge0SF bcharge1SF bcharge2SF bcharge3SF bcharge4SF bcharge5SF");

  AddSystematic("lumi2016", "lumi2016", Systematic::Type::ENVELOPE, "scale:1.01", "sim&&(2016preVFP||2016postVFP)");
  AddSystematic("lumi2017", "lumi2017", Systematic::Type::ENVELOPE, "scale:1.02", "sim&&2017");
  AddSystematic("lumi2018", "lumi2018", Systematic::Type::ENVELOPE, "scale:1.015", "sim&&2018");
  AddSystematic("lumi678", "lumi678", Systematic::Type::CORRELATED, "scale:1.006:sim&&(2016preVFP||2016postVFP) scale:1.009:sim&&2017 scale:1.02:sim&&2018");
  AddSystematic("lumi78", "lumi78", Systematic::Type::CORRELATED, "scale:1.006:sim&&2017 scale:1.002:sim&&2018");
  AddSystematic("lumi", "lumi.", Systematic::Type::MULTI, "lumi2016 lumi2017 lumi2018 lumi678 lumi78");

  AddSystematic("CFSF", "charge-flip", Systematic::Type::ENVELOPE, "_CFSF_up _CFSF_down", "sim");

  AddSystematic("exp", "experimental", Systematic::Type::MULTI, "JES JER prefireweight PUweight PUIDSF btagSF bchargeSF lumi CFSF");
  AddSystematic("exp_2b", "experimental", Systematic::Type::MULTI, "JES JER prefireweight PUweight PUIDSF btagSF lumi");

  // PDFSYS
  AddSystematic("dymuf", "DY #mu_{F}", Systematic::Type::ENVELOPE, "_scalevariation1 _scalevariation2", "dy");
  AddSystematic("dymur", "DY #mu_{R}", Systematic::Type::ENVELOPE, "_scalevariation3 _scalevariation6", "dy");
  AddSystematic("ttmuf", "t#bar{t} #mu_{F}", Systematic::Type::ENVELOPE, "_scalevariation1 _scalevariation2", "tt");
  AddSystematic("ttmur", "t#bar{t} #mu_{R}", Systematic::Type::ENVELOPE, "_scalevariation3 _scalevariation6", "tt");
  AddSystematic("dyscale", "DY #mu_{R}/#mu_{F}", Systematic::Type::ENVELOPE, "_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8", "dy");
  AddSystematic("ttscale", "t#bar{t} #mu_{R}/#mu_{F}", Systematic::Type::ENVELOPE, "_scalevariation0 _scalevariation1 _scalevariation2 _scalevariation3 _scalevariation4 _scalevariation6 _scalevariation8", "tt");
  AddSystematic("scale", "#mu_{R}/#mu_{F}", Systematic::Type::MULTI, "dyscale ttscale");

  AddSystematic("alphaS", "#alpha_{S}", Systematic::Type::ENVELOPE, "_alphaS_up _alphaS_down", "sim");
  AddSystematic("isr", "ISR", Systematic::Type::ENVELOPE, "_ISR_up _ISR_down", "sim");
  AddSystematic("fsr", "FSR", Systematic::Type::ENVELOPE, "_FSR_up _FSR_down", "sim");
  AddSystematic("pdf", "PDF", Systematic::Type::HESSIAN, FormRange("_pdf%d", Range(100)), "sim");

  AddSystematic("wxsec", "W xsec", Systematic::Type::ENVELOPE, "scale:1.038", "wjets");
  AddSystematic("ttllxsec", "ttll xsec", Systematic::Type::ENVELOPE, "scale:1.061", "ttll");
  AddSystematic("ttljxsec", "ttlj xsec", Systematic::Type::ENVELOPE, "scale:1.061", "ttlj");
  AddSystematic("twxsec", "tW xsec", Systematic::Type::ENVELOPE, "scale:1.054", "tw");
  AddSystematic("sttxsec", "st (t-ch) xsec", Systematic::Type::ENVELOPE, "scale:1.042", "stt");
  AddSystematic("stsxsec", "st (s-ch) xsec", Systematic::Type::ENVELOPE, "scale:1.039", "sts");
  AddSystematic("wwxsec", "WW xsec", Systematic::Type::ENVELOPE, "scale:1.025", "ww");
  AddSystematic("wzxsec", "WZ xsec", Systematic::Type::ENVELOPE, "scale:1.061", "wz");
  AddSystematic("zzxsec", "ZZ xsec", Systematic::Type::ENVELOPE, "scale:1.049", "zz");
  AddSystematic("aaxsec", "#gamma#gamma xsec", Systematic::Type::ENVELOPE, "scale:1.3", "aa");
  AddSystematic("qcdxsec", "QCD xsec", Systematic::Type::ENVELOPE, "scale:1.3", "qcd");
  AddSystematic("xsec", "xsec", Systematic::Type::MULTI, "wxsec ttllxsec ttljxsec twxsec sttxsec stsxsec wwxsec wzxsec zzxsec aaxsec qcdxsec");

  AddSystematic("notopptweight", "notopptweight", Systematic::Type::ENVELOPE, "_notopptweight", "tt");

  AddSystematic("nozptweight", "no DY Z p_{T} reweight", Systematic::Type::ENVELOPE, "_noZpt", "dy");
  AddSystematic("zptweight_gym", "DY Z p_{T} reweight (GYM)", Systematic::Type::ENVELOPE, "_zptweight_gym", "dy");
  AddSystematic("zptweight", "DY Z p_{T} reweight", Systematic::Type::MULTI, "nozptweight zptweight_gym", "dy");

  AddSystematic("noweakweight", "DY Weak correction", Systematic::Type::ENVELOPE, "_noweakweight", "dy");

  AddSystematic("theo", "theoretical", Systematic::Type::MULTI, "scale alphaS isr fsr pdf xsec notopptweight zptweight noweakweight");
  AddSystematic("theo_2b", "theoretical", Systematic::Type::MULTI, "scale alphaS isr fsr xsec notopptweight");
  AddSystematic("total", "total", Systematic::Type::MULTI, "exp theo");
  AddSystematic("total_2b", "experimental", Systematic::Type::MULTI, "exp_2b theo_2b");

  // LEPSYS
  AddSystematic("muonTrackingeffSF_sys1", "eff. #mu Trk sys1", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s1m0", "sim");
  AddSystematic("muonTrackingeffSF_sys2", "eff. #mu Trk sys2", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s2m0", "sim");
  AddSystematic("muonTrackingeffSF_sys3", "eff. #mu Trk sys3", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s3m0", "sim");
  AddSystematic("muonTrackingeffSF_sys4", "eff. #mu Trk sys4", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s4m0", "sim");
  AddSystematic("muonTrackingeffSF_sys5", "eff. #mu Trk sys5", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s5m0", "sim");
  AddSystematic("muonTrackingeffSF_sys6", "eff. #mu Trk sys6", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s6m0", "sim");
  AddSystematic("muonTrackingeffSF_sys7", "eff. #mu Trk sys7", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s7m0 _muonTrackingeffSF_s7m1", "sim");
  AddSystematic("muonTrackingeffSF_sys8", "eff. #mu Trk sys8", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s8m0 _muonTrackingeffSF_s8m1", "sim");
  AddSystematic("muonTrackingeffSF_sys9", "eff. #mu Trk sys9", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s9m0", "sim");
  AddSystematic("muonTrackingeffSF_sys10", "eff. #mu Trk sys10", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s10m0", "sim");
  AddSystematic("muonTrackingeffSF_sys11", "eff. #mu Trk sys11", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s11m0 _muonTrackingeffSF_s11m1", "sim");
  AddSystematic("muonTrackingeffSF_sys12", "eff. #mu Trk sys12", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s12m0 _muonTrackingeffSF_s12m1", "sim");
  AddSystematic("muonTrackingeffSF_sys13", "eff. #mu Trk sys13", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s13m0 _muonTrackingeffSF_s13m1", "sim");
  AddSystematic("muonTrackingeffSF_sys14", "eff. #mu Trk sys14", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s14m0", "sim");
  AddSystematic("muonTrackingeffSF_sys15", "eff. #mu Trk sys15", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s15m0", "sim");
  AddSystematic("muonTrackingeffSF_sys16", "eff. #mu Trk sys16", Systematic::Type::ENVELOPE, "_muonTrackingeffSF_s16m0", "sim");
  AddSystematic("muonTrackingeffSF", "eff. #mu Trk syst.", Systematic::Type::MULTI, FormRange("muonTrackingeffSF_sys%d", Range(1, 17)), "sim");

  AddSystematic("muonRECOeffSF_sys1", "eff. #mu RECO sys1", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s1m0", "sim");
  AddSystematic("muonRECOeffSF_sys2", "eff. #mu RECO sys2", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s2m0", "sim");
  AddSystematic("muonRECOeffSF_sys3", "eff. #mu RECO sys3", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s3m0", "sim");
  AddSystematic("muonRECOeffSF_sys4", "eff. #mu RECO sys4", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s4m0", "sim");
  AddSystematic("muonRECOeffSF_sys5", "eff. #mu RECO sys5", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s5m0", "sim");
  AddSystematic("muonRECOeffSF_sys6", "eff. #mu RECO sys6", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s6m0", "sim");
  AddSystematic("muonRECOeffSF_sys7", "eff. #mu RECO sys7", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s7m0 _muonRECOeffSF_s7m1", "sim");
  AddSystematic("muonRECOeffSF_sys8", "eff. #mu RECO sys8", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s8m0", "sim");
  AddSystematic("muonRECOeffSF_sys9", "eff. #mu RECO sys9", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s9m0", "sim");
  AddSystematic("muonRECOeffSF_sys10", "eff. #mu RECO sys10", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s10m0", "sim");
  AddSystematic("muonRECOeffSF_sys11", "eff. #mu RECO sys11", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s11m0 _muonRECOeffSF_s11m1", "sim");
  AddSystematic("muonRECOeffSF_sys12", "eff. #mu RECO sys12", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s12m0 _muonRECOeffSF_s12m1", "sim");
  AddSystematic("muonRECOeffSF_sys13", "eff. #mu RECO sys13", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s13m0 _muonRECOeffSF_s13m1", "sim");
  AddSystematic("muonRECOeffSF_sys14", "eff. #mu RECO sys14", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s14m0", "sim");
  AddSystematic("muonRECOeffSF_sys15", "eff. #mu RECO sys15", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s15m0", "sim");
  AddSystematic("muonRECOeffSF_sys16", "eff. #mu RECO sys16", Systematic::Type::ENVELOPE, "_muonRECOeffSF_s16m0", "sim");
  AddSystematic("muonRECOeffSF", "eff. #mu RECO syst.", Systematic::Type::MULTI, FormRange("muonRECOeffSF_sys%d", Range(1, 17)), "sim");

  AddSystematic("muonIDeffSF_sys1", "eff. #mu ID sys1", Systematic::Type::ENVELOPE, "_muonIDeffSF_s1m0", "sim");
  AddSystematic("muonIDeffSF_sys2", "eff. #mu ID sys2", Systematic::Type::ENVELOPE, "_muonIDeffSF_s2m0", "sim");
  AddSystematic("muonIDeffSF_sys3", "eff. #mu ID sys3", Systematic::Type::ENVELOPE, "_muonIDeffSF_s3m0", "sim");
  AddSystematic("muonIDeffSF_sys4", "eff. #mu ID sys4", Systematic::Type::ENVELOPE, "_muonIDeffSF_s4m0", "sim");
  AddSystematic("muonIDeffSF_sys5", "eff. #mu ID sys5", Systematic::Type::ENVELOPE, "_muonIDeffSF_s5m0", "sim");
  AddSystematic("muonIDeffSF_sys6", "eff. #mu ID sys6", Systematic::Type::ENVELOPE, "_muonIDeffSF_s6m0", "sim");
  AddSystematic("muonIDeffSF_sys7", "eff. #mu ID sys7", Systematic::Type::ENVELOPE, "_muonIDeffSF_s7m0 _muonIDeffSF_s7m1", "sim");
  AddSystematic("muonIDeffSF_sys8", "eff. #mu ID sys8", Systematic::Type::ENVELOPE, "_muonIDeffSF_s8m0 _muonIDeffSF_s8m1", "sim");
  AddSystematic("muonIDeffSF_sys9", "eff. #mu ID sys9", Systematic::Type::ENVELOPE, "_muonIDeffSF_s9m0", "sim");
  AddSystematic("muonIDeffSF_sys10", "eff. #mu ID sys10", Systematic::Type::ENVELOPE, "_muonIDeffSF_s10m0", "sim");
  AddSystematic("muonIDeffSF_sys11", "eff. #mu ID sys11", Systematic::Type::ENVELOPE, "_muonIDeffSF_s11m0 _muonIDeffSF_s11m1", "sim");
  AddSystematic("muonIDeffSF_sys12", "eff. #mu ID sys12", Systematic::Type::ENVELOPE, "_muonIDeffSF_s12m0 _muonIDeffSF_s12m1", "sim");
  AddSystematic("muonIDeffSF_sys13", "eff. #mu ID sys13", Systematic::Type::ENVELOPE, "_muonIDeffSF_s13m0 _muonIDeffSF_s13m1", "sim");
  AddSystematic("muonIDeffSF_sys14", "eff. #mu ID sys14", Systematic::Type::ENVELOPE, "_muonIDeffSF_s14m0", "sim");
  AddSystematic("muonIDeffSF_sys15", "eff. #mu ID sys15", Systematic::Type::ENVELOPE, "_muonIDeffSF_s15m0", "sim");
  AddSystematic("muonIDeffSF_sys16", "eff. #mu ID sys16", Systematic::Type::ENVELOPE, "_muonIDeffSF_s16m0", "sim");
  AddSystematic("muonIDeffSF_sys17", "eff. #mu ID sys17", Systematic::Type::ENVELOPE, "_muonIDeffSF_s17m0", "sim");
  AddSystematic("muonIDeffSF", "eff. #mu ID syst.", Systematic::Type::MULTI, FormRange("muonIDeffSF_sys%d", Range(1, 18)), "sim");

  AddSystematic("muonTriggereffSF_sys1", "eff. #mu trig. sys1", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s1m0", "sim");
  AddSystematic("muonTriggereffSF_sys2", "eff. #mu trig. sys2", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s2m0", "sim");
  AddSystematic("muonTriggereffSF_sys3", "eff. #mu trig. sys3", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s3m0", "sim");
  AddSystematic("muonTriggereffSF_sys4", "eff. #mu trig. sys4", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s4m0", "sim");
  AddSystematic("muonTriggereffSF_sys5", "eff. #mu trig. sys5", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s5m0", "sim");
  AddSystematic("muonTriggereffSF_sys6", "eff. #mu trig. sys6", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s6m0", "sim");
  AddSystematic("muonTriggereffSF_sys7", "eff. #mu trig. sys7", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s7m0 _muonTriggereffSF_s7m1", "sim");
  AddSystematic("muonTriggereffSF_sys8", "eff. #mu trig. sys8", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s8m0 _muonTriggereffSF_s8m1", "sim");
  AddSystematic("muonTriggereffSF_sys9", "eff. #mu trig. sys9", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s9m0", "sim");
  AddSystematic("muonTriggereffSF_sys10", "eff. #mu trig. sys10", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s10m0", "sim");
  AddSystematic("muonTriggereffSF_sys11", "eff. #mu trig. sys11", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s11m0 _muonTriggereffSF_s11m1", "sim");
  AddSystematic("muonTriggereffSF_sys12", "eff. #mu trig. sys12", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s12m0 _muonTriggereffSF_s12m1", "sim");
  AddSystematic("muonTriggereffSF_sys13", "eff. #mu trig. sys13", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s13m0 _muonTriggereffSF_s13m1", "sim");
  AddSystematic("muonTriggereffSF_sys14", "eff. #mu trig. sys14", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s14m0", "sim");
  AddSystematic("muonTriggereffSF_sys15", "eff. #mu trig. sys15", Systematic::Type::ENVELOPE, "_muonTriggereffSF_s15m0", "sim");
  AddSystematic("muonTriggereffSF", "eff. #mu trig. syst.", Systematic::Type::MULTI, Join(" ", FormRange("muonTriggereffSF_sys%d", Range(1, 16))), "sim");

  AddSystematic("muoneffSF", "#mu efficiency", Systematic::Type::MULTI, "muonTrackingeffSF muonRECOeffSF muonIDeffSF muonTriggereffSF", "sim");

  AddSystematic("electronRECOeffSF_sys1", "eff. e RECO sys1", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s1m0", "sim");
  AddSystematic("electronRECOeffSF_sys2", "eff. e RECO sys2", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s2m0", "sim");
  AddSystematic("electronRECOeffSF_sys3", "eff. e RECO sys3", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s3m0", "sim");
  AddSystematic("electronRECOeffSF_sys4", "eff. e RECO sys4", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s4m0", "sim");
  AddSystematic("electronRECOeffSF_sys5", "eff. e RECO sys5", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s5m0", "sim");
  AddSystematic("electronRECOeffSF_sys6", "eff. e RECO sys6", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s6m0", "sim");
  AddSystematic("electronRECOeffSF_sys7", "eff. e RECO sys7", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s7m0 _electronRECOeffSF_s7m1", "sim");
  AddSystematic("electronRECOeffSF_sys8", "eff. e RECO sys8", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s8m0 _electronRECOeffSF_s8m1", "sim");
  AddSystematic("electronRECOeffSF_sys9", "eff. e RECO sys9", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s9m0", "sim");
  AddSystematic("electronRECOeffSF_sys10", "eff. e RECO sys10", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s10m0", "sim");
  AddSystematic("electronRECOeffSF_sys11", "eff. e RECO sys11", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s11m0 _electronRECOeffSF_s11m1", "sim");
  AddSystematic("electronRECOeffSF_sys12", "eff. e RECO sys12", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s12m0 _electronRECOeffSF_s12m1", "sim");
  AddSystematic("electronRECOeffSF_sys13", "eff. e RECO sys13", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s13m0 _electronRECOeffSF_s13m1", "sim");
  AddSystematic("electronRECOeffSF_sys14", "eff. e RECO sys14", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s14m0", "sim");
  AddSystematic("electronRECOeffSF_sys15", "eff. e RECO sys15", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s15m0", "sim");
  AddSystematic("electronRECOeffSF_sys16", "eff. e RECO sys16", Systematic::Type::ENVELOPE, "_electronRECOeffSF_s16m0", "sim");
  AddSystematic("electronRECOeffSF", "eff. e RECO syst.", Systematic::Type::MULTI, FormRange("electronRECOeffSF_sys%d", Range(1, 17)), "sim");

  AddSystematic("electronIDeffSF_sys1", "eff. e ID sys1", Systematic::Type::ENVELOPE, "_electronIDeffSF_s1m0", "sim");
  AddSystematic("electronIDeffSF_sys2", "eff. e ID sys2", Systematic::Type::ENVELOPE, "_electronIDeffSF_s2m0", "sim");
  AddSystematic("electronIDeffSF_sys3", "eff. e ID sys3", Systematic::Type::ENVELOPE, "_electronIDeffSF_s3m0", "sim");
  AddSystematic("electronIDeffSF_sys4", "eff. e ID sys4", Systematic::Type::ENVELOPE, "_electronIDeffSF_s4m0", "sim");
  AddSystematic("electronIDeffSF_sys5", "eff. e ID sys5", Systematic::Type::ENVELOPE, "_electronIDeffSF_s5m0", "sim");
  AddSystematic("electronIDeffSF_sys6", "eff. e ID sys6", Systematic::Type::ENVELOPE, "_electronIDeffSF_s6m0", "sim");
  AddSystematic("electronIDeffSF_sys7", "eff. e ID sys7", Systematic::Type::ENVELOPE, "_electronIDeffSF_s7m0 _electronIDeffSF_s7m1", "sim");
  AddSystematic("electronIDeffSF_sys8", "eff. e ID sys8", Systematic::Type::ENVELOPE, "_electronIDeffSF_s8m0 _electronIDeffSF_s8m1", "sim");
  AddSystematic("electronIDeffSF_sys9", "eff. e ID sys9", Systematic::Type::ENVELOPE, "_electronIDeffSF_s9m0", "sim");
  AddSystematic("electronIDeffSF_sys10", "eff. e ID sys10", Systematic::Type::ENVELOPE, "_electronIDeffSF_s10m0", "sim");
  AddSystematic("electronIDeffSF_sys11", "eff. e ID sys11", Systematic::Type::ENVELOPE, "_electronIDeffSF_s11m0 _electronIDeffSF_s11m1", "sim");
  AddSystematic("electronIDeffSF_sys12", "eff. e ID sys12", Systematic::Type::ENVELOPE, "_electronIDeffSF_s12m0 _electronIDeffSF_s12m1", "sim");
  AddSystematic("electronIDeffSF_sys13", "eff. e ID sys13", Systematic::Type::ENVELOPE, "_electronIDeffSF_s13m0 _electronIDeffSF_s13m1", "sim");
  AddSystematic("electronIDeffSF_sys14", "eff. e ID sys14", Systematic::Type::ENVELOPE, "_electronIDeffSF_s14m0", "sim");
  AddSystematic("electronIDeffSF_sys15", "eff. e ID sys15", Systematic::Type::ENVELOPE, "_electronIDeffSF_s15m0", "sim");
  AddSystematic("electronIDeffSF_sys16", "eff. e ID sys16", Systematic::Type::ENVELOPE, "_electronIDeffSF_s16m0", "sim");
  AddSystematic("electronIDeffSF_sys17", "eff. e ID sys17", Systematic::Type::ENVELOPE, "_electronIDeffSF_s17m0", "sim");
  AddSystematic("electronIDeffSF_sys18", "eff. e ID sys18", Systematic::Type::ENVELOPE, "_electronIDeffSF_s18m0", "sim");
  AddSystematic("electronIDeffSF", "eff. e ID syst.", Systematic::Type::MULTI, FormRange("electronIDeffSF_sys%d", Range(1, 19)), "sim");

  AddSystematic("electronTriggereffSF_sys1", "eff. e trig. sys1", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s1m0", "sim");
  AddSystematic("electronTriggereffSF_sys2", "eff. e trig. sys2", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s2m0", "sim");
  AddSystematic("electronTriggereffSF_sys3", "eff. e trig. sys3", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s3m0", "sim");
  AddSystematic("electronTriggereffSF_sys4", "eff. e trig. sys4", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s4m0", "sim");
  AddSystematic("electronTriggereffSF_sys5", "eff. e trig. sys5", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s5m0", "sim");
  AddSystematic("electronTriggereffSF_sys6", "eff. e trig. sys6", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s6m0", "sim");
  AddSystematic("electronTriggereffSF_sys7", "eff. e trig. sys7", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s7m0 _electronTriggereffSF_s7m1", "sim");
  AddSystematic("electronTriggereffSF_sys8", "eff. e trig. sys8", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s8m0 _electronTriggereffSF_s8m1", "sim");
  AddSystematic("electronTriggereffSF_sys9", "eff. e trig. sys9", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s9m0", "sim");
  AddSystematic("electronTriggereffSF_sys10", "eff. e trig. sys10", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s10m0", "sim");
  AddSystematic("electronTriggereffSF_sys11", "eff. e trig. sys11", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s11m0 _electronTriggereffSF_s11m1", "sim");
  AddSystematic("electronTriggereffSF_sys12", "eff. e trig. sys12", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s12m0 _electronTriggereffSF_s12m1", "sim");
  AddSystematic("electronTriggereffSF_sys13", "eff. e trig. sys13", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s13m0 _electronTriggereffSF_s13m1", "sim");
  AddSystematic("electronTriggereffSF_sys14", "eff. e trig. sys14", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s14m0", "sim");
  AddSystematic("electronTriggereffSF_sys15", "eff. e trig. sys15", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s15m0", "sim");
  AddSystematic("electronTriggereffSF_sys16", "eff. e trig. sys16", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s16m0", "sim");
  AddSystematic("electronTriggereffSF_sys17", "eff. e trig. sys17", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s17m0", "sim");
  AddSystematic("electronTriggereffSF_sys18", "eff. e trig. sys17", Systematic::Type::ENVELOPE, "_electronTriggereffSF_s18m0", "sim");
  AddSystematic("electronTriggereffSF", "eff. e trig. syst.", Systematic::Type::MULTI, Join(" ", FormRange("electronTriggereffSF_sys%d", Range(1, 19))), "sim");

  AddSystematic("electroneffSF", "e efficiency", Systematic::Type::MULTI, "electronRECOeffSF electronIDeffSF electronTriggereffSF", "sim");

  AddSystematic("effSF_stat", "eff. stat.", Systematic::Type::GAUSSIAN, FormRange("_lepeffeffSF_stat%d", Range(20)), "sim");

  AddSystematic("MuonMomentum_stat", "Roch. cor. stat.", Systematic::Type::GAUSSIAN, FormRange("_MuonMomentum_s1m%d", Range(40)));
  AddSystematic("MuonMomentum_sys2", "Roch. cor. sys2", Systematic::Type::ENVELOPE, "_MuonMomentum_s2m0");
  AddSystematic("MuonMomentum_sys3", "Roch. cor. sys3", Systematic::Type::ENVELOPE, "_MuonMomentum_s3m0");
  AddSystematic("MuonMomentum_sys4", "Roch. cor. sys4", Systematic::Type::ENVELOPE, "_MuonMomentum_s4m0");
  AddSystematic("MuonMomentum_sys5", "Roch. cor. sys5", Systematic::Type::ENVELOPE, "_MuonMomentum_s5m0");
  AddSystematic("MuonMomentum", "Roch. cor.", Systematic::Type::MULTI, "MuonMomentum_stat MuonMomentum_sys2 MuonMomentum_sys3 MuonMomentum_sys4 MuonMomentum_sys5");

  AddSystematic("ElectronEnergy_stat", "Roch. cor. stat.", Systematic::Type::GAUSSIAN, FormRange("_ElectronEnergy_s1m%d", Range(40)));
  AddSystematic("ElectronEnergy_sys2", "Roch. cor. sys2", Systematic::Type::ENVELOPE, "_ElectronEnergy_s2m0");
  AddSystematic("ElectronEnergy_sys3", "Roch. cor. sys3", Systematic::Type::ENVELOPE, "_ElectronEnergy_s3m0");
  AddSystematic("ElectronEnergy_sys4", "Roch. cor. sys4", Systematic::Type::ENVELOPE, "_ElectronEnergy_s4m0");
  AddSystematic("ElectronEnergy_sys5", "Roch. cor. sys5", Systematic::Type::ENVELOPE, "_ElectronEnergy_s5m0");
  AddSystematic("ElectronEnergy_sys6", "Roch. cor. sys6", Systematic::Type::ENVELOPE, "_ElectronEnergy_s6m0");
  AddSystematic("ElectronEnergy_sys7", "Roch. cor. sys7", Systematic::Type::ENVELOPE, "_ElectronEnergy_s7m0");
  AddSystematic("ElectronEnergy_sys8", "Roch. cor. sys8", Systematic::Type::ENVELOPE, "_ElectronEnergy_s8m0");
  AddSystematic("ElectronEnergy", "Roch. cor.", Systematic::Type::MULTI, "ElectronEnergy_stat ElectronEnergy_sys2 ElectronEnergy_sys3 ElectronEnergy_sys4 ElectronEnergy_sys5 ElectronEnergy_sys6 ElectronEnergy_sys7 ElectronEnergy_sys8");

  AddSystematic("lepsys", "leptons", Systematic::Type::MULTI, "muoneffSF electroneffSF effSF_stat MuonMomentum ElectronEnergy");
  AddSystematic("exp_lepsys", "experimental", Systematic::Type::MULTI, "exp lepsys");
  AddSystematic("total_lepsys", "total", Systematic::Type::MULTI, "exp_lepsys theo");
}
#endif
