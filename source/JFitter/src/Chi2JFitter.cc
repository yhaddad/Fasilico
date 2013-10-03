#include "Chi2JFitter.hh"

#include <algorithm>

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
  
  void Chi2JFitter::setPrameters(std::vector<double> parameters)
  {
    this->_parameters = parameters ;
    for( unsigned int i=0; i < parameters.size(); i++){
      if(_function_1d != 0 ) _function_1d->SetParameter(i, parameters.at(i) );
      if(_function_2d != 0 ) _function_2d->SetParameter(i, parameters.at(i) );
    }
  }
  
  double Chi2JFitter::likelihood(CLHEP::HepLorentzVector dijet){
    double m_rec = sqrt( 250*250 + dijet.m()*dijet.m() - 2* dijet.e() * 250 );
    
    double L1 = TMath::Gaus(dijet.m(),91.187,2.4952); // Z test
    double L2 = 1.0 - TMath::Gaus(m_rec,91.187,2.4952); // recoil mass test
    
    return -2*TMath::Log(L1 * L2) ;
    
  }

  void Chi2JFitter::setJetCollection(EVENT::LCCollection* collection)
  {
    if(collection == 0) return; 
    
    std::string collectionType = collection->getTypeName();
    std::map<double, std::vector<int> > chi2map;
    std::vector<std::vector<int> > vec_com;
    double minChi2 = 9999999. ;
    
    if(collectionType == LCIO::RECONSTRUCTEDPARTICLE ){
      int njets    = collection->getNumberOfElements();
      JCombination combination(njets,2); // looking for dijets
      vec_com = combination.getConbinationVector();
      std::vector<std::vector<int> >::const_iterator row;
      for (row = vec_com.begin(); row != vec_com.end(); row++) {
	HepLorentzVector p1 = this->createHepLorentzVector(dynamic_cast<ReconstructedParticle*>(collection->getElementAt(row->at(0))));
	HepLorentzVector p2 = this->createHepLorentzVector(dynamic_cast<ReconstructedParticle*>(collection->getElementAt(row->at(1))));
	
	HepLorentzVector pij = p1  +  p2;
	double chi2 = -1.;
	chi2 = this->likelihood(pij);
	//if(_function_1d != 0 ) chi2 = _function_1d->Eval(pij.m());
	//if(_function_2d != 0 ) chi2 = _function_2d->Eval(pij.m(),pij.m());
	std::vector<int> dijet_id;
	std::cout << "pair (i,j) == ("
		  <<  row->at(0) <<"," 
		  <<  row->at(1) << ")" 
		  <<"--> "<< chi2 <<std::endl;
	
	dijet_id.push_back(row->at(0));
	dijet_id.push_back(row->at(1));
	minChi2 = std::min(minChi2, chi2);
	
	chi2map.insert( std::pair<double,std::vector<int> >(chi2, dijet_id ));
	
      }
    }else if(collectionType == LCIO::MCPARTICLE ){
      int npartons = collection->getNumberOfElements();
      JCombination combination(npartons,2); // looking for dijets
      vec_com = combination.getConbinationVector();
      std::vector<std::vector<int> >::const_iterator row;
      for (row = vec_com.begin(); row != vec_com.end(); row++) {
	HepLorentzVector p1 = this->createHepLorentzVector(dynamic_cast<ReconstructedParticle*>(collection->getElementAt(row->at(0))));
	HepLorentzVector p2 = this->createHepLorentzVector(dynamic_cast<ReconstructedParticle*>(collection->getElementAt(row->at(1))));
	
	HepLorentzVector pij = p1  +  p2;
	double chi2 = -1.;
	if(_function_1d != 0 ) chi2 = _function_1d->Eval(pij.m());
	//if(_function_2d != 0 ) chi2 = _function_2d->Eval(pij.m(),pij.m());
	std::vector<int> dijet_id;
	std::cout << "pair (i,j) == ("
		  <<  row->at(0) <<"," 
		  <<  row->at(1) << ")" 
		  <<"--> "<< chi2 <<std::endl;
	
	dijet_id.push_back(row->at(0));
	dijet_id.push_back(row->at(1));
	minChi2 = std::min(minChi2, chi2);
	
	chi2map.insert( std::pair<double,std::vector<int> >(chi2, dijet_id ));
	
      }
    }else{
      std::cerr << "ERROR:: the collection sould contain MCParticle of Reconstructedparticle" << std::endl;
      return;
    }
    _minChi2    =  minChi2;
    _best_dijet =  chi2map.find(minChi2)->second;
  }
    
  //} //  namespace JFitter
}// namespace fasilico
