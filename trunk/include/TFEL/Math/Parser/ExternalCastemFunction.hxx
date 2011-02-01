/*!
 * \file   ExternalCastemFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_MATH_PARSER_EXTERNALCASTEMFUNCTION_HXX_
#define _LIB_TFEL_MATH_PARSER_EXTERNALCASTEMFUNCTION_HXX_ 

#include<sstream>
#include<vector>
#include<stdexcept>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct TFEL_VISIBILITY_EXPORT ExternalCastemFunction
	: public ExternalFunction
      {
	ExternalCastemFunction(const tfel::system::CastemFunctionPtr,
			       const unsigned short);
	void
	setVariableValue(const std::vector<double>::size_type,
			 const double);
	std::vector<double>::size_type
	getNumberOfVariables(void) const;
	double getValue(void) const;
	void
	checkCyclicDependency(const std::string&) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::string&) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(const std::vector<std::string>&) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(std::vector<std::string>&,
							const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	void
	getParametersNames(std::set<std::string>&) const;
      private:
	std::vector<double> variables;
	tfel::system::CastemFunctionPtr f;
      };// end of struct ExternalCastemFunction
 
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_PARSER_EXTERNALCASTEMFUNCTION_HXX_ */
