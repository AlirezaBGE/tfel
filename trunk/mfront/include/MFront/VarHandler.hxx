/*!
 * \file   VarHandler.hxx
 * \brief  This file declares the VarHandler class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONT_VARHANDLER_H_
#define _LIB_MFRONT_VARHANDLER_H_ 

#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"

namespace mfront
{

  /*!
   * structure standing for a variable
   * The support of fixed-sized arrays has been added lately.
   * This variable is considered as an array if arraySize is greater than 1.
   */
  struct TFEL_VISIBILITY_EXPORT VarHandler
  {
    //! type of the variable
    //  If the variable has been declared as an array (see below),
    //  this field holds the type contained by the array.
    std::string    type;
    //! name of the variable
    std::string    name;
    //! if the variable has been declared as an array, this field
    //  contains a value greater than 1
    unsigned short arraySize;
    //! line at wich the variable has been declared
    unsigned short lineNumber;
    /*!
     * Constructor
     * \param[in] type_      : variable type
     * \param[in] name_      : variable name
     * \param[in] arraySize_ : if greater than 1, this variable will
     * be considered as an array
     * \param[in] lineNumber_ : if greater than 1, will be considered as an array
     */
    VarHandler(const std::string&,
	       const std::string&,
	       const unsigned short,
	       const unsigned short);

  }; // end of struct MFrontParserBase<Child>::VarHandler

  typedef std::vector<VarHandler>  VarContainer;

} // end of namespace mfront

#endif /* _LIB_MFRONT_VARHANDLER_H */

