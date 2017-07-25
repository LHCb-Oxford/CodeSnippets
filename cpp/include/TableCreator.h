#ifndef TABLECREATOR_H
#define TABLECREATOR_H

#include <iostream>
#include <string>
#include <math.h>
#include <fstream>

class TableCreator { 
  private : 

  public : 
    enum RowType {
      left, right, nowe, now2e
    };
    enum Format {
      bold, italic, math 
    };
    enum Options {
      null, debug
    };

    struct MultiCol {
      std::string text;
      unsigned int nCols;
      char alignment;
      MultiCol( const std::string& iText, const unsigned int& inCols , char iAlignment = 'c' ) : 
        text(iText), nCols(inCols), alignment(iAlignment) {} 
    };
    struct FormattedText {
      std::string text; 
      Format format;
      FormattedText( const std::string& iText, const Format& iFormat ) : 
        text(iText), format(iFormat) {}
    };
    struct MultiRow {
      std::string text;
      unsigned int nCols;
      char alignment;
      MultiRow( const std::string& iText, const unsigned int& inCols , char iAlignment = 'c' ) : 
        text(iText), nCols(inCols), alignment(iAlignment) {} 
    };

    TableCreator() {};

    TableCreator( const std::vector<RowType>& tableConfig ) : 
      m_colLength(0),
      m_tableConfig(tableConfig)
       {
        for( auto& config : m_tableConfig ){
          if( config == RowType::left )  m_colLength++; 
          if( config == RowType::right ) m_colLength++;
          if( config == RowType::nowe )  m_colLength+=2;
          if( config == RowType::now2e )  m_colLength+=3;
        }
        setNSF(3);   
      }

    template <typename... TYPES>
      void add( const unsigned int& row, TYPES... otherObj ) ; 
    template <typename... TYPES>
      void push(  TYPES... otherObj ) ; 

    void addRow( const unsigned int& row, const unsigned int& col);

    void addElement( const std::string& element, const unsigned int& row, unsigned int& col ){
      resize( row );
      if( col >= m_elements[row].size() ) std::cout << "Error - trying to append " << col << " to table with " << m_elements[row].size() << " columns" << std::endl;
      m_elements[row][col] = element ; 
      col++;
    }
    void addElement( const double& element, const unsigned int& row, unsigned int& col ){
      resize(row);
      if( col >= m_perRow.size() ){
        std::cout << "Error - trying to append row " << col << " with NSF buffer " << m_perRow.size() << std::endl; 
      }
      if( col >= m_elements[row].size() ) std::cout << "Error - trying to append " << col << " to table with " << m_elements[row].size() << " columns" << std::endl;
      m_elements[row][col] = trunc( element, m_perRow[col] );
      col++;
    }
    
    void addElement( const std::vector<double>& element, const unsigned int& row, unsigned int& col ){
      resize(row);
      for(unsigned int c=0 ; c < element.size(); c++ ){
        if( col+c >= m_perRow.size() ){
          std::cout << "Error - trying to append row " << col+c << " with NSF buffer " << m_perRow.size() << std::endl; 
        }
        if( col+c >= m_elements[row].size() ) std::cout << "Error - trying to append " << col+c << " to table with " << m_elements[row].size() << " columns" << std::endl;
      m_elements[row][col+c] = trunc( element[c], m_perRow[col+c] );
      }
      col = col + element.size();
    }

    void addElement( const int& element, const unsigned int& row, unsigned int& col ){
      resize(row);
      if( col >= m_elements[row].size() ) std::cout << "Error - trying to append " << col << " to table with " << m_elements[row].size() << " columns" << std::endl;
      m_elements[row][col] = std::to_string(element);
      col++;
    }
    void addElement( const MultiCol& element, const unsigned int& row, unsigned int& col ){
      resize(row);
      if( col >= m_elements[row].size() ) std::cout << "Error - trying to append " << col << " to table with " << m_elements[row].size() << " columns" << std::endl;
      m_elements[row][col] = 
        "\\multicolumn{"+std::to_string(element.nCols) + "}{"+element.alignment+"}{" + element.text + "}"; 
      for( unsigned int i = 1;i<element.nCols;++i) m_elements[row][col+i] = "##SKIP##";
      // m_skipCounter[row] += element.nCols -1;
      col=col+element.nCols; 
    }
    void addElement( const MultiRow& element, const unsigned int& row, unsigned int& col ){
      resize(row);
      m_elements[row][col++] = 
        "\\multirow{"+std::to_string(element.nCols) + "}{*}{" + element.text + "}"; 
    }
    void addElement( const FormattedText& element, const unsigned int& row, unsigned int& col ){
      resize(row);
      if( element.format == bold ) m_elements[row][col++] = "\\textbf{"+element.text+"}";
      if( element.format == italic ) m_elements[row][col++] = "\\textit{"+element.text+"}";
      if( element.format == math ) m_elements[row][col++] = "$"+element.text+"$";
    }
    unsigned int nRows() const { return m_elements.size(); }
    void addDivider( const unsigned int& where ){
      m_dividers.push_back(where);
    }
    void write( const std::string& fname ) const {
      std::ofstream of; of.open(fname);
      print( of );
      of.close();
    }

    void print( std::ostream& stream = std::cout ) const {
      stream << "\\begin{tabular}{";
      for( auto& config : m_tableConfig ){
        if( config == RowType::left ) stream << "l " << std::endl;
        if( config == RowType::right ) stream << "r " << std::endl;
        if( config == RowType::nowe ){ 
          stream << ">{\\collectcell\\num}r<{\\endcollectcell}";
          stream << " @{${}\\pm{}$}";
          stream << " >{\\collectcell\\num}l<{\\endcollectcell}" << std::endl; 
        }
        if( config == RowType::now2e ){ 
          stream << ">{\\collectcell\\num}r<{\\endcollectcell}";
          stream << " @{${}\\pm{}$}";
          stream << " >{\\collectcell\\num}l<{\\endcollectcell}";
          stream << " @{${}\\pm{}$}";
          stream << " >{\\collectcell\\num}l<{\\endcollectcell}" << std::endl; 
        }
      }

      stream << "}" << std::endl;
      stream << "\\toprule" << std::endl; 
      static const std::string delim = " & " ;
      static const std::string endl  = "\\\\\n";
      std::vector<unsigned int> dividers = m_dividers ; /// copy 
      std::sort( dividers.begin(), dividers.end() );
      unsigned int dPos=0;
      for( unsigned int row = 0 ; row < m_elements.size(); ++row ){
        for( unsigned int col = 0 ; col < m_elements[row].size() ; ++col ){
          if( m_elements[row][col] == "##SKIP##" ) continue; 
          if( col != 0 ) stream << delim; 
          stream << m_elements[row][col];
        }
        stream << endl; 
        while( dPos < dividers.size() && dividers[dPos] == row ){
          stream << "\\midrule" << std::endl; 
          dPos++;
        }
      }
      stream << "\\bottomrule" << std::endl; 
      stream << "\\end{tabular}" << std::endl; 
    }
    void preview(const TableCreator::Options& options = TableCreator::Options::null ) const ; 
    void setNSF( const unsigned int& nsf ){
      m_perRow.clear();
      for( unsigned int i=0;i<m_colLength;++i) m_perRow.push_back( nsf ); 
    }
    void setNSF( const std::vector<unsigned int>& nsfs ){ m_perRow = nsfs ; }
  private :  
    unsigned int m_colLength; 
    std::vector<RowType> m_tableConfig; 
    std::vector<unsigned int> m_perRow;
    std::vector<std::vector<std::string>> m_elements; 
    std::vector<unsigned int> m_dividers; 

    std::string trunc(const double& number, const unsigned int& nsf ){
      double value = round( number * pow(10,nsf) ) / pow(10,nsf) ;
      char buffer[20];
      sprintf( buffer, ("%." + std::to_string(nsf)+ "f").c_str() , value );
      std::string returnValue(buffer);
      return returnValue;
    }

    void resize( const unsigned int& row ){
      if( row >= m_elements.size() ) 
        m_elements.resize( row+1, std::vector<std::string>(m_colLength,"") );
    }
    template <typename TYPE, typename... TYPES>
      void addRow( const unsigned int& row, const unsigned int& col, TYPE obj, TYPES... otherObj ) ; 
};


void TableCreator::preview(const TableCreator::Options& opt ) const {

  std::ofstream texFile;
  texFile.open("/tmp/tmpTable.tex");

  /// preamble //// 
  texFile << "\\documentclass[12pt,a4paper]{article}" << std::endl;

  texFile << "\\usepackage[a0paper,total={12in,12in}]{geometry}" << std::endl;

  texFile << "\\usepackage{ifthen}" << std::endl;
  texFile << "\\newboolean{pdflatex}" << std::endl;
  texFile << "\\setboolean{pdflatex}{true}" << std::endl;

  texFile << "\\usepackage[pdftex]{graphicx}" << std::endl;
  texFile << "\\pagenumbering{gobble}" << std::endl;

  texFile << "\\usepackage{booktabs,siunitx}" << std::endl; 
  texFile << "\\sisetup{separate-uncertainty=true}" << std::endl;
  texFile << "\\usepackage{collcell}" << std::endl;
  texFile << "\\usepackage{xspace}" << std::endl; 
  texFile << "\\usepackage{multirow}" << std::endl; 
  texFile << "\\newcolumntype{P}[1]{>{\\centering\\arraybackslash}p{#1}}" << std::endl;
  texFile << "\\newcolumntype{M}[1]{>{\\centering\\arraybackslash}m{#1}}" << std::endl; 

  texFile << "\\begin{document}" << std::endl;
  print( texFile ); 
  texFile << "\\end{document}" << std::endl;
  texFile.close();
  //bool debug = opt & TableCreator::Options::debug ; 

  system("/home/evansti/K3Pi/AmpGenTools/preview.sh");
  
}

void TableCreator::addRow(const unsigned int& row,const unsigned int& col ){}

template < typename TYPE, typename... TYPES>
void TableCreator::addRow( const unsigned int& row,const unsigned int& col, TYPE obj, TYPES... otherObj ){
  unsigned int next_col = col;
  addElement(obj,row,next_col);
  addRow( row, next_col, otherObj... );
}

template < typename... TYPES > 
void TableCreator::add( const unsigned int& row, TYPES... otherObj ){
  addRow(row,0,std::forward<TYPES>(otherObj)...);
}

template < typename... TYPES > 
void TableCreator::push( TYPES... otherObj ){
  addRow(nRows(),0,std::forward<TYPES>(otherObj)...);
}
#endif
