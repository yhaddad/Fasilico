/** CHI2JFitter.hh --- 		
 *
 * Copyright (C) 2013 Y. Haddad
 *
 * Author: Y. Haddad <haddad@llr.in2p3.fr>
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

#ifndef Chi2JFitter_hh
#define Chi2JFitter_hh 1

#include <vector>
#include <iostream>
#include <CLHEP/Vector/LorentzVector.h>

//#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h> 
#include <EVENT/MCParticle.h>

#include <EVENT/MCParticle.h>
#include <EVENT/LCIO.h>
#include <lcio.h>

#include <TF1.h>
#include <TF2.h>
#include "TObject.h"
#include "TClonesArray.h"
#include "TRefArray.h"
#include "TRef.h"
#include "TH1.h"
#include "TBits.h"
#include "TMath.h"
#include "TTree.h"


#include <JFitter.hh>
#include <JTuple.hh>
using namespace lcio;

namespace fasilico{
  //namespace JFitter{
  class Chi2JFitter : public JFitter {     
  public:
    /// @brief constrcutor
    Chi2JFitter(): JFitter(), 
		   _minChi2(0), 
		   _function_1d(0),
		   _function_2d(0)
    {
      //std::cout << " ================== " << std::endl;
      //std::cout << " === Chi2Fitter === " << std::endl;
      //std::cout << " ================== " << std::endl;
      //_JTupleArray = new TClonesArray("JTuple");

      //std::cout << " === test of get Tree ==== "<<std::endl;
      
    }
    /// @brief destructor
    virtual ~Chi2JFitter(){
      delete this-> _function_1d;
      delete this-> _function_2d;
    }
    
    
    /// Set the LCCollection from LCIO file containing the Jet collections
    /// @param EVENT::LCCollection
    virtual void setJetCollection(EVENT::LCCollection* collection){};
    virtual void setJetCollection(std::vector<CLHEP::HepLorentzVector> collection);
    /** set the formulat for the \f$ \chi^2 \f$ as default the \f$ \chi^2 \f$ is defined as :  
        \f[ 
	\chi^2 = (\frac{m_{jj} - {m_0}}{\sigma_0})
	\f]
    */
    /// @param std::string 
    virtual void setFormulatCriterion(std::string formulat = "pow((x-[0])/[1],2)",int nVariable=1)
    {
      /// energy range 0-1 TeV
      /// 
      if(nVariable == 0 ) return ;
      this->_formulat = formulat;
      if(nVariable == 1 ) {
	_function_1d = new TF1("_function_1d",formulat.c_str()); 
      }else if(nVariable == 2) {
	_function_2d = new TF2("_function_2d",formulat.c_str()); 
      }
    }
    virtual void setParameters(std::vector<double> parameters);
    /// @brief create HepLorentzVector fron ReconstructedParticle
    virtual CLHEP::HepLorentzVector createHepLorentzVector(EVENT::ReconstructedParticle* rec);
    /// @brief create HepLorentzVector fron MCParticle
    virtual CLHEP::HepLorentzVector createHepLorentzVector(EVENT::MCParticle* mcp);
    /// @brief return HepLorentzVector of the best dijet
    //virtual CLHEP::HepLorentzVector getBestDiJet();
    /// @brief getIndex of the best Dijet
    virtual std::vector<int> getBestDiJetIndex(){return _best_dijet;}
    /// @brief getThe Jtuple Array with all dijet combination
    //virtual TClonesArray*    getDijets(){return _JTupleArray;}
    virtual std::vector<JTuple*>  getDijets(){return _dijets;}
    /// @brief get \f$ \chi^2 \f$
    virtual double getMinChi2(){return _minChi2;};
    /// @brief likelihood function defined like
    /// \f$ L(m_{jj}, m_{jj}^{recoil}|m_{Z},\sigma_{Z}) = P(m_{jj}|m_{Z},\sigma_{Z})(1-P(m_{jj}|m_{Z},\sigma_{Z})) \f$
    virtual double likelihood(CLHEP::HepLorentzVector dijet);
  private:
    std::string _formulat;
    std::vector<double> _parameters;
    
    std::vector<int>    _best_dijet;
    double _minChi2;
    TF1*   _function_1d;
    TF2*   _function_2d;
    
    std::vector<JTuple*> _dijets;
    
    //TClonesArray *_JTupleArray;
    //TTree *_outputTree;
    std::map<std::string,int> _counters;
  };// Chi2JFitter
  //}// JFitter namespace 
}// namespace
#endif // Chi2JFitter_hh
