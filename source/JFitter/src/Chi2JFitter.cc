#include "Chi2JFitter.hh"

#include <algorithm>
#include <utils.h>

using namespace CLHEP;
namespace fasilico{
  //namespace JFitter{
  
  CLHEP::HepLorentzVector Chi2JFitter::createHepLorentzVector(EVENT::MCParticle* mcp)
  {
    CLHEP::HepLorentzVector hepVec; 
    hepVec.setPx(mcp->getMomentum()[0]);
    hepVec.setPy(mcp->getMomentum()[1]);
    hepVec.setPz(mcp->getMomentum()[2]);
    hepVec.setE (mcp->getEnergy());
    return hepVec;
  }
  
  CLHEP::HepLorentzVector Chi2JFitter::createHepLorentzVector(EVENT::ReconstructedParticle* rec)
  {
    CLHEP::HepLorentzVector hepVec; 
    hepVec.setPx(rec->getMomentum()[0]);
    hepVec.setPy(rec->getMomentum()[1]);
    hepVec.setPz(rec->getMomentum()[2]);
    hepVec.setE (rec->getEnergy());
    return hepVec;
  }
  
  void Chi2JFitter::setParameters(std::vector<double> parameters)
  {
    this->_parameters = parameters ;
    for( unsigned int i=0; i < parameters.size(); i++){
      if(_function_1d != 0 ) _function_1d->SetParameter(i, parameters.at(i) );
      if(_function_2d != 0 ) _function_2d->SetParameter(i, parameters.at(i) );
    }
  }
  
  double Chi2JFitter::likelihood(CLHEP::HepLorentzVector dijet){
    double m_rec = utils::recoilMass(dijet,250.);
    // probamility to mjj is a z 
    // double L1 =  TMath::BreitWigner(dijet.m(),91.187,5); // Z test
    double L1 =  TMath::Gaus(dijet.m(),91.187,5); // Z test
    // probability to m_rec to be not a z 
    double L2 = 1.0 -  TMath::Gaus(m_rec,91.187,5); // recoil mass test
    
    //std::cout << __func__ << "::mrec ==" << m_rec <<std::endl;
    //std::cout << __func__ << "::m    ==" << dijet.m() <<std::endl;
    //std::cout << __func__ << "::L1   ==" << L1 <<std::endl;
    //std::cout << __func__ << "::L2   ==" << L2 <<std::endl;
    
    // heavy side function 
    //double L3 = 0;
    //if( dijet.m() < m_rec) L3 = 1;
    //else L3 = 0;
    
    //
    return L1 * L2;
  }
  
  
  
  void Chi2JFitter::setJetCollection(std::vector<CLHEP::HepLorentzVector> collection)
  {
    //if(collection.size() < 2 ) return; 
    
    std::map<double, std::vector<int> > chi2map;
    std::vector<std::vector<int> > vec_com;
    double minChi2 = 9999999. ;
    std::map<double,int> _chi2index;
    int njets    = collection.size();
    JCombination combination(njets,2,0,true); // looking for dijets
    vec_com = combination.getConbinationVector();
    int counter = 0;
    //std::cout << "Chi2JFitter::test 0"<< std::endl;
    for (std::vector<std::vector<int> >::iterator row = vec_com.begin(); row != vec_com.end(); row++){
      
      HepLorentzVector p1 = collection[row->at(0)];
      HepLorentzVector p2 = collection[row->at(1)];
            
      HepLorentzVector pij = p1  +  p2;
      double _likelihood = -1.;
      _likelihood = this->likelihood(pij);
      double _loglikelihood = 1e6;
      
      //std::cout << __func__ <<"::pij.m() == "<< pij.m() << std::endl;
      //std::cout << __func__ <<"::likelihood == "<< _likelihood << std::endl;
      
      if(_likelihood >= 0){
	_loglikelihood = -2*TMath::Log(_likelihood);
      }
      //if(_function_1d != 0 ) chi2 = _function_1d->Eval(pij.m());
      //if(_function_2d != 0 ) chi2 = _function_2d-run>Eval(pij.m(),pij.m());
      std::vector<int> dijet_id;
      std::cout << "Chi2JFitter::pair (i,j) ==  ("
		<<  row->at(0) << "," 
		<<  row->at(1) << ")" 
		<<"--> "<< _loglikelihood <<std::endl;
      
      dijet_id.push_back(row->at(0));
      dijet_id.push_back(row->at(1));
      minChi2 = std::min(minChi2, _loglikelihood);
      
      //JTuple *dijets = (JTuple*)_JTupleArray->ConstructedAt(counter);
      
      JTuple *dijet = new JTuple();
      
      double m_rec = utils::recoilMass(pij,205.);
      
      dijet->setHepLorentzVector(pij);
      dijet->setRecoilMass(m_rec);
      dijet->setLikelihood(_likelihood);
      dijet->setChi2(_loglikelihood);
      dijet->setdR(p1.deltaR(p2));
      dijet->setIndex(counter);
      dijet->setTag(0);
      
      
      _dijets.push_back(dijet);
      
      _chi2index.insert(std::pair<double,int> (_loglikelihood,counter));
      chi2map.insert( std::pair<double,std::vector<int> >(_loglikelihood, dijet_id ));
      counter++;
    }
    
    _minChi2    =  minChi2;
    std::map<double, std::vector<int> >::iterator it = chi2map.find(minChi2);
    int best_index = _chi2index.find(minChi2)->second;
    _best_dijet =  it->second;
    _dijets[best_index]->setTag(1);
    //_hep_best_dijet = _dijets[best_index]->getHepLorentzVector();
    std::cout << "Chi2JFitter::minChi2(index, value) == ("
	      <<_dijets[best_index]->getIndex()
	      <<" , "
	      << _dijets[best_index]->getChi2()
	      << " )"
	      << " ---> true value == ("
	      << best_index
	      << " , "<< _minChi2 <<" )"
	      << std::endl;
  }
  
  //} //  namespace JFitter
}// namespace fasilico
