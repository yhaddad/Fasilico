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

//#include "fastjet/config.h"

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
  double dr;
  double eta;
  double et;
  double mt;
  double plus;
  double minus;
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
  
  /**
     cuts for jetfinder   
  */
  
  double _ycut;
  
  bool  _do_y_scan;
  bool  _force_cluster;
  int   _njet_forced;
  
  std::string _tree_file_name ;
  std::string _tree_name      ;
  
  rooDiJet _roo_dijet;
  TTree *_output_tree;
  int _nRun ;
  int _nEvt ;
} ;


#endif



