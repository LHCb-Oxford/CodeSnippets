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
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TEventList.h"
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

/*
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
*/

template <class TYPE> 
std::map<std::string,TYPE*> getMap(TFile* f, const std::string& directory ) {

  TDirectoryFile* d = (TDirectoryFile*)f->Get(directory.c_str() );
  TList* keys = d == 0 ? f->GetListOfKeys() : d->GetListOfKeys();
  TObject* obj = keys->First();
  std::map<std::string, TYPE*> objects;
  do {
    TYPE* retrieved = dynamic_cast<TYPE*>(( (TKey*)obj)->ReadObj() ) ;
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

TEventList* getEventList( TTree* tree, const std::string& cut ){
  tree->Draw(">>evtList",cut.c_str());
  return (TEventList*)gDirectory->Get("evtList");
}

#endif
