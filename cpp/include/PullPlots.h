#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"

TH1D* calculate_pull_histogram( TH1* data, TH1* model ){

  TAxis* xaxis = data->GetXaxis();
  TH1D* pull = new TH1D("pull","", xaxis->GetNbins(), 
      xaxis->GetBinLowEdge( 1 ) , 
      xaxis->GetBinLowEdge( 1+xaxis->GetNbins() )  );
  pull->GetXaxis()->SetNoExponent( true );
  
  for( int i = 1 ; i <= data->GetXaxis()->GetNbins(); ++i){
    auto PDF = model->GetBinContent(i);
    auto val = data->GetBinContent(i);
    double sm = val == 0 ? 0 : ( val - PDF ) / sqrt(val) ;
    pull->SetBinContent( i , sm );
  }
  return pull; 
}

void pull_plot(TH1* data, TH1* model) {

  auto pull = calculate_pull_histogram(data,model);
  
  double p=0.35;
  
///  gPad = new TCanvas("c", "canvas", 500, 400*(1+p) );
  
  double scaleFactor=p/1.0;

  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.10, 1, p);
  
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.4);
  pad2->Draw();

  gPad->cd();          // Go back to the main canvas before defining pad2
  TPad *pad1 = new TPad("pad1", "pad1", 0, p, 1, 1.0);
  pad1->SetBottomMargin(0); // Upper and lower plot are joined
  pad1->SetGridx(0);         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad2->cd();

  pull->SetFillStyle(1001);
  pull->SetFillColor(kBlack);
  pull->Draw("HIST");       // Draw the ratio plot

  pull->SetLineColor(kWhite);
  pull->SetLineWidth(1);
  pull->GetYaxis()->SetRangeUser(-3.8,3.8);
  pull->GetYaxis()->SetTitle("#frac{N-N_{exp}}{#sqrt{N}}");
  pull->GetYaxis()->SetTitleOffset(0.3);
  pull->GetYaxis()->SetNdivisions(305);

  pull->GetXaxis()->SetTitle( data->GetXaxis()->GetTitle() );

  pull->GetYaxis()->SetLabelSize(  pull->GetYaxis()->GetLabelSize()  / scaleFactor );
  pull->GetXaxis()->SetLabelSize(  pull->GetXaxis()->GetLabelSize()  / scaleFactor );
  pull->GetYaxis()->SetTitleSize(  pull->GetYaxis()->GetTitleSize()  / scaleFactor );
  pull->GetXaxis()->SetTitleSize(  pull->GetXaxis()->GetTitleSize()  / scaleFactor );
  pull->GetXaxis()->SetTickLength( pull->GetXaxis()->GetTickLength() / scaleFactor );

  pad1->cd();
  data->Draw("E1");
  model->Draw("C HIST same");

  data->Draw("E1 same");
  gPad->RedrawAxis();
  gPad->cd();  
} 
