#ifndef RecoParticleFactory_h
#define RecoParticleFactory_h 1

#include "lcio.h"
#include "EVENT/MCParticle.h"
#include "EVENT/ReconstructedParticle.h"
  
/** Interface for a factory class that creates a ReconstructedParticle
 *  from an MCParticle 
 *
 *  @author F. Gaede, DESY
 *  @version $Id: IRecoParticleFactory.h,v 1.2 2005-10-11 12:56:28 gaede Exp $ 
 */ 
  
class RecoParticleFactory {
  
 public:
    
  /** Virtual d'tor.*/
  virtual ~RecoParticleFactory() {} 

  /** The actual factory method that creates a new ReconstructedParticle
   *  for the given MCParticle. NULL if no ReconstructedParticle should be created
   *  due to detector acceptance.
   */ 
  virtual lcio::ReconstructedParticle* createReconstructedParticle( const lcio::MCParticle* mcp ) = 0 ;
    
} ;
  
#endif



