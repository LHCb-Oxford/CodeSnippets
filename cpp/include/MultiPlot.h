#ifndef MULTIPLOT_H

#define MULTIPLOT_H 1 

class MultiPlot {

  public : 
    MultiPlot( const unsigned int& x, const unsigned int& y ) : 
      m_currentPad(0),
      m_nX(x),
      m_nY(y),
      m_index(0) {}

    void cd( const unsigned int& J ){
      if( m_currentPad != 0 ){
        m_currentPad->SaveAs( ("plot_"+std::to_string(m_index)+".pdf").c_str());
        delete m_currentPad; 
      }
      m_currentPad = new TCanvas("tmpPad","",500,400);
      gPad = m_currentPad; 
      m_index = J;  
    }
    void write(const std::string& fName) ; 
  private : 
    TPad* m_currentPad; 
    unsigned int m_nX;
    unsigned int m_nY;
    unsigned int m_index;
};




void MultiPlot::write( const std::string& name ){
  if( m_currentPad != 0 ){
    m_currentPad->SaveAs( ("plot_"+std::to_string(m_index)+".pdf").c_str());
    delete m_currentPad; 
  }
  std::ofstream texFile;
  texFile.open("/tmp/tmpPlot.tex");

  texFile << "\\documentclass[8pt,a4paper]{article}" << std::endl;

  texFile << "\\usepackage[a0paper,total={18in,28in}]{geometry}" << std::endl;

  texFile << "\\usepackage{ifthen}" << std::endl; 
  texFile << "\\newboolean{pdflatex}" << std::endl; 
  texFile << "\\setboolean{pdflatex}{true}" << std::endl; 

  texFile << "\\usepackage[pdftex]{graphicx}" << std::endl; 
  texFile << "\\pagenumbering{gobble}" << std::endl; 

  texFile << "\\begin{document}" << std::endl; 
  /*
     texFile << "\\includegraphics{s013_pipi_high}" << std::endl; 
     texFile << "\\includegraphics{s013_pipi_hpol}" << std::endl; 
     */
  texFile << "\\begin{tabular}{";
  for( unsigned int i = 0 ; i < m_nX ; ++i ){
    texFile << "l";
  }
  texFile << "}\n";
  for( unsigned int i = 1 ; i <= m_nX*m_nY; ++i ){
    texFile << "\\includegraphics{plot_"<<i<<".pdf}" << std::endl;
    if( i % m_nX != 0 ) 
      texFile << " & ";
    else texFile << "\\\\\n";
    // std::cout << i << " , " << xPos << " , " << xPos % i << std::endl; 
    // if( i != 0 && xPos % i == 0 ) texFile << "\n" << std::endl; 
  }
  texFile << "\\end{tabular}\n";
  texFile << "\\end{document}" << std::endl; 
  texFile.close();

  system("pdflatex /tmp/tmpPlot.tex > /dev/null");
  system("pdfcrop --margins '5 5 5 5' tmpPlot.pdf > /dev/null");
  system(("mv tmpPlot-crop.pdf " + name ).c_str() );

}
#endif 
