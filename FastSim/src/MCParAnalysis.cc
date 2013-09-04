#include "MCParAnalysis.h"

// modified marlin libs
#include "ParticleFactory.h"
#include "TrackSmearer.h"
#include "ClusterSmearer.h"

// marlin includes
#include "marlin/ErrorOfSigma.h"
#include "marlin/FastMCParticleType.h" // not checked yet !!

//--- LCIO headers 
#include "IMPL/LCCollectionVec.h"   
#include "EVENT/ReconstructedParticle.h" 
#include "IMPL/ReconstructedParticleImpl.h" 
#include "UTIL/LCRelationNavigator.h"
#include <EVENT/MCParticle.h>

#include <JetCombin.h>
#include <iostream>
#include <cmath>

#include <CLHEP/Vector/LorentzVector.h>



using namespace lcio ;
using namespace marlin ;
using namespace std;
using namespace CLHEP;

//namespace marlin{

MCParAnalysis aMCParAnalysis ;


MCParAnalysis::MCParAnalysis() : Processor("MCParAnalysis"),
				 _factory(NULL),
				 _nRun(-1),
				 _nEvt(-1)
{
  
  // modify processor description
  _description = "MCParAnalysis creates ReconstrcutedParticles from MCParticles " 
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

  
  registerProcessorParameter( "MomentumCut" , 
			      "cut momenta (in [GeV])"  ,
			      _momentumCut ,
			      float( 0.001 ) ) ;

  
}
  
  
void MCParAnalysis::init() { 
    
  // usually a good idea to
  printParameters() ;
    
  _nRun = 0 ;
  _nEvt = 0 ;
  
  // histogram manager
  hbook = new HistBooker();
}


void MCParAnalysis::processRunHeader( LCRunHeader* run) { 
  _nRun++ ;
} 
  

void MCParAnalysis::processEvent( LCEvent * evt ) { 
  
  if(evt != NULL){
    try{
      LCCollection* recCol = evt->getCollection(_recoParticleCollectionName ) ;
      LCCollection* relCol = evt->getCollection( _mcTruthCollectionName ) ;
      LCRelationNavigator relNav( relCol ) ;
      
      // new collections 
      //LCCollection* col_electron = new LCCollection( LCIO::RECONSTRUCTEDPARTICLE ) ;
      //LCCollection* col_muon     = new LCCollection( LCIO::RECONSTRUCTEDPARTICLE ) ;
      //LCCollection* col_tau      = new LCCollection( LCIO::RECONSTRUCTEDPARTICLE ) ;
      //LCCollection* col_photon   = new LCCollection( LCIO::RECONSTRUCTEDPARTICLE ) ;
      LCCollectionVec* col_electron  = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE) ;
      //LCCollectionVec* col_muon      = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE) ;
      //LCCollectionVec* col_tau       = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE) ;
      //LCCollectionVec* col_photon    = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE) ;
      
      std::cout << "event ---> " << _nEvt << std::endl;
      
      if( relCol == 0 ) {
	streamlog_out( WARNING ) << "MCParAnalysis::check no collection found: MCTruthMapping" << std::endl ;
	return ;
      }
      
      std::vector<ReconstructedParticle*> vec_rec;
      std::vector<MCParticle*> vec_mcp;
  
      if( recCol != 0 ){
	int nRec = recCol->getNumberOfElements()  ;
	for(int i=0; i< nRec ; i++){
	  ReconstructedParticle* rec  = dynamic_cast<ReconstructedParticle*>(recCol->getElementAt(i));
	  const LCObjectVec&  mcps 	  = relNav.getRelatedToObjects(rec) ;
	  MCParticle* mcp = dynamic_cast<MCParticle*>(mcps[0]); // we have a 1-1 relation here 
	  //if(mcp->getPDG() == 11 ) 
	  col_electron->addElement(rec);
	  
	  vec_mcp.push_back(mcp);
	  vec_rec.push_back(rec);
	}
      }
  
  
      //for(int i=0; i< vec_rec.size() ; i++){
      //
      //	int  parType = vec_rec[i]->getType() ;
      //	int  parPDG  = vec_mcp[i]->getPDG();
      //
      //	switch(parType){
      //	case 1:
      //	  {// find isolated leptons 
      //	    //HepLorentzVector p_1( vec_rec[i]->getMomentum()[0],
      //	    //			  vec_rec[i]->getMomentum()[1],
      //	    //			  vec_rec[i]->getMomentum()[2],
      //	    //			  vec_rec[i]->getEnergy()   );
      //	    //int near = 0;
      //	    //
      //	    //for(int j=0; j< vec_rec.size() ; j++){
      //	    //  HepLorentzVector p_2( vec_rec[j]->getMomentum()[0],
      //	    //			    vec_rec[j]->getMomentum()[1],
      //	    //			    vec_rec[j]->getMomentum()[2],
      //	    //			    vec_rec[j]->getEnergy()   );
      //	    //  if( (p_1.deltaR(p_2) < 0.5)  && (p_2 != p_1))
      //	    //	near++;
      //	    //}
      //	    //if(near==0){
      //	    //  switch(int(fabs(parPDG))){
      //	    //  case 11: // electron
      //	    //	col_electron->addElement(vec_rec[i]);
      //	    //	break;
      //	    //  case 13: // muon
      //	    //	col_muon->addElement(vec_rec[i]);
      //	    //	break;
      //	    //  case 15: // tau
      //	    //	col_tau->addElement(vec_rec[i]);
      //	    //	break;
      //	    //  }
      //	    //}
      //	    //hbook->book_h1("/isolated/lepton/near:;n_{near}",21,-0.5,20)->Fill(near);
      //	  } break;
      //
      //	case 2:
      //	  {
      //	    // find isolated photon
      //	    //std::cout << "blah 1" << std::endl;
      //	    //HepLorentzVector p_1( vec_rec[i]->getMomentum()[0],
      //	    //			  vec_rec[i]->getMomentum()[1],
      //	    //			  vec_rec[i]->getMomentum()[2],
      //	    //			  vec_rec[i]->getEnergy()   );
      //	    //int near = 0;
      //	    //
      //	    //for(int j=0; j< vec_rec.size() ; j++){
      //	    //  HepLorentzVector p_2( vec_rec[j]->getMomentum()[0],
      //	    //			    vec_rec[j]->getMomentum()[1],
      //	    //			    vec_rec[j]->getMomentum()[2],
      //	    //			    vec_rec[j]->getEnergy()   );
      //	    //  if( (p_1.deltaR(p_2) < 0.7)  && (p_2 != p_1))
      //	    //	near++;
      //	    //}
      //	    //if(near==0) 
      //	    //  col_photon->addElement(vec_rec[i]);
      //	    //std::cout << "blah 2" << std::endl;
      //	  }break;
      //	}
      //}
      
      cout << "TEST: " << vec_rec.size() << endl;
      
      std::cout << "::: electron n == "<< col_electron->getNumberOfElements () 
		<< std::endl;
      //std::cout << "::: muon     n == "<< col_muon->getNumberOfElements () 
      //		<< std::endl;
      //std::cout << "::: tau      n == "<< col_tau->getNumberOfElements () 
      //		<< std::endl;
      //std::cout << "::: photon   n == "<< col_photon->getNumberOfElements () 
      //		<< std::endl;
      
      // col_electron->setDefault( true ) ;
      // col_muon->setDefault( true ) ;
      // col_tau->setDefault( true ) ;
      // col_photon->setDefault( true ) ;
  
      evt->addCollection( col_electron, "lamerde");
      //evt->addCollection( col_muon    , "muon");
      //evt->addCollection( col_tau     , "tau");
      //evt->addCollection( col_photon  , "photon");
  
      _nEvt ++ ;
      
    }catch(lcio::DataNotAvailableException err) {} // try 
  } // if(evt != NULL)
}



void MCParAnalysis::check( LCEvent * evt ) { 
  //std::cout<< "=== isolated particle  Analysis====" << std::endl;
  //LCCollection* electron_col = evt->getCollection("lamerde") ; // jets collection 
  //if( electron_col == 0 ) {
  //  streamlog_out( WARNING ) << "JetFinder::check no electron collection " << std::endl ;
  //  return ;
  //}
  //if( electron_col != 0 ){
  //  std::cout << "::: electron collection == " <<  electron_col->getNumberOfElements() << std::endl;
  //}
}


void MCParAnalysis::end(){ 
  
  hbook->write_histograms("./analysis.root");
  
  streamlog_out( MESSAGE4 )  << "MCParAnalysis::end()  " << name() 
			     << " processed " << _nEvt << " events in " << _nRun << " runs "
			     << std::endl ;
}
//}

