
/** IsolatedPart.h --- 
 *
 * Copyright (C) 2013 haddad
 *
 * Author: haddad <haddad.yacine@gmx.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
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

enum IsoAlgorithm {
  /* Relative tracker isolation :
   * \f$  I_{track} = \sum_{track} p_{t}/p_{candidate}\f$
   */
  TRACK_SHAPE = 0,
  /* Relative tracker isolation : 
   * \f$  I_{track} = \sum_{track} E_{calo}/p_{candidate}\f$
   *
   * \f$E_{calo} \f$Can be the HCAL, ECAL or \f$E_{HCAL}+E_{ECAL}\f$ energy 
   */
  ENERGY_SHAPE = 1,
  /* Relative tracker isolation : 
   * \f$  I_{track} = \sum_{track} (E_{calo} + p_{t})/p_{candidate}\f$
   *
   * \f$E_{calo} = E_{HCAL}+E_{ECAL}\f$
   */
  PFO_SHAPE = 2,
  ///! no algorithm 
  UNDEFINED_SHAPE = 99
};
  
class Isolation{
public:
  Isolation();
  Isolation(IsoAlgorithm algorithm);
  virtual ~Isolation() {};
  
  virtual void setCollection (LCCollection* col );
  virtual void setRelationCol(LCCollection* col );
  virtual void registerCuts(const std::vector<double>& cutVec );
  virtual void registerCuts(const std::vector<float>& cutVec );
  virtual LCCollection* getCollection(std::string name);
  virtual bool havePart(std::string partname);
  virtual void setMonitoring(HistBooker* hbook);
  virtual ReconstructedParticleImpl* duplicateReco(ReconstructedParticle* reco);
  virtual void setFlags(std::string flags);
private:
  // collection for each isolated type particle 
  
  //std::map<std::string, LCCollectionVec*> _colmap;
  std::map<std::string, LCCollection*> _colmap;
  LCRelationNavigator* _relNav;
  int _nIsolatedpart;
  double _deltaRMax;
  double _PtRatioMax;
  double _PtCut;
  bool _debug_flag;
  
  bool _hbook_flag;
  HistBooker *_hbook;

  IsoAlgorithm _algorithm;
};

#endif /* _ISOLATEDPART_H */

