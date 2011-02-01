/*!
 * \file   OpDiff.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 f�v 2007
 */
#include<string>
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{
    
    const std::string
    OpDiff::getName(void)
    {
      return "OpDiff";
    } // end of OpDiff

    namespace stdfunctions{

      OpDiff D;

    } // end of namespace stdfunctions
    
  } // end of namespace math

} // end of namespace tfel
