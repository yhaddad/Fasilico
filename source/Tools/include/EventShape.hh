/** EventShape.hh --- 		
 *
 * Copyright (C) 2013 Y. Haddad
 *
 * Author: Y. Haddad <hadddad@yacine>
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


#ifndef EventShape_hh
#define EventShape_hh 1

#include <iostream>
#include <CLHEP/Vector/LorentzVector.h>
#include <CLHEP/Vector/ThreeVector.h>
#include "TMath.h"
#include "TMatrix.h"
#include "TRandom.h"

using namespace CLHEP;

namespace Tools{
  
  class EventShape{
  public:
    EventShape();
    ~EventShape();
    
    void setPartList(std::vector<CLHEP::HepLorentzVector> e);
    // Input the particle 3(4)-vector list
    // e: 3-vector  TVector3       ..(px,py,pz) or
    //    4-vector  TLorentzVector ..(px,py,pz,E) 
    // Even input the TLorentzVector, we don't use Energy 
    
    void     setThMomPower(Double_t tp);
    Double_t getThMomPower();
    void     setFast(Int_t nf);
    Int_t    getFast();
    
    Hep3Vector thrustAxis();
    Hep3Vector majorAxis();
    Hep3Vector minorAxis();
    
    /// @brief thrust :: Corresponding thrust, major, and minor value.
    Hep3Vector thrust(); 
    
    double Acoplanarity();
    
    Double_t oblateness();
    
  private:	
    Double_t ulAngle(Double_t x, Double_t y);
    Double_t sign(Double_t a, Double_t b);
    void     ludbrb(TMatrix *mom, 
		    Double_t the, 
		    Double_t phi, 
		    Double_t bx , 
		    Double_t by ,
		    Double_t bz );
    
    Int_t iPow(Int_t man, Int_t exp);
    
    Double_t m_dSphMomPower; 
    // PARU(41): Power of momentum dependence in sphericity finder.
    
    Double_t m_dDeltaThPower;
    // PARU(42): Power of momentum dependence in thrust finder.	
    
    Int_t m_iFast; 
    // MSTU(44): # of initial fastest particles choosen to start search.
    
    Double_t m_dConv;
    // PARU(48): Convergence criteria for axis maximization.
    
    Int_t m_iGood;
    // MSTU(45): # different starting configurations that must
    // converge before axis is accepted as correct.	
    
    TMatrix m_dAxes;
    
    // data: results
    // m_dAxes[1] is the Thrust axis.
    // m_dAxes[2] is the Major axis.
    // m_dAxes[3] is the Minor axis.
    
    std::vector<CLHEP::HepLorentzVector> _myobjects;
    
    Hep3Vector m_ThrustAxis;
    Hep3Vector m_MajorAxis;
    Hep3Vector m_MinorAxis;
    Hep3Vector m_Thrust;
    
    TRandom m_random;
    
    Double_t m_dThrust[4];
    Double_t m_dOblateness;
    
    static Int_t m_maxpart;
  };
  
}
#endif // EventShape_hh
