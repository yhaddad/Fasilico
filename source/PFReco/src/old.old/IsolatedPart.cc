#include "IsolatedPart.h"

IsolatedPart::IsolatedPart():_deltaRMax(0):_PtRationMax(0)
{
  this->partVect.clear();
  this->isolPartVec.clear();
}

IsolatedLepton::IsolatedLepton():_nIsolatedLepton(0)
{
  
}

bool IsolatedLepton::IsIsolated(const lcio::ReconstructedParticle *recp)
{
  
}

  

  



