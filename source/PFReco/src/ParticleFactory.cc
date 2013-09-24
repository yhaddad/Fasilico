#include "marlin/MarlinConfig.h" // defines MARLIN_CLHEP

#ifdef MARLIN_CLHEP  // only if CLHEP is available !

#include <cstdlib>
#include "ParticleFactory.h"
#include "IMPL/ReconstructedParticleImpl.h"

#if ! LCIO_PATCHVERSION_GE( 1,51,3 )
#define USE_CLHEP // to activate code from LCIO header <UTIL/LCFourVector.h>
#endif

#include "UTIL/LCFourVector.h"

using namespace lcio ;


ParticleFactory::ParticleFactory() : _momentumCut( 0.0000000001 )
{
  _smearingVec.resize( NUMBER_OF_FASTMCPARTICLETYPES ) ;
}

void ParticleFactory::setMomentumCut( double mCut ) {
  _momentumCut = mCut ;
}

//void ParticleFactory::setPtCut( double ptCut ) {
//  _ptCut = mCut ;
//}
//
//void ParticleFactory::setEtaCut( double ptCut ) {
//  _EtaCut = mCut ;
//}

void ParticleFactory::registerFourVectorSmearer( FourVectorSmearer* sm , 
						 FastMCParticleType type ) {
  _smearingVec[ type ] = sm ;
}


FastMCParticleType ParticleFactory::getParticleType( const lcio::MCParticle* mcp ) {
  FastMCParticleType type( UNKNOWN )  ;
  float charge =  mcp->getCharge()  ;
  
  if( charge > 1e-10 || charge < -1e-10  ){  
    type = CHARGED ;
  } else if(  mcp->getPDG() == 22 )  { // photon
    type = PHOTON ;
  } else if(  std::abs( mcp->getPDG() ) == 12 || std::abs( mcp->getPDG() ) == 14 ||
	      std::abs( mcp->getPDG() ) == 16 || std::abs( mcp->getPDG() ) == 18 )  { // neutrinos - 18 is tau-prime
    type = NEUTRINO ;
  } else {  // treat everything else neutral hadron  
    type = NEUTRAL_HADRON ;
  }
  return type ;
}

ReconstructedParticle* ParticleFactory::createReconstructedParticle( const MCParticle* mcp ) {
  // this is where we do the fast Monte Carlo ....
#ifdef LCIO_PATCHVERSION_GE  // has been defined in 1.4.1 which has a bug fix in  LCFourVector<T>
  LCFourVector<MCParticle>  mc4V( mcp ) ;
#else
  HepLorentzVector mc4V( mcp->getMomentum()[0], mcp->getMomentum()[1],
			 mcp->getMomentum()[2], mcp->getEnergy() )  ;
#endif
  FastMCParticleType type = getParticleType(mcp ) ;
  FourVectorSmearer* sm = _smearingVec[ type ] ;
  HepLorentzVector reco4v(0.,0.,0.,0.)  ;
  
  if( sm == 0 ){
    // if we don't have a smearer registered we don't reconstruct the particle, e.g for neutrinos
    return 0 ;
  }
  reco4v = sm->smearedFourVector( mc4V , mcp->getPDG() ) ;
  ReconstructedParticleImpl* rec = 0 ;
  if(  reco4v.vect().mag() >  _momentumCut  ) {  
    rec = new ReconstructedParticleImpl ;
    float p[3] ;
    p[0] = reco4v.px() ;
    p[1] = reco4v.py() ;
    p[2] = reco4v.pz() ;
    rec->setMomentum( p ) ;
    rec->setEnergy( reco4v.e() ) ;
    rec->setMass( reco4v.m() ) ;
    rec->setCharge( mcp->getCharge() ) ;
    
    float vtx[3] ;
    vtx[0] = mcp->getVertex()[0] ;
    vtx[1] = mcp->getVertex()[1] ;
    vtx[2] = mcp->getVertex()[2] ;
    rec->setReferencePoint( vtx ) ;
    rec->setType( type ) ;
  }
  return  rec ;
}
#endif // MARLIN_CLHEP

