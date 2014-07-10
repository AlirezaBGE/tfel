/*!
 * \file   IntToType.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   06 Jun 2006
 */

#ifndef _LIB_TFEL_INTTOTYPE_H_
#define _LIB_TFEL_INTTOTYPE_H_ 

#include <string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \class  Int2Type
     * \brief  Int2Type assigns a type to an integer value.
     *
     * \param int, integer 
     *
     * This class was introduced to model units as described 
     * in David Abrahams and Aleksey Gurtovoy book
     * \latexonly\cite{abrahams04:_template_metaprogramming}\endlatexonly
     * \htmlonly C++ template metaprogramming\endhtmlonly.
     *
     * \author Helfer Thomas
     * \date   06 Jun 2006
     */
    template<int N>
    struct TFEL_VISIBILITY_LOCAL Int2Type{
      /*
       * Value of the template parameter
       */
      static const int value = N;
    };

    /*!
     * \class  UInt2Type
     * \brief  UInt2Type assigns a type to an positive integer value.
     *
     * \param unsigned int, integer 
     *
     * This class was introduced to model units as described 
     * in David Abrahams and Aleksey Gurtovoy book
     * \latexonly\cite{abrahams04:_template_metaprogramming}\endlatexonly
     * \htmlonly C++ template metaprogramming\endhtmlonly.
     *
     * \author Helfer Thomas
     * \date   06 Jun 2006
     */
    template<unsigned int N>
    struct TFEL_VISIBILITY_LOCAL UInt2Type{
      /*
       * Value of the template parameter
       */
      static const unsigned int value = N;
    };

  } // end of Namesapce math

} // end of namespace tfel


#endif /* _LIB_TFEL_INTTOTYPE_H */

