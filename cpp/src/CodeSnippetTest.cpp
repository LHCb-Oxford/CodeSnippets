#include "include/MsgService.h"
#include "include/rootutils.h"

void CodeSnippetTest(){


  TFile* f = TFile::Open("/data/lhcb/users/evansti/data/D02K3Pi_NEW.root");
  TTree* t = get<TTree>(f,"RS/DecayTree");
  Branch<TLorentzVector> K(t,"D0_DTF_K", {"_PX","_PY","_PZ","_E"});
  t->GetEntry(0);
  K.Print();
  
  auto files = getListOfFiles("$DATADIR/MC/");
  for( auto& f : files ) INFO( f );

  TH1D* hist = get<TH1D>(f,"RS/DecayTree"); /// should fail /// 
}
