/*!
 * \file   FAnderson.hxx
 * \brief    
 * \author Étienne Castelier
 * \date   10 oct. 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_FANDERSON_HXX
#define LIB_TFEL_MATH_FANDERSON_HXX

#include"TFEL/Math/AccelerationAlgorithms/AndersonBase.hxx"

namespace tfel{

  namespace math{

    template<typename Field,typename real>
    struct FAnderson
      : public AndersonBase<Field,real>
    {
      //! constructor
      FAnderson(const typename AndersonBase<Field,real>::Allocator);
      //! Displacement fields for a new iteration
      //! \param uO,uN Old and new displacement field
      void newIter(Field*&uO,Field*&uN,Field*&Df);
      //! First Iteration of a new time step
      //! \param uO,uN Old and new displacement field
      void restart(Field*&uO,Field*&uN,Field*&Df);
    }; // end of struct FAnderson
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/AccelerationAlgorithms/FAnderson.ixx"

#endif /* LIB_TFEL_MATH_FANDERSON_HXX */
