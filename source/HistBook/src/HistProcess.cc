#include "HistProcess.h"
#include <iostream>

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>

#include "marlin/VerbosityLevels.h"

using namespace lcio ;
using namespace marlin ;


HistProcess aHistProcess ;


HistProcess::HistProcess() : Processor("HistProcess") {
  _description = "HistProcess for histogramming" ;
  
  _historams_file="fasilico_check_plots.root";
  registerProcessorParameter( "histograms_file_name"  , 
                              "histograms file name"  ,
                              _historams_file         ,
			      _historams_file        );
}



void HistProcess::init() { 
  printParameters() ;
  book = gHistBooker->global();
}

void HistProcess::end(){ 
  book->write_histograms(_historams_file.c_str());
}

