#ifndef LCJetFinder_h
#define LCJetFinder_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include <TTree.h>
#include <TFile.h>

// ----- include for Jets Clustering ---------
#include <fastjet/ClusterSequence.hh>
#include <fastjet/PseudoJet.hh>

#include <TClonesArray.h>
#include <HistBooker.h>

//#ifdef FASTJET_ENABLE_PLUGIN_JADE
//#include "fastjet/JadePlugin.hh"
//#else
//#warning "Jade plugin not enabled. Skipping the example"
//#endif // FASTJET_ENABLE_PLUGIN_JADE






using namespace lcio ;
using namespace marlin ;
using namespace fastjet ;


/**  Example processor for marlin.
 * 
 *  <h4>Input - Prerequisites</h4>
 *  Needs the collection of MCParticles.
 *
 *  <h4>Output</h4> 
 *  A histogram.
 * 
 * @param CollectionName Name of the MCParticle collection
 * 
 * @author F. Gaede, DESY
 * @version $Id: LCJetFinder.h,v 1.4 2005-10-11 12:57:39 gaede Exp $ 
 */

typedef struct{
  int event;
  int njet;
  
  double m;
  double px;
  double py;
  double pz;
  double theta;
  double cosTheta;
  double E;
  double pt;
  double p;
  double mrecoil;
  double likelihood;
  double chi2;
  double dr;
  double eta;
  double et;
  double mt;
  double plus;
  double minus;
  double y34;
  double y12;
  double thrust;
  double cosThetaThrust;
  double acoplanarity;
  
}rooDiJet;


class LCJetFinder : public Processor {
  
 public:
  
  virtual Processor*  newProcessor() { return new LCJetFinder ; }
  
  
  LCJetFinder() ;
  
  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;
  
  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;
  
  /** Called for every event - the working horse.
   */
  virtual void processEvent( LCEvent * evt ) ; 
  
  
  virtual void check( LCEvent * evt ) ; 

  
  virtual void Zfinder(LCCollection* jetCol ) ; 
  
  /** Called after data processing for clean up.
   */
  virtual void end() ;
  
  
  
  /** function called for my analysis
   */
  //virtual void booktree(TTree *tree, rooDiJet dijet){
  //  
  //  tree->Branch("event"   ,&dijet.event         ,"event/I");
  //  tree->Branch("njets"   ,&dijet.njet          ,"njets/I");
  //  
  //  tree->Branch("m"       ,&dijet.m        ,"m/D");
  //  tree->Branch("eta"     ,&dijet.eta      ,"eta/D");
  //  tree->Branch("et"      ,&dijet.et       ,"et/D");
  //  tree->Branch("mt"      ,&dijet.mt       ,"mt/D");
  //  tree->Branch("plus"    ,&dijet.plus     ,"plus/D");
  //  tree->Branch("minus"   ,&dijet.minus    ,"minus/D");
  //  tree->Branch("px"      ,&dijet.px       ,"px/D");
  //  tree->Branch("py"      ,&dijet.py       ,"py/D");
  //  tree->Branch("pz"      ,&dijet.pz       ,"pz/D");
  //  tree->Branch("E"       ,&dijet.E        ,"E/D");
  //  tree->Branch("pt"      ,&dijet.pt       ,"pt/D");
  //  tree->Branch("p"       ,&dijet.p        ,"p/D");
  //  tree->Branch("mrecoil" ,&dijet.mrecoil  ,"mrecoil/D");
  //  tree->Branch("dr"      ,&dijet.dr       ,"dr/D");
  //  tree->Branch("chi2"    ,&dijet.chi2     ,"chi2/D");
  //  tree->Branch("cosTheta",&dijet.cosTheta ,"cosTheta/D");
  //  tree->Branch("theta"   ,&dijet.theta    ,"theta/D");
  //  tree->Branch("likelihood"    ,&dijet.likelihood    ,"likelihood/D");
  //}




  //void dijet_analyzer(vector<fastjet::PseudoJet> jets);
  
  //virtual void process_finalstat_particles(std::vector<EVENT::MCParticle> p){;}
  //virtual void process_initial_particles(std::vector<EVENT::MCParticle> p){;}
  
    
  
  
 protected:

  /** Input collection name.
   */
  std::string _histos  ;
  
  /**  Input collection name */
  std::string _inputCollectionName ;
  
  /**  
       Ouput collection names 
  */
  std::string _recoParticleCollectionName ;
  std::string _mcTruthCollectionName ;
  
  HistBooker *_hbook;  
  /**
     cuts for jetfinder   
  */
  
  double _ycut;
  
  bool  _do_y_scan;
  bool  _force_cluster;
  int   _njet_forced;
  
  std::string _tree_file_name ;
  std::string _tree_name      ;
  
  TFile *_tree_file;
  
  TClonesArray *_JTupleArray;
  std::map<std::string,int> _counters;
  rooDiJet _roo_dijet;
  TTree *_output_tree;
  TTree *_bestdijet_tree;
  int _nRun ;
  int _nEvt ;
} ;


#endif



