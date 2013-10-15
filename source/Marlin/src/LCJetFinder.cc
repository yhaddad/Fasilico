#include "LCJetFinder.h"
#include <iostream>
//#include <cmath>

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include "IMPL/LCCollectionVec.h"   
#include "EVENT/ReconstructedParticle.h" 
#include "IMPL/ReconstructedParticleImpl.h"
#include "UTIL/LCRelationNavigator.h"
#include <EVENT/MCParticle.h>

// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"
#include <TTree.h>
#include <TFile.h> 
#include <CLHEP/Vector/LorentzVector.h>
#include <Chi2JFitter.hh>
//#include <TMath.h>


#include "HistBooker.h"
#include "utils.h"
#include "JTuple.hh"

#define _print_ false

using namespace lcio ;

using namespace marlin ;
using namespace fastjet;
using namespace std;
using namespace CLHEP;

LCJetFinder aLCJetFinder ;
HistBooker *hbook;

LCJetFinder::LCJetFinder() : Processor("LCJetFinder"),_JTupleArray(0){
  // modify processor description
  _description = "LCJetFinder does whatever it does ..." ;
  
  _counters["event"]  = 0;
  _counters["ndijet"] = 0;
  _counters["njets"] = 0;
  // register steering parameters: name, description, class-variable, default value
  registerInputCollection( LCIO::MCPARTICLE,
			   "InputCollectionName" , 
			   "Name of the MCParticle input collection"  ,
			   _inputCollectionName ,
			   std::string("MCParticle") ) ;
  
  
  registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			    "RecoParticleCollectionName" , 
			    "Name of the ReconstructedParticles output collection"  ,
			    _recoParticleCollectionName ,
			    std::string("ReconstructedParticles") ) ;

  registerOutputCollection( LCIO::LCRELATION,
			    "MCTruthMappingCollectionName" , 
			    "Name of the MCTruthMapping output collection"  ,
			    _mcTruthCollectionName ,
			    std::string("MCTruthMapping") ) ;
  
  _force_cluster=false;
  registerProcessorParameter( "force_cluster"  , 
                              "force jet clutering to find njet"  ,
                              _force_cluster   ,
			      _force_cluster ) ;
  
  _njet_forced=2;
  registerProcessorParameter( "njet_forced"  , 
                              "number of jets ex: di-jets njet == 2 ..."  ,
                              _njet_forced   ,
			      _njet_forced ) ;
  
  _ycut=0.008;
  registerProcessorParameter( "ycut"  , 
                              "y cut for Durham jet clustering"  ,
                              _ycut   ,
			      _ycut ) ;
  
  _do_y_scan=true;
  registerProcessorParameter( "do_y_scan"  , 
                              "y scan for Durham jet clustering"  ,
                              _do_y_scan   ,
			      _do_y_scan ) ;
  
  //_histos="histos.root";
  //registerProcessorParameter( "histo" , 
  //			      "histo root" ,
  //			      _histos ,
  //			      _histos);
  
  _tree_file_name="dijet_tree.root";
  registerProcessorParameter("TreeOutputFile" , 
			     "ROOT tree" ,
			     _tree_file_name ,
			     _tree_file_name);
  
  _tree_name="dijet";
  registerProcessorParameter( "TreeName" , 
			      "The name of the ROOT tree" ,
			      _tree_name ,
			      _tree_name);
}



void LCJetFinder::init() { 
  
  streamlog_out(DEBUG) << "   init called  " << std::endl ;
  //hbook = new HistBooker();
  _hbook = gHistBooker->global();
  printParameters() ;
  
  //std::string _tree_file_name = "tree.root";
  //std::string _tree_name      = "dijet";
  _JTupleArray = new TClonesArray("JTuple");
    
  _tree_file=new TFile(_tree_file_name.c_str(),"RECREATE");
  if (!_tree_file->IsOpen()) {// test the fil opening 
    delete _tree_file;
    _tree_file=new TFile(_tree_file_name.c_str(),"NEW");
  }
  
  UInt_t bufsize=64000;
  
  _output_tree = new TTree(_tree_name.c_str(),_tree_name.c_str());
  _output_tree->SetAutoSave(32*1024*1024);
  _output_tree->Branch("event" , &_counters["event"] ,"event/I");
  _output_tree->Branch("ndijet", &_counters["ndijet"],"ndijet/I");
  _output_tree->Branch("njets" , &_counters["njets"],"njets/I");
  _output_tree->Branch("dijet" , &_JTupleArray,bufsize,99);
  
  //_bestdijet_tree = new TTree(std::string("best")+_tree_name.c_str(),std::string("best")+_tree_name.c_str());
  std::string new_name = "best_" + _tree_name;
  _bestdijet_tree = new TTree(new_name.c_str(),new_name.c_str());
  _bestdijet_tree->SetAutoSave(32*1024*1024);
  
  _bestdijet_tree->Branch("event"   ,&_roo_dijet.event         ,"event/I");
  _bestdijet_tree->Branch("njets"   ,&_roo_dijet.njet          ,"njets/I");
  _bestdijet_tree->Branch("m"       ,&_roo_dijet.m        ,"m/D");
  _bestdijet_tree->Branch("eta"     ,&_roo_dijet.eta      ,"eta/D");
  _bestdijet_tree->Branch("et"      ,&_roo_dijet.et       ,"et/D");
  _bestdijet_tree->Branch("mt"      ,&_roo_dijet.mt       ,"mt/D");
  _bestdijet_tree->Branch("plus"    ,&_roo_dijet.plus     ,"plus/D");
  _bestdijet_tree->Branch("minus"   ,&_roo_dijet.minus    ,"minus/D");
  _bestdijet_tree->Branch("px"      ,&_roo_dijet.px       ,"px/D");
  _bestdijet_tree->Branch("py"      ,&_roo_dijet.py       ,"py/D");
  _bestdijet_tree->Branch("pz"      ,&_roo_dijet.pz       ,"pz/D");
  _bestdijet_tree->Branch("E"       ,&_roo_dijet.E        ,"E/D");
  _bestdijet_tree->Branch("pt"      ,&_roo_dijet.pt       ,"pt/D");
  _bestdijet_tree->Branch("p"       ,&_roo_dijet.p        ,"p/D");
  _bestdijet_tree->Branch("mrecoil" ,&_roo_dijet.mrecoil  ,"mrecoil/D");
  _bestdijet_tree->Branch("dr"      ,&_roo_dijet.dr       ,"dr/D");
  _bestdijet_tree->Branch("chi2"    ,&_roo_dijet.chi2     ,"chi2/D");
  _bestdijet_tree->Branch("cosTheta",&_roo_dijet.cosTheta ,"cosTheta/D");
  _bestdijet_tree->Branch("theta"   ,&_roo_dijet.theta    ,"theta/D");
  _bestdijet_tree->Branch("likelihood"    ,&_roo_dijet.likelihood    ,"likelihood/D");
  //this->booktree(_bestdijet_tree,_roo_dijet);
  
  _nRun = 0 ;
  _nEvt = 0 ;
  
}

//virtual bookTree(){
//  _JTupleArray = new TClonesArray("dijet");
//  TFile *tree_file=new TFile("jets.root","RECREATE");
//  
//  if (!tree_file->IsOpen()) {
//    delete tree_file;
//    tree_file=new TFile("jets.root","NEW");
//  }
//  _outputTree = new TTree("Jtuple","JTuple");
//  _outputTree->SetAutoSave(32*1024*1024);
//  
//  _outputTree->Branch("event" , &_counters["event"] ,"event/I");
//  _outputTree->Branch("ndijet", &_counters["ndijet"],"ndijet/I");
//  _outputTree->Branch("dijet" , &_JTupleArray,bufsize,99);
//}

/// @brief Initialize the dijets tree
//virtual void write_tree()
//{
//  if (_outputTree) {
//    TFile *tree_file = _outputTree->GetCurrentFile(); 
//    tree_file->Write();
//    delete tree_file;
//  }
//  delete _JTupleArray;
//}

void LCJetFinder::processRunHeader( LCRunHeader* run) { 
  _nRun++ ;
}

class MyInfo: public PseudoJet::UserInfoBase {
public:
  MyInfo(int id) : _pdg_id(id){;}
  int pdg_id() const {return _pdg_id;}
  int _pdg_id;
};

void LCJetFinder::processEvent( LCEvent * evt ) { 
  
  LCCollection* recCol = evt->getCollection(_recoParticleCollectionName ) ;
  LCCollection* relCol = evt->getCollection( _mcTruthCollectionName ) ;
  LCRelationNavigator relNav( relCol ) ;
  
  std::vector<PseudoJet>  input_particles;
  std::vector<MCParticle> final_part;
  
  
  std::map<int,int> index_pid;
  if( recCol != NULL ){
    int nRec = recCol->getNumberOfElements();
    
    for(int i=0; i< nRec ; i++){
      ReconstructedParticle* rec = dynamic_cast<ReconstructedParticle*>( recCol->getElementAt( i ) ) ;
      const LCObjectVec&  mcps = relNav.getRelatedToObjects( rec ) ;
      MCParticle* mcp = dynamic_cast<MCParticle*>( mcps[0] ) ; // we have a 1-1 relation here 
      
      fastjet::PseudoJet in_part( rec->getMomentum()[0], // px
				  rec->getMomentum()[1], // py  
				  rec->getMomentum()[2], // pz
				  rec->getEnergy()       // E
				  );
      
      in_part.set_user_index(i);
      //MyInfo *info_pid = new MyInfo(mcp->getPDG());
      index_pid[i] = mcp->getPDG();
      //in_part.set_user_info( info_pid );  
      input_particles.push_back( in_part );
      
      _hbook->book_h1("/Part/E:Par Energy;E_{par} [GeV]",500,0,250) ->Fill(in_part.E()  );
      _hbook->book_h1("/Part/pt:Par p_{t};p_{t} [GeV]",500,0,250)->Fill(in_part.pt() );
      _hbook->book_h1("/Part/m:Jet Mass; m_{par} [GeV]",500,0,250) ->Fill(in_part.m()  );
      _hbook->book_h1("/Part/eta:;#eta",100,0,50)->Fill(in_part.eta());
      //std::cout <<" pdg --> "<< in_part.user_info<MyInfo>().pdg_id() << std::endl;
      
      //delete info_pid;
    } 
  }
  
  fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme;
  fastjet::Strategy strategy = fastjet::Best;
  
  fastjet::JetDefinition jet_def( fastjet::ee_kt_algorithm, recomb_scheme, strategy );
  fastjet::ClusterSequence clust_seq( input_particles, jet_def );
  
  //int n = 3;
  //vector<fastjet::PseudoJet> exclusive_jets = clust_seq.exclusive_jets(n);
  
  _hbook->book_h1("/Jet/y23:y_{23};y_{min}",500,0,20) ->Fill(-TMath::Log(clust_seq.exclusive_ymerge(2))); 
  _hbook->book_h1("/Jet/y34:y_{34};y_{min}",500,0,20) ->Fill(-TMath::Log(clust_seq.exclusive_ymerge(3))); 
  _hbook->book_h1("/Jet/y45:y_{45};y_{min}",500,0,20) ->Fill(-TMath::Log(clust_seq.exclusive_ymerge(4))); 
  _hbook->book_h1("/Jet/y56:y_{56};y_{min}",500,0,20) ->Fill(-TMath::Log(clust_seq.exclusive_ymerge(5))); 
  
  if(_do_y_scan){
    vector<vector< fastjet::PseudoJet> > vec_exclusive_jets;
    for(unsigned int i=0; i < 500; i++)
      vec_exclusive_jets.push_back(clust_seq.exclusive_jets_ycut (0.001*i));
    
    //vector<fastjet::PseudoJet> exclusive_jets;
    //exclusive_jets.push_back(clust_seq.exclusive_jets());
    
    // tell the user what was done
    //  - the description of the algorithm used
    //  - extract the inclusive jets with pt > 5 GeV
    //    show the output as 
    //      {index, rap, phi, pt, number of constituents}
    //----------------------------------------------------------
    
    
    
    for(unsigned int iscan=0; iscan < vec_exclusive_jets.size(); iscan++ ){
      double njet = vec_exclusive_jets[iscan].size(); 
      _hbook->book_h1(Form("/Jet/scan/ycut_%i/njet:Number of Jet;n_{jet}",iscan),21,-0.5,20.5)->Fill(njet);
      _hbook->book_profile1(Form("/Jet/scan/Njetscan:;y_{cut};<n_{jet}>",iscan),500,0,0.5,0,20)->Fill(0.001*iscan,njet);
      _hbook->book_profile1(Form("/Jet/scan/Qscan:;y_{cut};(njet_{find}-njet_{true})^{2}/njet_{true}^{2}",iscan),500,0,0.5,0,20)->Fill(0.001*iscan,(njet-2)*(njet-2)/4);
      
      // print out the details for each jet
      for (unsigned int i = 0; i < vec_exclusive_jets[iscan].size(); i++) {
	vector<fastjet::PseudoJet> constituents = vec_exclusive_jets[iscan].at(i).constituents();
	
	_hbook->book_h1(Form("/Jet/scan/ycut_%i/npart:;N_{particle}",iscan),51,-0.5,50.5)->Fill( constituents.size() );
	_hbook->book_h1(Form("/Jet/scan/ycut_%i/E:Jet Energy;E_{jet} [GeV]",iscan),500,0,250) ->Fill(vec_exclusive_jets[iscan].at(i).E()  );
	_hbook->book_h1(Form("/Jet/scan/ycut_%i/pt:Jet p_{t};p_{t} [GeV]",iscan),500,0,250)   ->Fill(vec_exclusive_jets[iscan].at(i).pt() );
	_hbook->book_h1(Form("/Jet/scan/ycut_%i/m:Jet Mass; m_{jet} [GeV]",iscan),500,0,250)  ->Fill(vec_exclusive_jets[iscan].at(i).m()  );
	_hbook->book_h1(Form("/Jet/scan/ycut_%i/d_to_beam:Distance to beam; d_{to beam}",iscan),500,0,250)->Fill(vec_exclusive_jets[iscan].at(i).beam_distance()  );
	_hbook->book_h1(Form("/Jet/scan/ycut_%i/eta:;#eta",iscan),100,0,50)->Fill(vec_exclusive_jets[iscan].at(i).eta());   
	_hbook->book_profile1(Form("/Jet/scan/ycut_%i/njet_mass:;n_{jet};m [GeV]",iscan),21,-0.5,20.5,0,250)->Fill(njet,vec_exclusive_jets[iscan].at(i).m());
	_hbook->book_profile1(Form("/Jet/scan/ycut_%i/mass_njet:;m [GeV];n_{jet}",iscan),500,0,250,0,20)->Fill(vec_exclusive_jets[iscan].at(i).m(),njet);
      }
    }
  }
  
  
  
  
  // optimal ycut
  //char *partype[3] = {"charged","neutral","photon"};
  _hbook->book_h1("/Jet/pid:;Jet_{pdg}",10001,-5000.5,5000.5);
  _hbook->get_h1("/Jet/pid")->SetBit(TH1::kCanRebin);
  
  LCCollectionVec* reco_jets    = new LCCollectionVec( LCIO::RECONSTRUCTEDPARTICLE ) ;
  //LCCollectionVec* reco_leptons = new LCCollectionVec( LCIO::RECONSTRUCTEDPARTICLE ) ;
  
  vector<fastjet::PseudoJet> exclusive_jets;
  if(_force_cluster){
    exclusive_jets = clust_seq.exclusive_jets(_njet_forced);
  }else{
    exclusive_jets = clust_seq.exclusive_jets_ycut (_ycut);
  }
  
  
  _hbook->book_h1("/Jet/Reco/njet:Number of Jet;n_{jet}",21,-0.5,20.5)->Fill(exclusive_jets.size());
  
  for (unsigned int i = 0; i < exclusive_jets.size(); i++) {
    ReconstructedParticleImpl* rec = new ReconstructedParticleImpl();
    double p[3] = { exclusive_jets[i].px() ,  
		    exclusive_jets[i].py() ,  
		    exclusive_jets[i].pz() };
    
    //int pid = exclusive_jets[i].user_info<MyInfo>().pdg_id();
    
    std::cout << "pid == " << index_pid[i] << std::endl;
    _hbook->get_h1("/Jet/pid")->Fill(index_pid[i]);
    rec->setMomentum (p);
    rec->setEnergy(exclusive_jets[i].E() );
    
    _hbook->book_h1("/Jet/Reco/npart:;N_{particle}",51,-0.5,50.5)     ->Fill(exclusive_jets[i].constituents().size() );
    
    for (unsigned int j = 0; j < exclusive_jets[i].constituents().size(); j++) {_hbook->get_h1("/Jet/pid")->Fill( index_pid[exclusive_jets[i].constituents()[j].user_index()] );}
    _hbook->book_h1("/Jet/Reco/E:Jet Energy;E_{jet} [GeV]",500,0,250) ->Fill(exclusive_jets[i].E()  );
    //_hbook->book_h1("/Jet/Reco/Area:Jet Area;Area_{jet} [GeV]",1000,0,250) ->Fill(exclusive_jets[i].area()  );
    _hbook->book_h1("/Jet/Reco/pt:Jet p_{t};p_{t} [GeV]",500,0,250)   ->Fill(exclusive_jets[i].pt() );
    _hbook->book_h1("/Jet/Reco/m:Jet Mass; m_{jet} [GeV]",500,0,250)  ->Fill(exclusive_jets[i].m()  );
    _hbook->book_h1("/Jet/Reco/d_to_beam:Distance to beam; d_{to beam}",500,0,250)->Fill(exclusive_jets[i].beam_distance()  );
    _hbook->book_h1("/Jet/Reco/eta:;#eta",100,0,50)->Fill(exclusive_jets[i].eta());   
    
    double max_pt = 0.;
    double min_pt = 9999.;
    for (unsigned int j = 0; j < exclusive_jets[i].constituents().size(); j++) {
      max_pt = max( max_pt,  exclusive_jets[i].constituents()[j].pt());
      min_pt = max( min_pt,  exclusive_jets[i].constituents()[j].pt());
    }
    _hbook->book_h1("/Jet/Reco/ptmax:Jet pmax_{t};pmax_{t} [GeV]",500,0,250)   ->Fill( max_pt );
    _hbook->book_h1("/Jet/Reco/ptmin:Jet pmin_{t};pmin_{t} [GeV]",500,0,250)   ->Fill( min_pt );
    
    //if( exclusive_jets[i].constituents().size()  > 4 ){
    reco_jets->addElement(rec);
    //}else{
    //reco_leptons->addElement(rec);
    //}
  }
  
  evt->addCollection( reco_jets   , "jets" ) ;
  //evt->addCollection( reco_leptons, "leptons" ) ;
  _nEvt ++ ;
}

void LCJetFinder::check( LCEvent * evt ) { 
  std::cout<< "=== DiJets Analysis====" << std::endl;
  LCCollection* jetCol = evt->getCollection("jets") ; // jets collection 
  if( jetCol == 0 ) {
    streamlog_out( WARNING ) << "JetFinder::check no Jets collection " << std::endl ;
    return ;
  }
  if( jetCol != 0 ){
    _JTupleArray->Clear("C");
    int nJet = jetCol->getNumberOfElements()  ;
    _counters["njets"]  = nJet;
    _counters["event"]  = evt->getEventNumber ();
    if(nJet < 2) return ;
    std::vector<HepLorentzVector> collection;
    fasilico::Chi2JFitter fitter;
    for(int i=0; i < nJet ; i++){
      collection.push_back(fitter.createHepLorentzVector(dynamic_cast<ReconstructedParticle*>(jetCol->getElementAt(i))));
    }
    
    fitter.setFormulatCriterion("pow((x-[0])/[1],2)");
    vector<double> par;
    par.push_back(91.187) ;    // mass  z
    par.push_back(2.4952*3.4) ;// sigma z
    fitter.setParameters(par);
    fitter.setJetCollection(collection);
    
    std::vector<JTuple*> com_dijets = fitter.getDijets();
    std::cout << "LCJetFinder::com_dijets.size=="<< com_dijets.size() <<std::endl;
    _counters["ndijets"] = com_dijets.size();
    for(int i=0; i < com_dijets.size(); i++){
      //JTuple *dijets = (JTuple*)_JTupleArray->ConstructedAt(counter);
      new((*_JTupleArray)[i]) JTuple(com_dijets[i]);
      //new(*_JTupleArray[i]) JTuple(); 
    }
        
    std::vector<int> z_dijet_id = fitter.getBestDiJetIndex();
    
    std::cout <<__func__<<"::best dijet index  == ( " 
	      << z_dijet_id[0] <<" , " 
	      << z_dijet_id[1] <<" ) " << std::endl;
    
    HepLorentzVector p1 = collection.at(z_dijet_id[0]);
    HepLorentzVector p2 = collection.at(z_dijet_id[1]);
    
    double pt_1 = p1.v().mag()/cosh(p1.pseudoRapidity());
    double pt_2 = p2.v().mag()/cosh(p2.pseudoRapidity());
    
    std::vector<CLHEP::HepLorentzVector> dijet_vector;
    fasilico::EventShape dijetshape;
    dijetshape.setPartList(dijet_vector);
    
    
    HepLorentzVector dijet = p1 + p2;
    
    std::cout << __func__ <<"::dijet.m() == " << dijet.m()<< std::endl;
    
    _roo_dijet.m          = dijet.m();
    std::cout << __func__ <<"::dijet.m() == " << _roo_dijet.m<< std::endl;
    _roo_dijet.cosTheta   = dijet.cosTheta();
    _roo_dijet.dr         = p1.deltaR(p2);
    _roo_dijet.E          = dijet.e();
    _roo_dijet.pt         = dijet.v().mag()/cosh(dijet.pseudoRapidity()); 
    _roo_dijet.p          = dijet.v().mag();
    _roo_dijet.px         = dijet.px();
    _roo_dijet.py         = dijet.py();
    _roo_dijet.pz         = dijet.pz();
    _roo_dijet.mrecoil    = utils::recoilMass(dijet, 250.);
    _roo_dijet.eta        = dijet.pseudoRapidity();
    _roo_dijet.mt         = dijet.mt();
    _roo_dijet.et         = dijet.et();
    _roo_dijet.plus       = dijet.plus();
    _roo_dijet.minus      = dijet.minus();
    _roo_dijet.theta      = dijet.theta();
    
    // event shape
    _roo_dijet.Acoplanarity      = dijetshape.Acoplanarity();
    _roo_dijet.oblateness      = dijetshape.oblateness();
    //
    _roo_dijet.likelihood =  fitter.likelihood(dijet);
    _roo_dijet.chi2       =  -2*TMath::Log(_roo_dijet.likelihood);
    _roo_dijet.event      =  _nEvt;
    _roo_dijet.njet       =  nJet;
    


    
    _bestdijet_tree->Fill();
    
    double delta_pt=sqrt((p1.v()[0]-p2.v()[0])*(p1.v()[0]-p2.v()[0])-(p1.v()[1]-p2.v()[1])*(p1.v()[1]-p2.v()[1]));
    
    _hbook->book_h1("/Dijet/delta_pt:; #delta p_{t}",400,-100,100)->Fill( pt_1-pt_2 );
    _hbook->book_h1("/Dijet/delta_p:; #delta p",400,-100,100)->Fill( p1.v().mag()-p2.v().mag() );
    _hbook->book_h1("/Dijet/delta_px:; #delta px",400,-100,100)->Fill( p1.v()[0]+p2.v()[0] );
    _hbook->book_h1("/Dijet/delta_py:; #delta py",400,-100,100)->Fill( p1.v()[1]+p2.v()[1] );
    
    _hbook->book_h1("/Dijet/delat_pt_2:; #delta |p1+p2|",200,0,100)->Fill( delta_pt );
        
    _hbook->book_h1("/Dijet/E_sqrts:;(E_{ij} - #sqrt{s}) [GeV]",1000,-250,250)->Fill( _roo_dijet.E - 250);
    _hbook->book_h1("/Dijet/chi2:; #chi^{2}",1000,0,250)->Fill(_roo_dijet.chi2);
    
    _hbook->book_h1("/Dijet/theta:;cos(#theta)",200,-1,1)->Fill(dijet.cosTheta());
    _hbook->book_h1("/Dijet/m:Jet Mass; m_{jj} [GeV]",1000,0,500)->Fill(_roo_dijet.m);
    _hbook->book_h1("/Dijet/pt:Jet P_{t}; Pt_{jj} [GeV]",1000,0,500)->Fill(_roo_dijet.pt);
    _hbook->book_h1("/Dijet/mreco:recoil mass; m_{recoil} [GeV]",1000,0,500)->Fill(_roo_dijet.mrecoil);
    _hbook->book_h1("/Dijet/deltaR:;#Delta R",200,0,20)->Fill(_roo_dijet.dr);
    _hbook->book_h1("/Dijet/energy:;E_{jj} [GeV]",1000,0,500)->Fill(_roo_dijet.E);
    _hbook->book_h1("/Dijet/p:;|p_{jj}| [GeV]",1000,0,500)->Fill(_roo_dijet.p);
    
    // correlation 
    _hbook->book_h2("/Dijet/energy_p:;E_{jj} [GeV];|p_{jj}| [GeV]",1000,0,500
    		   ,1000,0,500)->Fill(_roo_dijet.E,_roo_dijet.p);
    
    _hbook->book_h2("/Dijet/costheta_m:;cos(#theta_{ij});m_{jj} [GeV]",200,-1,1
    		   ,1000,0,500)->Fill(_roo_dijet.cosTheta,_roo_dijet.m);
    
    _hbook->book_h2("/Dijet/mreco_m:;m_{recoil};m_{jj} [GeV]",1000,0,500
    		   ,1000,0,500)->Fill(_roo_dijet.mrecoil,_roo_dijet.m);
    
    _hbook->book_h2("/Dijet/chi2_pt:;#chi^{2};pt_{jj} [GeV]",1000,0,500
    		   ,1000,0,500)->Fill(_roo_dijet.chi2,_roo_dijet.pt);
    
    _output_tree->Fill();
    
  }
  
}

void LCJetFinder::Zfinder( LCCollection* jetCol ) {  
  //if( jetCol == 0 ) {
  //  streamlog_out( WARNING ) << "JetFinder::check no Jets collection " << std::endl ;
  //  return ;
  //}
  //if( jetCol != 0 ){
  //  
  //  std::vector<HepLorentzVector > vec_jets;
  //  
  //  int nJet = jetCol->getNumberOfElements()  ;
  //  for(int i=0; i< nJet ; i++){
  //    ReconstructedParticle* rec=dynamic_cast<ReconstructedParticle*>(jetCol->getElementAt(i));
  //    
  //    HepLorentzVector v( rec->getMomentum()[0],
  //			  rec->getMomentum()[1],
  //			  rec->getMomentum()[2],
  //			  rec->getEnergy()   );
  //    
  //    vec_jets.push_back(v);
  //  }
  //  
  //  
  //  
  //  if(nJet >= 2){ // 2 a la base
  //    JetCombin test(nJet,2);
  //    vector<vector<int> > vec_com;
  //    vec_com = test.getCombinVec();
  //    
  //    vector<vector<int> >::const_iterator row;
  //    for (row = vec_com.begin(); row != vec_com.end(); row++) {  
  //	HepLorentzVector p1 = vec_jets[row->at(0)];
  //    	HepLorentzVector p2 = vec_jets[row->at(1)];
  //	
  //	
  //    }
  //    
  //  }
  //}
}

void LCJetFinder::end(){ 
  //_hbook->write_histograms(_histos.c_str());
  
  if (_output_tree) {
    _tree_file = _output_tree->GetCurrentFile();
    _tree_file->Write();
    delete _tree_file;
  }
  
}

