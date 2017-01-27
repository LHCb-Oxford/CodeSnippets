#ifndef ROOTUTILS
#define ROOTUTILS 1

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "dirent.h"
#include <sys/stat.h>
#include "TFile.h"
#include <cxxabi.h>
#include "TKey.h"
#include "TObject.h"
#include <map>
#include "include/stringutils.h"
#include "include/MsgService.h"

/*
 *  rootutils : 
 *    vector<string> getListOfFiles(path) -> 
 *      lists directories in path, including global variable expansion and primative wildcard matching 
 *    get<TYPE>(TFile*, string name ) ->
 *      get name from TFile, checking against null and type conversions
 *    getMap<TYPE>(TFile*, string dir ) ->
 *      get all objects of type TYPE in dir dir in TFile
 *
 *    class Branch<TYPE>(TTree*, name) ->
 *      associate variable with tree, i.e. 
 *    Branch<double> D0_M( tree, "D0_M")
 *      then works as a double with tree->GetEntry()
 *      should work with copy and move constructors i.e. work in vector
 *     -> specialisations for 
 *     Branch<TVector3>(TTree*, name , branch_names ) expects name_PX, name_PY, name_PZ to be elements 
 *     Branch<TLorentzVector>(TTree*, name ,branch_names ) expects name_PX, name_PY, name_PZ, name_PE
 *
 *
 */

static std::vector<std::string> cmd( const std::string& command ){
  std::vector<std::string> output;
  FILE* proc = popen(command.c_str(), "r");
  char buf[4096];
  // Loop over the entries.
  while (!feof(proc) && fgets(buf, sizeof(buf), proc)) {
    output.push_back( buf );
  }
  pclose(proc);
  return output;
}

static std::string round(const double& number, const unsigned int& nsf ){
  double value = round( number * pow(10,nsf) ) / pow(10,nsf) ;
  char buffer[20];
  sprintf( buffer, ("%." + std::to_string(nsf)+ "f").c_str() , value );
  //  return std::to_string( value / pow(10,nsf) ) ;
  std::string returnValue(buffer);
  return returnValue;
}


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

bool stringMatchesWildcard( const std::string& input, const std::string& wildcard_string ){
  auto pos = wildcard_string.find("*");  /// TEST_foobar -> *_foobar
  if( pos == std::string::npos ){
    ERROR("Not wildcard like string : " << wildcard_string );
    return false; 
  } 
  if( pos == 0 ){
    return input.find( wildcard_string.substr(1) ) == input.size() - wildcard_string.size() + 1;
  }
  if( pos == wildcard_string.size() - 1 ){
    return input.find( wildcard_string.substr(0,wildcard_string.size()-1) ) == 0 ; 
  }
  else return stringMatchesWildcard(input, wildcard_string.substr(0,pos+1) )
    && stringMatchesWildcard(input, wildcard_string.substr(pos));
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

template <class TYPE> 
std::map<std::string,TYPE*> getMap(TFile* f, const std::string& directory ) {

  TDirectoryFile* d = (TDirectoryFile*)f->Get(directory.c_str() );
  TList* keys = d == 0 ? f->GetListOfKeys() : d->GetListOfKeys();
  TObject* obj = keys->First();
  std::map<std::string, TYPE*> objects;
  do {
    TYPE* retrieved = dynamic_cast<TH1D*>(( (TKey*)obj)->ReadObj() ) ;
    obj = keys->After( obj );
    if( retrieved != nullptr ) objects[ retrieved->GetName() ] = retrieved ; 
  } while( obj != 0 );
  return objects ; 
}

template < class TYPE > TYPE* get( TFile* file, const std::string& name ){
  TObject* t = (TObject*)file->Get( name.c_str() );
  if( t == 0 ){
    ERROR( name << " does not exist in FILE: " << file->GetName() );
    return nullptr;
  }
  TYPE* tTyped = dynamic_cast<TYPE*>(t);
  if( tTyped == 0){
    int status=0;

    ERROR( t->ClassName() << " cannot be casted to type " << abi::__cxa_demangle( typeid(TYPE).name(),0,0,&status ) );
    return nullptr;
  }
  return tTyped;
};

template <class TYPE> class Branch {
  public:
  Branch( TTree* tree, const std::string& name ) : m_tree(tree) , m_name(name) {
    tree->SetBranchStatus(name.c_str(),1);
    tree->SetBranchAddress(name.c_str(),&m_data);
  }
  operator TYPE() const { return m_data; } 
  Branch( Branch&& o ) : Branch( o.m_tree, o.m_name ) {};
  Branch( const Branch& o ) : Branch( o.m_tree, o.m_name ) {};

  TYPE m_data;
  std::string m_name; 
  TTree* m_tree;
};

template <> class Branch<TVector3> : public TVector3 {
  public:
  Branch( TTree* tree, const std::string& name , const std::vector<std::string>& branch_names = {"_PX","_PY","_PZ"} ) : m_tree(tree), m_name(name){ 
    for( unsigned int i = 0 ; i < 3 ; ++i ){
      tree->SetBranchStatus( (name+branch_names[i]).c_str(),1);
      tree->SetBranchAddress( (name+branch_names[i]).c_str(),&((*this)[i]));
    }
  }
  Branch( Branch&& o ) : Branch( o.m_tree, o.m_name ) {};
  Branch( const Branch& o ) : Branch( o.m_tree, o.m_name ) {};
  TTree* m_tree;
  std::string m_name; 
 // operator TVector3() const { return m_data; }
//  operator TVector3()       { return m_data; }
};

template <> class Branch<TLorentzVector> : public TLorentzVector {
  public:
  Branch( TTree* tree, const std::string& name , const std::vector<std::string>& branch_names = {"_PX","_PY","_PZ","_PE"}){
    
    for( unsigned int i = 0 ; i < 4 ; ++i ){
      tree->SetBranchStatus( (name+branch_names[i]).c_str(),1);
      tree->SetBranchAddress( (name+branch_names[i]).c_str(),&((*this)[i]));
    }
  }
  Branch( Branch&& o ) : Branch( o.m_tree, o.m_name ) {};
  Branch( const Branch& o ) : Branch( o.m_tree, o.m_name ) {};
  TTree* m_tree;
  std::string m_name; 
};

#endif
