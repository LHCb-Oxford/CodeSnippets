/*
 * rootLogin.C -> mixture of Akiba style with bits of 
 * other styles mixed in
 * usage is add line to rootrc
 * Rint.Logon:              $PATH_TO_LOGIN/rootLogin.C
 */

#include "TLatex.h"
#include "TROOT.h"
#include "TColor.h"
#include "TStyle.h"
#include "TPaveText.h"

int  chart1, chart2, chart3, chart4, chart6, chart8, chart12;
int  grey40, grey60;
int  sun3, sun4;
int  BlueGrey;
int  DarkPink; 
int  Azure; 
int  Purple; 
int  Violet;
int  Pink;
int  Teal;
int  LighterBlueGrey;
int  LightBlueGrey;
int  DarkGrey;
int  LightGrey;
int  CoolBlue;
int  AvocadoGreen;
int  LightBlue;
int  DarkGreen;
int  Yellow;
int  Orange;
int  DarkBlue;
static const int ANABlue=862;
static const int ANAGreen=417;
static const int ANAPurple=876;

void rootLogin(){
  ///
  gROOT->SetStyle("Plain"); //no grey background on plots
  gStyle->SetStatStyle(0); //to view data behind box too!
  gStyle->SetStatBorderSize(0);   // remove shade from stat box
  gStyle->SetStatH(0.2);  //x,y,h,w define size and position
  gStyle->SetStatW(0.225);
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.9);
  //  gStyle->SetPadGridY(1);
  //  gStyle->SetPadGridX(1);
  //  gStyle->SetFillStyle(1001);

  gStyle->SetStatBorderSize(0); 
  gStyle->SetStatFormat("4.2g");

  gStyle->SetTitleBorderSize(0);  // remove shade from title box
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111111);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetAxisColor(1, "X");  // remove shade from title box
  gStyle->SetAxisColor(1, "Y");  // remove shade from title box
  gStyle->SetTitleColor(1, "X");  // remove shade from title box
  gStyle->SetTitleColor(1, "Y");  // remove shade from title box
  gStyle->SetTitleOffset(1, "X");  //
  gStyle->SetTitleOffset(1.1, "Y");  //
  gStyle->SetLabelOffset(0.005,"X");  //
  gStyle->SetLabelOffset(0.005, "Y");  //
  gStyle->SetTitleX(0.25);
  gStyle->SetTitleW(0.75);
  gStyle->SetTitleH(.092);
  gStyle->SetTitleFont(133,"XYZ");
  gStyle->SetLabelFont(133,"XYZ");
  gStyle->SetLegendFont(133);

  gStyle->SetTitleTextColor(1); //make the thing transparant??
  gStyle->SetTitleStyle(0); //make the thing transparant??
  //and axis
  gStyle->SetLabelSize(.03,"X");
  gStyle->SetLabelSize(.03,"Y");
  gStyle->SetLabelSize(.03,"Z");
  // //now the hist itself
  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(2);
  gStyle->SetHistLineColor(1);
  //gStyle->SetHistFillColor(0);
  //  gStyle->SetFillStyle(0);


  const Int_t NRGBs = 5;
  const Int_t NCont = 64;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  // gROOT->ProcessLine(".x ~/lhcbstyle.C");
  //////////////////////////////////////////////////////////////////////////////////////////

  Int_t lhcbFont = 132;
  Double_t lhcbWidth = 2.00;

  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetPalette(55);
  // set the paper & margin sizes
  gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.06);
  gStyle->SetPadRightMargin(0.12); // increase for colz plots!!
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.14);
  // use large fonts
  gStyle->SetTextFont(lhcbFont);
  gStyle->SetTextSize(0.08);
  gStyle->SetLabelFont(lhcbFont,"x");
  gStyle->SetLabelFont(lhcbFont,"y");
  gStyle->SetLabelFont(lhcbFont,"z");
  gStyle->SetLabelSize(0.042,"x");
  gStyle->SetLabelSize(0.042,"y");
  gStyle->SetLabelSize(0.042,"z");
  gStyle->SetTitleFont(lhcbFont);
  gStyle->SetTitleSize(0.06,"x");
  gStyle->SetTitleSize(0.06,"y");
  gStyle->SetTitleSize(0.06,"z");
  // use bold lines and markers
  gStyle->SetLineWidth(lhcbWidth);
  gStyle->SetFrameLineWidth(2.0);
  gStyle->SetHistLineWidth(lhcbWidth*1.5);
  gStyle->SetFuncWidth(lhcbWidth*1.5);
  //gStyle->SetGridWidth(1.5);
  gStyle->SetGridStyle(1);
  gStyle->SetGridColor(kGray+1);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  //gStyle->SetMarkerStyle(15);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(0);

  gStyle->SetLegendBorderSize(0);
  // label offsets
  gStyle->SetLabelOffset(0.015);

  // by default, do not display histogram decorations:
  //gStyle->SetOptStat(0);  
  gStyle->SetOptStat(1110);  // show only nent, mean, rms
  gStyle->SetOptTitle(0);
  //gStyle->SetOptFit(0);
  gStyle->SetOptFit(1011); // show probability, parameters and errors

  // look of the statistics box:
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatFont(lhcbFont);
  gStyle->SetStatFontSize(0.04);
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.9);
  gStyle->SetStatW(0.25);
  gStyle->SetStatH(0.15);

  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameFillColor(0);
  //  gStyle->SetHistFillStyle(0);
  //  gStyle->SetFillStyle(0);
  gStyle->SetStatStyle(0); //to view data behind box too!
  gStyle->SetStatBorderSize(0);   // remove shade from stat box
  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  // histogram divisions: only 5 in x to avoid label overlaps
  gStyle->SetNdivisions(505,"x");
  gStyle->SetNdivisions(510,"y");

  TPaveText *lhcbName = new TPaveText(0.65,0.8,0.9,0.9,"BRNDC");
  lhcbName->SetFillColor(0);
  lhcbName->SetTextAlign(12);
  lhcbName->SetBorderSize(0);
  lhcbName->AddText("LHCb");

  TText *lhcbLabel = new TText();
  lhcbLabel->SetTextFont(lhcbFont);
  lhcbLabel->SetTextColor(1);
  lhcbLabel->SetTextSize(0.04);
  lhcbLabel->SetTextAlign(12);

  TLatex *lhcbLatex = new TLatex();
  lhcbLatex->SetTextFont(lhcbFont);
  lhcbLatex->SetTextColor(1);
  lhcbLatex->SetTextSize(0.04);
  lhcbLatex->SetTextAlign(12);

  ////////////////////////////////////////////////////////////////////////////////////////////////////// 





  TColor *svc = new TColor();

  BlueGrey = svc->GetColor(230, 230, 255);
  DarkPink = svc->GetColor(102,   0,  51);
  Pink     = svc->GetColor(204,   0, 153);
  Azure    = svc->GetColor(  0, 153, 255);
  Purple   = svc->GetColor(102,   0, 204);
  Violet   = svc->GetColor( 51,   0, 153);
  Teal     = svc->GetColor( 51, 153, 102);

  chart1   = svc->GetColor(  0,  69, 134);
  chart2   = svc->GetColor(255,  66,  14);
  chart3   = svc->GetColor(255, 211,  32);
  chart4   = svc->GetColor( 87, 157,  28);
  chart6   = svc->GetColor(131, 202, 255);
  chart8   = svc->GetColor(174, 207,   0);
  chart12  = svc->GetColor(  0, 132, 209);
  grey40   = svc->GetColor(153, 153, 153);
  grey60   = svc->GetColor(102, 102, 102);
  sun3     = svc->GetColor(153, 153, 204);
  sun4     = svc->GetColor(204, 204, 255);

  LighterBlueGrey = sun4; 
  LightBlueGrey = sun3; 
  DarkGrey = grey60;
  LightGrey = grey40;
  CoolBlue = chart12;
  AvocadoGreen = chart8;
  LightBlue = chart6;
  DarkGreen = chart4;
  Yellow = chart3;
  Orange = chart2;
  DarkBlue = chart1;


  gStyle->SetCanvasColor(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetHistFillColor(0);
  gStyle->SetMarkerSize(0.7);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerColor(1);
  //gStyle->SetOptStat(10);
  gStyle->SetOptTitle(0);
  gStyle->SetPadColor(0);
  //  gStyle->SetPadGridX(1);
  //  gStyle->SetPadGridY(1);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPalette(1);
  gStyle->SetStatColor(0);
  gStyle->SetTitleFillColor(0);
  //  gStyle->SetHistFillStyle(1001);
  gStyle->SetCanvasDefH(400);
  gStyle->SetCanvasDefW(500);

  gStyle->SetStatFontSize(0.06);
  gStyle->SetLabelSize(0.06,"XYZ");
  gStyle->SetTitleSize(0.06,"XYZ");

  gStyle->SetLabelFont(132,"XYZ"); 
  gStyle->SetTitleFont(132,"XYZ"); 
  gStyle->SetPalette(112);
  gStyle->SetOptStat(0);
  gStyle->SetLegendTextSize(0.06);
  gStyle->SetLineWidth(2);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetOptFit(kFALSE);
}

