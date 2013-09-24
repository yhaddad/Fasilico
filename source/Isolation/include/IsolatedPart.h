/* @(#)IsolatedPart.h
 */

#ifndef _ISOLATEDPART_H
#define _ISOLATEDPART_H 1

#include "marlin/MarlinConfig.h"
#ifdef MARLIN_CLHEP
#include "marlin/FastMCParticleType.h"
#include <CLHEP/Vector/LorentzVector.h>
#endif
//--- LCIO headers 
#include <lcio.h>
#include "IMPL/LCCollectionVec.h"
#include "EVENT/LCCollection.h"
#include "EVENT/ReconstructedParticle.h" 
#include "IMPL/ReconstructedParticleImpl.h" 
#include "UTIL/LCRelationNavigator.h"
#include <EVENT/MCParticle.h>

//--- local and root dep
#include <HistBooker.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <TMath.h>

using namespace lcio ;
using namespace std;
using namespace CLHEP;

using namespace marlin;
class Isolation{
 public:
  Isolation();
  virtual ~Isolation() {};
  
  virtual void setCollection (LCCollection* col );
  virtual void setRelationCol(LCCollection* col );
  virtual void registerCuts(const std::vector<double>& cutVec );
  virtual void registerCuts(const std::vector<float>& cutVec );
  //virtual LCCollectionVec* getCollection(std::string name);
  virtual LCCollection* getCollection(std::string name);
  virtual bool havePart(std::string partname);
  virtual void setMonitoring(HistBooker* hbook);
  virtual ReconstructedParticleImpl* duplicateReco(ReconstructedParticle* reco);
 private:
  // collection for each isolated type particle 
  
  //std::map<std::string, LCCollectionVec*> _colmap;
  std::map<std::string, LCCollection*> _colmap;
  LCRelationNavigator* _relNav;
  int _nIsolatedpart;
  double _deltaRMax;
  double _PtRatioMax;

  bool _hbook_flag;
  HistBooker *_hbook;
};

#endif /* _ISOLATEDPART_H */

