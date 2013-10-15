/** JCombination.hh --- 		
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


#ifndef JCombination_hh
#define JCombination_hh 1

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <TMath.h>

namespace fasilico{
  //namespace JFitter{
    /** 
     *   \class JetCombin
     *   \version 0.1
     *   
     *   \brief Jet-Fitter combinations
     *   
     *   <h2> Description</h2>
     *   example :
     *   JetCombin test(8,3);
     *   vector<vector<int> > vec;
     *   vec = test.getCombinVec();
     *   
     *   vector<vector<int> >::const_iterator row;
     *   vector<int>::const_iterator col;
     *   for (row = vec.begin(); row != vec.end(); row++) {
     *     for (col = row->begin(); col != row->end(); col++) {
     *       cout << *col;
     *     }
     *     cout << endl;
     *   }
     *   
     *   std::cout << "com size == "<< vec.size() << std::endl;
     *   return 0;
     */
  
  
    class JCombination
    {
    public:
      /**
       * @brief constructor
       */
      JCombination(){};
      JCombination(int n, int k, int offset=0, bool print=false);
      
      /**
       * @brief destructor
       */
      virtual ~JCombination(){};
      /**
       * @brief print jet combination
       */
      virtual void print(const std::vector<int>& v);
      
      /**
       * @brief get the combination index.
       */
      virtual std::vector<std::vector<int> > getConbinationVector()
      {
	std::cout << "JCombination::Ncombination == "<< _out.size() << std::endl;
	return _out;
      }
      virtual int getNJCombination(); 
    
      /**
       * @brief some set function 
       */ 
    
      void setOffset(int offset){_offset = offset;}
      void setCombinationIndex(int n, int k){_n = n; _k = k;}
      void setPrintFlag(bool print=false){_print_flag = print;}
      
      //std::vector<std::vector<int> > getConbinationVector();
    
      void reset    (){_data.clear();_out.clear();_combination.clear();_n=_k=0;}
    
      //static JCombination* instance();
    private:
      void go(int offset, int k);
      int _offset;
      bool _print_flag;
      
      int _n,_k;
    
      std::vector<int> _data;
      std::vector<int> _combination;
      std::vector<std::vector<int> > _out;
    
    
      //static JCombination* _me;
    }; // JCombination class
  //  }// namespace JFitter
}// namespace
#endif // JCombination_hh


