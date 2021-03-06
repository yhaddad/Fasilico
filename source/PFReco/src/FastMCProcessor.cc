#include "FastMCProcessor.h"

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
HistBooker *book;  
FastMCProcessor aFastMCProcessor ;


FastMCProcessor::FastMCProcessor() : Processor("FastMCProcessor"),
				     _factory(NULL),
				     _nRun(-1),
				     _nEvt(-1)
{
  
  // modify processor description
  _description = "FastMCProcessor creates ReconstrcutedParticles from MCParticles " 
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
			      "No reconstructed particles are produced for smaller momenta (in [GeV])"  ,
			      _momentumCut ,
			      float( 0.001 ) ) ;

  registerProcessorParameter( "PtCut" , 
			      "No reconstructed particles are produced for smaller pt(in [GeV])"  ,
			      _ptCut ,
			      double( 2. ) ) ;
  
  
  registerProcessorParameter( "EtaCut" , 
			      "Fiducial cuts on Eta"  ,
			      _EtaCut ,
			      double( 2.66 ) ) ;

  registerProcessorParameter( "CheckPlots" , 
			      "Fast Simulation Check plots"  ,
			      _checkPlotsFile ,
			      std::string("FastSimCheckPlots.root"));
  
  FloatVec chResDefault ;
  chResDefault.push_back( 5e-5 ) ;
  chResDefault.push_back( 0.00 ) ;
  chResDefault.push_back( 3.141593/2. ) ;
    
  registerProcessorParameter( "ChargedResolution" , 
			      "Resolution of charged particles in polar angle range:  d(1/P)  th_min  th_max"  ,
			      _initChargedRes ,
			      chResDefault ,
			      chResDefault.size() ) ;
    
  FloatVec gammaResDefault ;
  gammaResDefault.push_back( 0.01 ) ;
  gammaResDefault.push_back( 0.10 ) ;
  gammaResDefault.push_back( 0.00 ) ;
  gammaResDefault.push_back( 3.141593/2. ) ;
    
  registerProcessorParameter( "PhotonResolution" , 
			      "Resolution dE/E=A+B/sqrt(E/GeV) of photons in polar angle range: A  B th_min  th_max"  ,
			      _initPhotonRes ,
			      gammaResDefault ,
			      gammaResDefault.size() ) ;
    
  FloatVec hadronResDefault ;
  hadronResDefault.push_back( 0.04 ) ;
  hadronResDefault.push_back( 0.50 ) ;
  hadronResDefault.push_back( 0.00 ) ;
  hadronResDefault.push_back( 3.141593/2. ) ;
  
  registerProcessorParameter( "NeutralHadronResolution" , 
			      "Resolution dE/E=A+B/sqrt(E/GeV) of neutral hadrons in polar angle range: A  B th_min  th_max"  ,
			      _initNeutralHadronRes ,
			      hadronResDefault ,
			      hadronResDefault.size() ) ;
  
  

}
  
  
void FastMCProcessor::init() { 
    
  // usually a good idea to
  printParameters() ;
    
  _nRun = 0 ;
  _nEvt = 0 ;
  
  _factory = 0 ;

#ifdef MARLIN_CLHEP
  
  ParticleFactory* simpleFactory  =  new ParticleFactory() ; 
  
  simpleFactory->registerFourVectorSmearer(  new TrackSmearer( _initChargedRes ), CHARGED ) ;
  simpleFactory->registerFourVectorSmearer(  new ClusterSmearer( _initPhotonRes ), PHOTON ) ;
  simpleFactory->registerFourVectorSmearer(  new ClusterSmearer( _initNeutralHadronRes ), NEUTRAL_HADRON ) ;
  simpleFactory->setMomentumCut( _momentumCut ) ;
  
  _factory = simpleFactory ;
  
  streamlog_out( MESSAGE )  << " FastMCProcessor::init() : registering ParticleFactory " << std::endl ;
  
#endif // MARLIN_CLHEP
  
  // histogram manager
  book = new HistBooker();
}


void FastMCProcessor::processRunHeader( LCRunHeader* run) { 
  _nRun++ ;
} 
  

void FastMCProcessor::processEvent( LCEvent * evt ) { 
  
  std::cout << " ====== processEvent : "<< _nEvt <<" ==== " << std::endl;
  
  //check(evt); // test 
  
  
  const LCCollection* mcpCol = evt->getCollection( _inputCollectionName ) ;
  LCCollectionVec* recVec    = new LCCollectionVec( LCIO::RECONSTRUCTEDPARTICLE ) ;
  LCRelationNavigator relNav( LCIO::RECONSTRUCTEDPARTICLE , LCIO::MCPARTICLE ) ;
  
  int lepton_count = 0;
  
  for(int i=0; i<mcpCol->getNumberOfElements() ; i++){
    MCParticle* mcp = dynamic_cast<MCParticle*> ( mcpCol->getElementAt( i ) ) ;
    if( mcp->getGeneratorStatus() == 1 ) { // stable particles only 
      ReconstructedParticle*  rec = 0 ;
      if( _factory != 0 ) {
    	rec = _factory->createReconstructedParticle( mcp ) ;
      }
      if( rec != 0 ) {
	HepLorentzVector cand(rec->getMomentum()[0],
			      rec->getMomentum()[1],
			      rec->getMomentum()[2],
			      rec->getEnergy()    );
	
	
	double eta = - TMath::Log( fabs(tan( cand.theta()/2. )));
	if( fabs(eta) < _EtaCut && cand.perp() > _ptCut ){
	  if( fabs(mcp->getPDG()) == 11 || fabs(mcp->getPDG()) == 13 || fabs(mcp->getPDG()) == 14) 
	    lepton_count++;
	  recVec->addElement( rec ) ;
	  relNav.addRelation( rec , mcp ) ;
	}
      }
    }
  }
  
  std::cout << "lepton number in FS == " << lepton_count << std::endl;
  
  recVec->setDefault( true ) ;
  
  evt->addCollection( recVec, _recoParticleCollectionName ) ;
  evt->addCollection( relNav.createLCCollection() , _mcTruthCollectionName ) ;
  _nEvt ++ ;
}



void FastMCProcessor::check( LCEvent * evt ) { 
  
  std::cout << "CHECK Process .... " << std::endl;
  std::cout << "test the combination code" << std::endl;
  
  LCCollection* recCol = evt->getCollection(_recoParticleCollectionName ) ;
  LCCollection* relCol = evt->getCollection( _mcTruthCollectionName ) ;
  LCRelationNavigator relNav( relCol ) ;
  
  if( relCol == 0 ) {
    streamlog_out( WARNING ) << "FastMCProcessor::check no collection found: MCTruthMapping" << std::endl ;
    return ;
  }
  
  if( recCol != 0 ){
    int nRec = recCol->getNumberOfElements()  ;
    for(int i=0; i< nRec ; i++){
      ReconstructedParticle* rec = dynamic_cast<ReconstructedParticle*>( recCol->getElementAt( i ) ) ;
      const LCObjectVec&  mcps = relNav.getRelatedToObjects( rec ) ;
      MCParticle* mcp = dynamic_cast<MCParticle*>( mcps[0] ) ; // we have a 1-1 relation here 
      
      if( rec->getType() == CHARGED) { 
  	double recP = std::sqrt( rec->getMomentum()[0] *  rec->getMomentum()[0] +
  				 rec->getMomentum()[1] *  rec->getMomentum()[1] +
  				 rec->getMomentum()[2] *  rec->getMomentum()[2] ) ;
  	double mcpP = std::sqrt( mcp->getMomentum()[0] *  mcp->getMomentum()[0] +
  				 mcp->getMomentum()[1] *  mcp->getMomentum()[1] +
  				 mcp->getMomentum()[2] *  mcp->getMomentum()[2] ) ;
  	
	book->book_h1("/photon/hChargedRes:;(P_{rec}-P_{mc})/P_{rec}",2000,-0.1,0.1)
	  ->Fill(( recP - mcpP ) / recP );
	book->book_h1("/photon/hChargedE:E_{charged hadron} [GeV]",2000,0,500)
	  ->Fill( rec->getEnergy() );
      } else {  // not charged
  	
  	double rE =  rec->getEnergy() ;
  	double mE =  mcp->getEnergy() ;
  	double dEoverE = ( rE - mE ) /  mE  ;
  	
  	if( rec->getType() == PHOTON) { 
	  book->book_h1("/photon/hPhotonE:E_{photon} [GeV]",2000,0,500)
	    ->Fill( rec->getEnergy() );
	  book->book_profile1("/photon/hGamHelper:;E_{photon} [GeV];(E_{r}- E_{m})/E_{m}",2000,0,500,0.,1.)
	    ->Fill( rec->getEnergy() , dEoverE );
	}
  	else if( rec->getType() == NEUTRAL_HADRON ) { 
	  book->book_h1("/photon/hHadronE:;E_{hadron} [GeV]",2000,0,500)
	    ->Fill( rec->getEnergy() );
	  book->book_profile1("/photon/hGamHelper:;E_{hadron} [GeV];(E_{r}- E_{m})/E_{m}",2000,0,500,0.,1.)
	    ->Fill( rec->getEnergy() , dEoverE );
	}
      }	  
    } 
  }
  
  


  
}


void FastMCProcessor::end(){ 
  
  book->write_histograms("./test.root");
  
  streamlog_out( MESSAGE4 )  << "FastMCProcessor::end()  " << name() 
			     << " processed " << _nEvt << " events in " << _nRun << " runs "
			     << std::endl ;
}
//}

