#include "JetCombin.h"

#include <TROOT.h>
#include <TStyle.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <CLHEP/Vector/LorentzVector.h>

using namespace CLHEP;
using namespace std;

//JetCombin* JetCombin::_me = 0;
//JetCombin* JetCombin::instance()
//{
//  if( _me == 0 )
//    _me = new JetCombin;
//  return _me;
//}

JetCombin::JetCombin(int n, int k, int offset, bool pr)
{
  std::cout << "Number of combination C("<< n<<","<< k<<") :: "<< getNCombin(n,k) << std::endl;
  for (int i = 0; i < n; ++i) { _data.push_back(i); }
  go(offset,k);
}

vector<vector<int> > JetCombin::getCombinVec()
{
  return _out;
}

void JetCombin::pretty_print(const vector<int>& v) {
  static int count = 0;
  cout << "combination no " << (++count) << ": [ ";
  for (unsigned int i = 0; i < v.size(); ++i) { cout << v[i] << " "; }
  cout << "] " << endl;
}

int  JetCombin::getNCombin(int n, int k)
{
  return TMath::Factorial(n) / (TMath::Factorial(n-k) * TMath::Factorial(k) );
}

void JetCombin::go(int offset, int k) {
  if (k == 0) {
    if(_print) pretty_print(_combination);
    _out.push_back(_combination);
    return;
  }
  for (unsigned int i = offset; i <= _data.size() - k; ++i) {
    _combination.push_back(_data[i]);
    go(i+1, k-1);
    _combination.pop_back();
  }
  
}

double JetCombin::chi2fun_zh(HepLorentzVector dijet, 
			     double m_z, 
			     double m_h,
			     double sigma_z ,
			     double sigma_h )
{
  double m_rec = sqrt( 250*250 + dijet.m()*dijet.m() - 2* dijet.e() * 250 );
  return pow(( dijet.m() - m_z ) / sigma_z ,2) + pow(( m_rec - m_h ) / sigma_h ,2);
}

double JetCombin::chi2fun(HepLorentzVector dijet, double m, double sigma)
{
  return pow(( dijet.m() - m ) / sigma ,2); 
}


std::vector<int> JetCombin::Fitter( std::vector<HepLorentzVector > jets ){
  int nJet = jets.size();
  std::map<double, std::vector<int> > chi2map;
  
  JetCombin combin(nJet,2);
  vector<vector<int> > vec_com;
  vec_com = combin.getCombinVec();
  double minChi2 = 9999999. ;
  vector<vector<int> >::const_iterator row;
  for (row = vec_com.begin(); row != vec_com.end(); row++) {
    HepLorentzVector p1 = jets[row->at(0)];
    HepLorentzVector p2 = jets[row->at(1)];
    
    HepLorentzVector pij = p1  +  p2;
    //double chi2 = chi2fun(pij, 80.4 , 2.1 );
    double chi2 = chi2fun_zh(pij, 90.1876, 125, 2.4952, 2.5); 
    std::vector<int> dijet_id;
    std::cout << "pair (i,j) == ("
	      <<  row->at(0) <<"," 
	      <<  row->at(1) << ")" 
	      <<"--> "<< chi2 <<std::endl;
    
    dijet_id.push_back(row->at(0));
    dijet_id.push_back(row->at(1));
    minChi2 = min(minChi2, chi2);
    
    chi2map.insert( std::pair<double,std::vector<int> >(chi2, dijet_id ));
  }
  //std::vector<int> z_dijet_id;
  
  return chi2map.find(minChi2)->second;
  //std::cout << "pair (i,j) == ("
  //	    <<  z_dijet_id[0] <<"," 
  //	    <<  z_dijet_id[1]<< ")" 
  //	    <<"--> "<< minChi2 <<std::endl;
  
}

