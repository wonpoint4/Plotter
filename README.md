# Plotter
General purpose plotter using ROOT histogram

[1.Basic classes](#basic-classes)  
[2.Quick start using Plotter](#quick-start-using-plotter)  
[3.Make your plotter](#make-your-plotter)  
[4.Draw options](#draw-options)  
[5.Verbosity](#verbosity)  


## Basic Classes
### Style
### Sample
* Define a sample. It can be a file(FILE), a sample(SAMPLE), or a collection of samples(SUM,STACK)
* main variables
  * title: the title of this sample in plots
  * subs: a vector of sub-samples for Collection type or files for Sample type
  * styles: a plotting style for this sample. Index 0 for the nominal and others for the systematics.
### Plot
* Define a plot. It is passed as argument of `Plotter::GetHist` function
### Plotter
* Base class for plotters
* Draw or save a TCanvas with sample entries(`vector<Sample> Plotter::entries`).
  
## Quick start using Plotter
```c++
root [0] .L Plotter.cc
root [1] Plotter aa
root [2] aa.ScanFiles("/data6/Users/hsseo/SKFlatOutput/Run2Legacy_v4/EfficiencyValidation/2016/")
```
This will add ROOT files. You can check added files using `Plotter::PrintFiles` function.
```c++
root [3] aa.PrintFiles(true)
--------------------------
@Key: EfficiencyValidation_WZ_pythia 
  /data6/Users/hsseo/SKFlatOutput/Run2Legacy_v4/EfficiencyValidation/2016/EfficiencyValidation_WZ_pythia.root 2020-04-21 02:42:45
--------------------------
@Key: EfficiencyValidation_ZZ_pythia 
  /data6/Users/hsseo/SKFlatOutput/Run2Legacy_v4/EfficiencyValidation/2016/EfficiencyValidation_ZZ_pythia.root 2020-04-22 11:07:36
--------------------------
```
Next, setup entries.
```c++
root [4] aa.SetupEntries("EfficiencyValidation_ZZ_pythia EfficiencyValidation_WZ_pythia")
root [5] aa.PrintEntries(true)
--------------------------
+Title:EfficiencyValidation_ZZ_pythia Type:UNDEF  FC:0 LC:1 MC:1 DO:'e hist' 
  /data6/Users/hsseo/SKFlatOutput/Run2Legacy_v4/EfficiencyValidation/2016/EfficiencyValidation_ZZ_pythia.root 2020-04-22 11:07:36
--------------------------
+Title:EfficiencyValidation_WZ_pythia Type:UNDEF  FC:0 LC:2 MC:2 DO:'e hist' 
  /data6/Users/hsseo/SKFlatOutput/Run2Legacy_v4/EfficiencyValidation/2016/EfficiencyValidation_WZ_pythia.root 2020-04-21 02:42:45
--------------------------
```
For Draw or save a plot
```c++
root [6] aa.DrawPlot("ee2016/m80to100/dimass_MediumID_Q","norm")
root [7] aa.SavePlot("ee2016/m80to100/dimass_MediumID_Q","norm")
root [8] aa.SavePlot("ee2016/m80to100/dimass_MediumID_Q","norm save:myplotname.pdf")
```
For time-consuming jobs, You can use condor scheduler with *Plotter::SavePlotCondor*. It will submit a *SavePlot* job in background.
```c++
root [9] aa.SavePlotCondor("ee2016/m80to100/dimass_MediumID_Q","norm save:myplotname.pdf")
```

## Make your plotter
You can make your own plotter class inherited from `Plotter`. See 'EfficiencyPlotter.cc' as an example.

## Draw and Save options
You can pass a option to a specific pad using `PADINDEX:OPTION` syntax. ex) `1:logy`  
* absx - plot with absolute x-axis
* absX - rebin N-D histogram with absolute x-axis
* absY - rebin N-D histogram with absolute y-axis
* absZ - rebin N-D histogram with absolute z-axis
* blind[:FLOAT,FLOAT] - remove data points for blind analyses. Needs two floating numbers to define range.
* chi2 - calculate chi2 and add text on the plot [WIP: only works for 2 entries]
* chi2detail - make chi2 report plot in new canvas;
* pvalue - add p-value on the plot (should be used with chi2 option)
* logx - log scale x-axis
* logy - log scale y-axis
* lumi:FLOAT - sepcify integrated luminosity
* norm - Normalized
* noleg - Do not draw a legend
* preliminary - Draw "CMS preliminary working in progress" and lumi and era info
* noera - In the preliminary option, do not draw era info
* nolumi - In the preliminary option, do not draw lumi info
* nostat - set statistical errro to 0
* TLleg - top-left legend
* TMleg - top-middle legend
* BLleg - bottom-left legend
* BMleg - bottom-middle legend
* BRleg - bottom-right legend
* rebin:INT - Rebinning by merging INT bins.
* rebin:{bin0,bin1,...,binN} - Rebinning plotting axis with new bin boundary.
* rebinX:{bin0,bin1,...,binN} - Rebinning N-D histogram x-axis with new bin boundary.
* rebinY:{bin0,bin1,...,binN} - Rebinning N-D histogram y-axis with new bin boundary.
* rebinZ:{bin0,bin1,...,binN} - Rebinning N-D histogram z-axis with new bin boundary.
* replace:STRING->STRING[:tag] - Modify the histogram name. You can specify sample tag. You can also use regular expression.
* save:PATH - For *DrawPlot*, specify the path or file format (pdf or png)
* scale:FLOAT[:tag] - scale histogram
* suffix:STRING[:tag] - Add suffix to histogram name. Same with replace:$->STRING[:tag].
* shapesys - variations are normalized to nominal histogram so that we can inspect only the shape difference
* sysname:STRING - Draw with systematic uncertainties defined by AddSystematic.
* sysdetail - Decompose systematic uncertainty.
* sysleg - Draw legend for systematic uncertainties.
* text:FLOAT,FLOAT,STRING - Add text on the plot. Consider PADINDEX syntax for multi-pad plots.
* type:INT
    * 0:UNDEF
    * 1:Compare - Draw multiple histograms in entries
    * 2:Ratio - val/base_val, you can set base entry using 'base:INT' option. 
    * 3:Diff - val-base_val, you can set base entry using 'base:INT' option.
    * 4:Sig - Diff/err
    * 5:CompareAndRatio - 'Compare' plot in the upper pannel and Ratio' plot in the lower pannel (Default)
    * 6:CompareAndDiff - 'Compare' plot in the upper pannel and 'Diff' plot in the lower pannel
    * 7:CompareAndSig - 'Compare' plot in the upper pannel and 'Sig' plot in the lower pannel 
    * 8:DoubleRatio (Developing)
    * 9:Collection (Developing)
* xsquare: rebin with x^2 axis
* xmin:FLOAT,xmax:FLOAT - x-axis range
* ymin:FLOAT,ymax:FLOAT - y-axis range
* Xmin:FLOAT,Xmax:FLOAT - x-axis projection range
* Ymin:FLOAT,Ymax:FLOAT - y-axis projection range
* Zmin:FLOAT,Zmax:FLOAT - z-axis projection range
* Umin:FLOAT,Umax:FLOAT - u-axis projection range
* title:STRING - set title, you can use single quote (') if title has spaces
* title+,title++ - larger title
* xtitle:STRING - set xtitle, you can use single quote (') if title has spaces
* xtitle+,xtitle++ - larger xtitle
* xtitle-,xtitle-- - smaller xtitle
* ytitle:STRING - set ytitle, you can use single quote (') if title has spaces
* ytitle+,ytitle++ - larger ytitle
* ytitle-,ytitle-- - smaller ytitle
* widey - For ratio plot, set yaxis range from 0.5 to 1.5
* widewidey - For ratio plot, set yaxis range from 0.0 to 2.0
* widthweight - For variable bin size, divide bin contents by bin size to get proper shape
* finey - For ratio plot, set yaxis range from 0.9 to 1.1
* finefiney - For ratio plot, set yaxis range from 0.95 to 1.05
* movesys - Collect systematic uncertainty to one entity in ratio pane. Useful when there is correlation of systematic uncertainty between entities  
## Verbosity
You can set verbosity using `Verbosity` variable (QUIET, ERROR, WARNING, INFO, DEBUG, ALL)
```c++
root [7] Verbosity=VERBOSITY::ERROR
```




