/*!
 * \file   StensorConcept.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   02 janv. 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Raise.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math{

    unsigned short getStensorSize(const unsigned short d){
      if(d==1u){
	return 3u;
      }
      if(d==2u){
	return 4u;
      }
      if(d==3u){
	return 6u;
      }
      raise("getStensorSize: invalid dimension");
    }

  } // end of namespace math

} // end of namespace tfel
