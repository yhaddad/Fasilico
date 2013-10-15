#include "IsolationProcess.h"

// modified marlin libs
#include "ParticleFactory.h"
#include "TrackSmearer.h"
#include "ClusterSmearer.h"
#include "IsolatedPart.h"
// marlin includes
#include "marlin/ErrorOfSigma.h"
#include "marlin/FastMCParticleType.h" // not checked yet !!

//--- LCIO headers 
#include "IMPL/LCCollectionVec.h"   
#include "EVENT/ReconstructedParticle.h" 
#include "IMPL/ReconstructedParticleImpl.h" 
#include "UTIL/LCRelationNavigator.h"
#include <EVENT/MCParticle.h>

#include <HistBooker.h>
#include <iostream>
#include <cmath>
#include <TMath.h>

#include <CLHEP/Vector/LorentzVector.h>



using namespace lcio ;
using namespace marlin ;
using namespace std;
using namespace CLHEP;

//namespace marlin{
IsolationProcess aIsolationProcess ;


IsolationProcess::IsolationProcess() : Processor("IsolationProcess"),
				       _nRun(-1),
				       _nEvt(-1)
{
  
  // modify processor description
  _description = "IsolationProcess creates ReconstrcutedParticles" 
    "according to the resolution given in the steering file." ;
  // register steering parameters: name, description, class-variable, default value
  registerInputCollection( LCIO::MCPARTICLE,
			   "InputCollectionName" , 
			   "Name of the MCParticle input collection"  ,
			   _inputCollectionName ,
			   std::string("MCParticle") ) ;
  
  
  registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			    "RecoParticleCollectionName" , 
			    "Name of the ReconstructedParticles output collection"  ,
			    _recoParticleCollectionName ,
			    std::string("ReconstructedParticles") ) ;
  
  registerOutputCollection( LCIO::LCRELATION,
			    "MCTruthMappingCollectionName" , 
			    "Name of the MCTruthMapping output collection"  ,
			    _mcTruthCollectionName ,
			    std::string("MCTruthMapping") ) ;
  
  FloatVec defaultCut ;
  defaultCut.push_back( 0.00015 ) ;
  defaultCut.push_back( 0.6 ) ;
  defaultCut.push_back( 2.0 ) ;
  
  registerProcessorParameter( "cut vector",
			      "cut vector",
			      _cutVec ,
			      defaultCut );
  
  
}

  
void IsolationProcess::init() { 
  // usually a good idea to
  printParameters() ;
  
  _hbook = gHistBooker->global();
  _nRun = 0 ;
  _nEvt = 0 ;
  
}


void IsolationProcess::processRunHeader( LCRunHeader* run) { 
  _nRun++ ;
} 
  

void IsolationProcess::processEvent( LCEvent * evt ) { 
  
  std::cout << " ====== processEvent : "<< _nEvt <<" ==== " << std::endl;
  
  LCCollection* recCol = evt->getCollection( _recoParticleCollectionName ) ;
  LCCollection* relCol = evt->getCollection( _mcTruthCollectionName ) ;
  LCRelationNavigator relNav( relCol ) ;
  
    
  if( relCol == 0 ) {
    streamlog_out( WARNING ) << "IsolationProcess::check no collection found: MCTruthMapping" << std::endl ;
    return ;
  }

Isolation isolation(PFO_SHAPE);
isolation.setMonitoring(_hbook);
  
  isolation.setRelationCol (relCol);
  isolation.setCollection  (recCol);
  
  isolation.registerCuts   (_cutVec);
  
  
  //if(isolation.havePart("muon"))
  //std::cout << "process:: numberof elmnt == "<<isolation.getCollection("photon")->getNumberOfElements()<< std::endl;
  _hbook->book_h1("/isolation/N_muon:;n_{#mu}",11,-0.5,10.5)->Fill( isolation.getCollection("muon")->getNumberOfElements());
  _hbook->book_h1("/isolation/N_electron:;n_{e}",11,-0.5,10.5)->Fill( isolation.getCollection("electron")->getNumberOfElements());
  _hbook->book_h1("/isolation/N_gamma:;n_{#gamma}",11,-0.5,10.5)->Fill( isolation.getCollection("photon")->getNumberOfElements());
  
  
  if(isolation.havePart("muon"))
    evt->addCollection(isolation.getCollection("muon"),"muon");
  if(isolation.havePart("electron")) 
    evt->addCollection(isolation.getCollection("electron"),"electron");
  if(isolation.havePart("photon")) 
    evt->addCollection(isolation.getCollection("photon"),"photon");
  
  _nEvt ++ ;
}



void IsolationProcess::check( LCEvent * evt ) { 
  
}


void IsolationProcess::end(){
  streamlog_out( MESSAGE4 )  << "IsolationProcess::end()  " << name() 
			     << " processed " << _nEvt << " events in " << _nRun << " runs "
			     << std::endl ;
}
//}

