#ifndef IsolationProcess_h
#define IsolationProcess_h 1

#include "marlin/Processor.h"
#include "RecoParticleFactory.h"
#include <HistBooker.h>
#include "lcio.h"
#include <string>



using namespace lcio ;
using namespace marlin;

class IsolationProcess : public Processor {
  
 public:
  /** Returns a new instance of the processor.*/
  virtual Processor*  newProcessor() { return new IsolationProcess ; }
  IsolationProcess() ;
  
        
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
  HistBooker *_hbook;
  bool _hbook_flag;
  /** cuts */
  FloatVec _cutVec;
  

  int _nRun ;
  int _nEvt ;
    
} ;
  
//} // end namespace 

#endif


