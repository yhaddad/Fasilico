/** Isolation.hh --- 		
 *
 * Copyright (C) 2013 haddad
 *
 * Author: haddad <hadddad@yacine>
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


#ifndef Isolation_hh
#define Isolation_hh 1

#include <string>
#include <iostream>

#include "EVENT/LCCollection.h"



namespace fasilico{
  /**
   * @brief Isolation base for isolation
   */
  class Isolation {     
  public:
    ///! constructor
    Isolation()  {;}
    ///! destructor
    virtual ~Isolation() {;}
    ///! @brief Load specifique PFO from LCCollection pointer 
    virtual void setCollection(EVENT::LCCollection* , const std:: string &) = 0;
    ///! @brief Load collection from an other framwork: not implemented yet !
    virtual void loadObject(void *) = 0;
    ///! @brief Return new collection of isolated PFO
    virtual EVENT::LCCollection *getIsolatedPFO() = 0;
  private:
    std::string PFOType;
    
  }; // Isolation class
} // namespace
#endif // Isolation_hh
