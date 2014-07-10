/*! 
 * \file  LUException.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 */

#ifndef _LIB_TFEL_MATH_LUEXCEPTION_H_
#define _LIB_TFEL_MATH_LUEXCEPTION_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Exception/TFELException.hxx"

#include<string>
 
namespace tfel
{

  namespace math
  {
    
    struct TFELMATH_VISIBILITY_EXPORT LUException
      : public tfel::exception::TFELException
    {
      LUException(const char* const);
      LUException(const std::string&);
    }; // end of struct LUException
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_LUEXCEPTION_H */

