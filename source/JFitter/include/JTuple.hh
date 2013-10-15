/** LHFitter.hh --- 		
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


#ifndef JTuple_hh
#define JTuple_hh 1

#include <TObject.h>
#if (!defined(__CINT__)) 
#include <CLHEP/Vector/LorentzVector.h>
#else
class HepLorentzVector;
#endif

class JTuple : public TObject
{
public:
  JTuple()
  {
    //std::cout << __func__ << "JTuple::Constructor" << std::endl;
  }
  
#if (!defined(__CINT__)) 
  JTuple( JTuple * jt){
    setHepLorentzVector(jt->getHepLorentzVector());
    setLikelihood      (jt->getLikelihood());
    setChi2            (jt->getChi2      ());
    setdR              (jt->getdR        ());
    setTag             (jt->getTag       ());
    setIndex           (jt->getIndex     ());
    setRecoilMass      (jt->getRecoilMass());
    setUserVariable    (jt->getRecoilMass());
    //std::cout << __func__ <<" mass == "<< jt->getHepLorentzVector().m() << std::endl;
    //std::cout << __func__ <<" chi2 == "<< jt->getChi2      () << std::endl;
  }
  void setHepLorentzVector(CLHEP::HepLorentzVector dijet){
    m         = dijet.m();
    //std::cout << __func__ << "JTuple::m == "<< m << std::endl;
    cosTheta  = dijet.cosTheta();
    E         = dijet.e();
    pt        = dijet.v().mag()/cosh(dijet.pseudoRapidity()); 
    p         = dijet.v().mag();
    px        = dijet.px();
    py        = dijet.py();
    pz        = dijet.pz();
    eta       = dijet.pseudoRapidity();
    mt        = dijet.mt();
    et        = dijet.et();
    plus      = dijet.plus();
    minus     = dijet.minus();
    theta     = dijet.theta();
    
    _dijet    = dijet;
  }
  
  CLHEP::HepLorentzVector getHepLorentzVector() {return _dijet;}
#endif
  double getLikelihood()const {return likelihood ;}
  double getChi2      ()const {return chi2       ;}
  double getdR        ()const {return dr         ;}
  int    getTag       ()const {return tag        ;}
  int    getIndex     ()const {return id         ;}
  double getRecoilMass()const {return mrecoil    ;}
  double getUserVariable()const{return user;}
  
  void setLikelihood(double _likelihood){likelihood = _likelihood;}
  void setChi2      (double _chi2      ){chi2 =_chi2;}
  void setdR        (double _dr        ){dr = _dr;}
  void setTag       (int    _tag       ){tag= _tag;}
  void setIndex     (int    _id        ){id = _id;}
  void setRecoilMass(double _mrecoil   ){mrecoil = _mrecoil;}
  void setUserVariable(double _user    ){user    = _user;}
  //void isBest       (bool   _is        ){isBest = _is};
protected:
  double m;
  double px;
  double py;
  double pz;
  double theta;
  double cosTheta;
  double E;
  double pt;
  double p;
  double mrec;
  double chi2;
  double likelihood;
  double dr;
  double eta;
  double et;
  double mt;
  double plus;
  double minus;
  double mrecoil;
  double user;
#if (!defined(__CINT__)) 
  CLHEP::HepLorentzVector _dijet;
#endif  
  int tag;
  int id;
  
  ClassDef(JTuple,2)
};
#endif // JTuple_hh
