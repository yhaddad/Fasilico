#ifndef FastMCProcessor_h
#define FastMCProcessor_h 1

#include "marlin/Processor.h"
#include "RecoParticleFactory.h"

#include "lcio.h"
#include <string>



using namespace lcio ;
using namespace marlin;

class FastMCProcessor : public Processor {
  
 public:
  
  /** Returns a new instance of the processor.*/
  virtual Processor*  newProcessor() { return new FastMCProcessor ; }
    
    
  FastMCProcessor() ;
    
        
  /** Initializes ...
   */
  virtual void init() ;
    
  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;
    
  /** Updates all registered conditions handlers and adds the data to the event.
   */
  virtual void processEvent( LCEvent * evt ) ; 
    
  /** Creates some checkplots.
   */
  virtual void check( LCEvent * evt ) ; 
    
  /** Called after data processing for clean up.
   */
  virtual void end() ;
    
    
 protected:
    
  /**  Input collection name */
  std::string _inputCollectionName ;

  /**  Ouput collection names */
  std::string _recoParticleCollectionName ;
  std::string _mcTruthCollectionName ;

  /** Momentum cut in GeV */
  float _momentumCut ;

  /** Resolutions of charged particles */
  FloatVec _initChargedRes ;

  /** Resolutions of photons */
  FloatVec _initPhotonRes ;

  /** Resolutions of photons */
  FloatVec _initNeutralHadronRes ;
  
  /** The particle factory */
  RecoParticleFactory* _factory ;
  
  /** fast sim check plots  */
  std::string _checkPlotsFile;
  
  //! ptCut 
  double _ptCut;
  
  //! ptCut 
  double _EtaCut;
  
  int _nRun ;
  int _nEvt ;
    
} ;
  
//} // end namespace 

#endif


