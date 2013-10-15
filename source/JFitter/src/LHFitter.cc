/** LHFitter.cc --- 		
 *
 * Copyright (C) 2013 Y. Haddad
 *
 * Author: Y. Haddad <hadddad@yacine>
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



namespace fasilico{
  //namespace KinFitter{
  
  //double Chi2JFitter::likelihood(CLHEP::HepLorentzVector dijet){
  //  double m_rec = sqrt( 250*250 + dijet.m()*dijet.m() - 2* dijet.e() * 250 );
  //  
  //  double L1 = TMath::Gaus(dijet.m(),91.187,2.4952); // Z test
  //  double L2 = 1.0 - TMath::Gaus(m_rec,91.187,2.4952); // recoil mass test
  //  if( L1 * L2 > 0)
  //    return -2*TMath::Log(L1 * L2) ;
  //  else 
  //    return 9999999.;
  //}
  //
  //void Chi2JFitter::setJetCollection(std::vector<CLHEP::HepLorentzVector> collection)
  //{
  //  //if(collection.size() < 2 ) return; 
  //  
  //  std::map<double, std::vector<int> > chi2map;
  //  std::vector<std::vector<int> > vec_com;
  //  double minChi2 = 9999999. ;
  //  int njets    = collection.size();
  //  JCombination combination(njets,2,0,true); // looking for dijets
  //  vec_com = combination.getConbinationVector();
  //  for (std::vector<std::vector<int> >::iterator row = vec_com.begin(); row != vec_com.end(); row++){
  //    
  //    HepLorentzVector p1 = collection[row->at(0)];
  //    HepLorentzVector p2 = collection[row->at(1)];
  //    
  //    HepLorentzVector pij = p1  +  p2;
  //    double chi2 = -1.;
  //    chi2 = this->likelihood(pij);
  //    //if(_function_1d != 0 ) chi2 = _function_1d->Eval(pij.m());
  //    //if(_function_2d != 0 ) chi2 = _function_2d->Eval(pij.m(),pij.m());
  //    std::vector<int> dijet_id;
  //    std::cout << "pair (i,j) == ("
  //		<<  row->at(0) <<"," 
  //		<<  row->at(1) << ")" 
  //		<<"--> "<< chi2 <<std::endl;
  //    
  //    dijet_id.push_back(row->at(0));
  //    dijet_id.push_back(row->at(1));
  //    minChi2 = std::min(minChi2, chi2);
  //    
  //    chi2map.insert( std::pair<double,std::vector<int> >(chi2, dijet_id ));
  //    
  //  }
  //  _minChi2    =  minChi2;
  //  _best_dijet =  chi2map.find(minChi2)->second;
  //}
  
  //}
}
