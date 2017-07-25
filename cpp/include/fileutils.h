
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "dirent.h"
#include <sys/stat.h>
#include <cxxabi.h>
#include <map>

bool isDir( const std::string& pathname ){
  struct stat sb;
  return stat(pathname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
}
size_t find_next_of( const std::string& str , 
    const std::vector<std::string>& possibleItems,
    const unsigned int& startPos = 0 ){
  auto min_pos = std::string::npos;
  for( auto& item : possibleItems ){
    size_t new_min_pos = str.find( item, startPos  );
    if( new_min_pos < min_pos ){
      min_pos = new_min_pos;
    }
  }
  return min_pos;
}

std::string expandGlobals( std::string path ){

  size_t pos;
  do {
    pos = path.find("$");
    if( pos == std::string::npos ) break;
    size_t end_pos = std::string::npos;
    std::string variable_name; 
    if( path[pos+1] == '{' ){
      end_pos = path.find("}", pos);
      variable_name = path.substr(pos+2, end_pos-pos-2);
      end_pos = end_pos+1;
    }
    else {
      end_pos = find_next_of( path, {".","/"}, pos );
      variable_name = path.substr(pos+1,end_pos-pos-1);
    }
    const char* global_var = getenv( variable_name.c_str() );
    if( global_var == 0 ){
      ERROR( "variable " << variable_name << " not found");
      break;
    }
    path = path.replace(pos, end_pos - pos+1, global_var );
  } while ( pos != std::string::npos );

  return path;
}

bool stringMatchesWildcard( const std::string& input, 
                            const std::string& wildcard_string, 
                            const char wildcard_character='*' ){
  auto pos = wildcard_string.find(wildcard_character);  /// TEST_foobar -> *_foobar
  if( wildcard_string == wildcard_character ){
    DEBUG("Returning true");
    return true; 
  }
  if( pos == std::string::npos ){
    DEBUG("Returning " << input << " = " << wildcard_string << " ?");
    return input == wildcard_string; 
  }
  if( pos == wildcard_string.size() - 1 ){
    DEBUG("Returning " << input << " contains " << wildcard_string );
    return input.find( wildcard_string.substr(0,wildcard_string.size()-1) ) == 0 ; 
  }
  else { 
    const std::string pattern1 = wildcard_string.substr(0,pos+1);
    const std::string pattern2 = wildcard_string.substr(pos+1);
    DEBUG( "Matching " << pattern1 << " to " << input );
    bool match1 = stringMatchesWildcard(input,pattern1,wildcard_character );
    if( ! match1 ) return false;
    auto pos2 = pattern2.find( wildcard_character );
    auto posInInputString = input.find( pattern2.substr(0,pos2) );
    if( posInInputString == std::string::npos ) return false;
    return stringMatchesWildcard( input.substr(posInInputString), pattern2, wildcard_character );  
  }
  return false; 
}


std::vector<std::string> getListOfFiles( const std::string& directory, std::string patternString="" ){

  std::string expanded_path = expandGlobals( directory );
  /// wild-card structure /// 

  std::vector<std::string> files;
  std::vector<std::string> top_paths; 

  auto wildcard_pos = expanded_path.find("*");
  if( wildcard_pos != std::string::npos ){
    auto end = expanded_path.find("/", wildcard_pos );
    auto begin = expanded_path.find_last_of("/",wildcard_pos);
    std::string sub_path = expanded_path.substr(0, begin );
    std::string matchingString = expanded_path.substr(begin+1,end-begin-1);
    auto theseFiles = getListOfFiles( sub_path, matchingString );  
    for( auto& file : theseFiles ){
      if( isDir( file) ) top_paths.push_back( file ) ; 
      else files.push_back( file );
    }
  }
  else top_paths.push_back( expanded_path ); 
  DIR *dir;
  struct dirent *ent;
  for( auto& top_path : top_paths ){
    if ((dir = opendir (top_path.c_str() )) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        //printf ("%s\n", ent->d_name);
        std::string name = ent->d_name ;
        if( name == ".." || name == "." ) continue ;
        if( patternString == "" || stringMatchesWildcard(name,patternString) ) 
          files.push_back( top_path +"/" + name ) ;
      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return files;
    }
  }
  return files;
}
