#ifndef STRING_UTILS
#define STRING_UTILS 1

/*
 *  Miscellaenous useful string utilities :
 *  string = replaceAll(input,toReplace,replaceWith) ->
 *    replaces all instances of toReplace with replaceWith in input
 *  int FNV1a_hash(name) -> 
 *    implements FNV1a hash function of a string 
 *  vectorFromFile(fname) -> 
 *    reads file and puts it into a vector of strings 
 *  split( string, char delimiter ) ->
 *    split string into substrings using character delimiter char 
 */

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

static std::string replaceAll( const std::string& input,
    const std::string& toReplace,
    const std::string& replaceWith){
  size_t pos = 0;
  std::string output=input;
  if( toReplace == replaceWith ){
    return input;
  }
  do{
    pos = output.find(toReplace);
    if( pos != std::string::npos ){
      output.replace(pos,toReplace.length(), replaceWith );
    }
  } while( pos != std::string::npos );
  return output;
};

static unsigned int FNV1a_hash(const std::string& m_name ) { //// implements FNV-1a hash function //// 
  unsigned int hash=2166136261;
  for( auto& c : m_name ){
    hash ^= c ;
    hash *= 16777619;
  }
  return hash;
}


static std::vector<std::string> vectorFromFile( const std::string& filename,
    const char ignoreLinesThatBeginWith='#'
    )
{
  std::vector<std::string> output;
  std::string tmp;
  std::ifstream inFile( filename.c_str() );
  while( inFile.good() )
  {
    std::getline( inFile, tmp );
    if( tmp.size() == 0 || tmp[0] == ignoreLinesThatBeginWith ) continue;
    output.push_back( tmp );
  }
  return output;
}

static std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::string item;
  std::stringstream ss(s);
  while (std::getline(ss, item, delim)) {
    if( item != " " && item != "" && item != "\n" && item != "\t")
      elems.push_back(item);
  }
  return elems;
}

#endif 
