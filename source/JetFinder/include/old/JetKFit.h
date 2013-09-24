#ifndef _JETCOMBIN_H
#define _JETCOMBIN_H

#include <TMath.h>
#include <iostream>
#include <vector>
#include <map>

#include <CLHEP/Vector/LorentzVector.h>
using namespace CLHEP;
/** 
    \class JetKFitter
    \author  Y.Haddad
    \date July 2013
    \version 0.1
    
    \brief Jet-Combination for jet reconstruction 
    
    <h2> Description</h2>
    JetKFitter test(8,3);
    vector<vector<int> > vec;
    vec = test.getCombinVec();
    
    vector<vector<int> >::const_iterator row;
    vector<int>::const_iterator col;
    for (row = vec.begin(); row != vec.end(); row++) {
      for (col = row->begin(); col != row->end(); col++) {
        cout << *col;
      }
      cout << endl;
    }
    
    std::cout << "com size == "<< vec.size() << std::endl;
    return 0;
    
*/

using namespace std;

//class JetKFitter_base{
//  JetKFitter(int n, int k, int offset=0, bool print=false);
//  
//};

class JetKFitter
{
public:
  JetKFitter(int n, int k, int offset=0, bool print=false);
  virtual ~JetKFitter(){;};
  
  vector<vector<int> > getCombinVec();
  int  getNCombin(int n,int k);
  void go(int offset, int k);
  void pretty_print(const vector<int>& v);
  double chi2fun(HepLorentzVector dijet, double m, double sigma);
  double chi2fun_zh(HepLorentzVector dijet,                                                
		    double m_z,                                                            
		    double m_h,                                                            
		    double sigma_z ,                                                       
		    double sigma_h );
  
  std::vector<int> Fitter( std::vector<HepLorentzVector > jets );
  //static JetKFitter* instance();
private:
  bool _print;
  vector<int> _data;
  vector<int> _combination;
  vector<vector<int> > _out; 
  
  //static JetKFitter* _me;
};

#endif
