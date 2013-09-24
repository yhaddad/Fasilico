#include "IsolatedPart.h"

Isolation::Isolation()
{
  this->_deltaRMax = 0;
  this->_PtRatioMax = 0;
  this->_colmap.clear();
  
  this->_colmap["electron"] = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  this->_colmap["photon"]   = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  this->_colmap["muon"]     = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  
  this->_hbook_flag = false; //  no histograming
  this->_hbook = 0;
}
void Isolation::registerCuts(const std::vector<double>& cutVec ){
  _deltaRMax  = cutVec[0];
  _PtRatioMax = cutVec[1];
  
  std::cout << "delta R  == " << _deltaRMax  << std::endl;
  std::cout << "Ratio Pt == " << _PtRatioMax << std::endl;
}

void Isolation::registerCuts(const std::vector<float>& cutVec ){
  _deltaRMax  = double(cutVec[0]);
  _PtRatioMax = double(cutVec[1]);
  
  std::cout << "delta R  == " << _deltaRMax  << std::endl;
  std::cout << "Ratio Pt == " << _PtRatioMax << std::endl;
}

void Isolation::setRelationCol(LCCollection *colrel)
{
  if( colrel == 0 ) {
    std::cout << "FastMCProcessor::check no collection found: MCTruthMapping" << std::endl ;
    return ;
  }
  this->_relNav = new LCRelationNavigator( colrel ) ;
}

void Isolation::setMonitoring(HistBooker *hbook)
{
  if( hbook != 0 ){
    _hbook = hbook;
    _hbook_flag = true;
  }else{
    std::cout<< "EROOR:: HistBooker not initialized ...!!"<< std::endl;
  }
}

void Isolation::setCollection(LCCollection* col)
{
  ReconstructedParticle* cand = 0;
  ReconstructedParticle* isol = 0;
  
  Double_t sumPT, ratio;
  Int_t counter;
  //Double_t rho = 0.0;
  
  try{
    for(int i=0; i< col->getNumberOfElements() ; i++){
      cand = dynamic_cast<ReconstructedParticle*>(col->getElementAt(i));
      if( cand != 0 ) {
	//this->partVect.push_pack(cand);
	HepLorentzVector p_cand( cand->getMomentum()[0],
				 cand->getMomentum()[1],
				 cand->getMomentum()[2],
				 cand->getEnergy()    );
	// loop over all input tracks
	sumPT = 0.0;
	counter = 0;
	for(int j=0; j < col->getNumberOfElements() ; j++){
	  isol = dynamic_cast<ReconstructedParticle*>(col->getElementAt(j));
	  HepLorentzVector p_isol( isol->getMomentum()[0],
				   isol->getMomentum()[1],
				   isol->getMomentum()[2],
				   isol->getEnergy()    );
	  if( isol == cand ) continue;// not count the similar particle
	  
	  
	  if( p_cand.deltaR(p_isol) <= 0.6)
	    {
	      
	      if( _hbook_flag ){
		this->_hbook->book_h1("/isolation/deltaR:;#delta R",200,0,10)->Fill(p_cand.deltaR(p_isol));
	      }
	      sumPT += p_isol.perp(); // sum of pt
	      ++counter;
	    }
	}
	
	//sumPT = sumPT - _deltaRMax*_deltaRMax*TMath::Pi();
	ratio = sumPT/p_cand.perp();
	if( _hbook_flag ){
	  //this->_hbook->boon_h1("");
	  this->_hbook->book_h1("/isolation/R_iso:;R^{l}_{iso}",200,0,0.1)->Fill(ratio);
	  this->_hbook->book_h1("/isolation/N_counter:;R^{l}_{iso}",11,-0.5,10.5)->Fill(counter);
	}
	
	if( ratio > 0.00015 || p_cand.perp() <= 2.0 /*Gev*/) continue;
	//if( counter == 1 ) continue;
	//-- look on the mc particle
	const LCObjectVec&  mcps = _relNav->getRelatedToObjects( cand ) ;
	MCParticle* mccand = dynamic_cast<MCParticle*>( mcps[0] ) ; // we have a 1-1 relation here 
	if(fabs(mccand->getPDG()) == 13)this->_hbook->book_h1("/isolation/Pt_muon:;P_{t}",500,0,100)->Fill(p_cand.perp());
	if(fabs(mccand->getPDG()) == 11)this->_hbook->book_h1("/isolation/Pt_electron:;P_{t}",500,0,100)->Fill(p_cand.perp());
	if(mccand->getPDG() == 22      )this->_hbook->book_h1("/isolation/Pt_photon:;P_{t}",500,0,100)->Fill(p_cand.perp());
	std::cout << "candidate pdg == "<< mccand->getPDG() << std::endl;
	
	if(mccand->getPDG() == 22      ) this->_colmap["photon"]   ->addElement( this->duplicateReco(cand) );
	if(fabs(mccand->getPDG()) == 11) this->_colmap["electron"] ->addElement( this->duplicateReco(cand) );
	if(fabs(mccand->getPDG()) == 13) this->_colmap["muon"]     ->addElement( this->duplicateReco(cand) );
      }
    }
  }catch(...){
    
  }
}

ReconstructedParticleImpl* Isolation::duplicateReco(ReconstructedParticle* reco)
{
  ReconstructedParticleImpl* rec = 0 ;
  if(reco != 0){
    rec = new ReconstructedParticleImpl ;
    float p[3] ;
    p[0] = reco->getMomentum()[0];
    p[1] = reco->getMomentum()[1];
    p[2] = reco->getMomentum()[2];
    rec->setMomentum( p ) ;
    rec->setEnergy( reco->getEnergy() ) ;
    rec->setMass( reco->getMass() ) ;
    rec->setCharge(reco->getCharge() ) ;
    
    float vtx[3] ;
    vtx[0] = reco->getReferencePoint()[0] ;
    vtx[1] = reco->getReferencePoint()[1] ;
    vtx[2] = reco->getReferencePoint()[2] ;
    rec->setReferencePoint( vtx ) ;
    rec->setType( reco->getType () ) ;
    
    return rec;
  }else
    return 0;
  
}
//LCCollectionVec* Isolation::getCollection(std::string name)
LCCollection* Isolation::getCollection(std::string name)
{
  // name : muon, electron, photon
  std::cout << "getCollection::name == " << name.c_str() << std::endl;
  return this->_colmap[name];
}

bool Isolation::havePart(std::string partname)
{
  if(this->_colmap[partname] == NULL){
    return false;
  }else{ 
    if(this->_colmap[partname]->getNumberOfElements() > 0)
      std::cout << "found :: "<< this->_colmap[partname]->getNumberOfElements() 
		<< "  in the col::" << partname.c_str()
		<< std::endl;
    return true;
  }
}

  

  



