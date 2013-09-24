#ifndef FourVectorSmearer_h
#define FourVectorSmearer_h 1

#include "marlin/MarlinConfig.h"

#ifdef MARLIN_CLHEP  // only if CLHEP is available !

#include "CLHEP/Vector/LorentzVector.h"

namespace CLHEP{}    // declare namespace CLHEP for backward compatibility
using namespace CLHEP ;

/** Interface for smearing of four vectors - based on CLHEP::HepLorentzVector
 *
 *  @author F. Gaede, DESY
 *  @version $Id: IFourVectorSmearer.h,v 1.3 2006-03-30 16:12:16 gaede Exp $ 
 */ 

class FourVectorSmearer {
  
 public:
  
  /** Virtual d'tor.*/
  virtual ~FourVectorSmearer() {} 
  
  /** Smears the given four vector 
   */ 
  virtual HepLorentzVector smearedFourVector( const HepLorentzVector& v, int pdgCode ) = 0 ;
  
} ;

#endif // MARLIN_CLHEP
#endif // IFourVectorSmearer_h

