/*!
 * \file   OpDiff.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 f�v 2007
 */
#include<string>
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{
    
    std::string
    OpDiff::getName(void)
    {
      return "OpDiff";
    } // end of OpDiff

    namespace stdfunctions{

      TFELMATH_VISIBILITY_EXPORT OpDiff D TFEL_UNUSED_ATTRIBUTE;

    } // end of namespace stdfunctions
    
  } // end of namespace math

} // end of namespace tfel
