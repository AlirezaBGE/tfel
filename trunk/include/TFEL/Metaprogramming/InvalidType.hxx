/*!
 * \file   InvalidType.hxx
 * \brief  This file declares InvalidType
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifndef _LIB_TFEL_INVALIDTYPE_H_
#define _LIB_TFEL_INVALIDTYPE_H_ 

#include<string>
#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \brief InvalidType is a type that is declared but not defined.
     * InvalidType is declared but not defined.
     * It is safe to make a typedef to InvalidType but it cannot be
     * instanciated. This type is generally used to give default and 
     * invalid values to fields in traits class.
     * \author Helfer Thomas
     * \date   28 Aug 2006     
     */
    struct InvalidType;

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_INVALIDTYPE_H */

