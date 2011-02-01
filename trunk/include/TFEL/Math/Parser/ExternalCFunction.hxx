/*!
 * \file   ExternalCFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX_
#define _LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX_ 

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

      struct ExternalCFunctionException
      {
	static void
	throwUnimplementedDifferentiateFunctionException(void);
	static void
	throwInvalidCreateFunctionByChangingParametersIntoVariables(void);
	static void
	throwInvalidVariableIndex(const std::vector<double>::size_type,
				  const unsigned short);
      }; // end of struct ExternalCFunctionException

      template<unsigned short N>
      struct ExternalCFunctionBase
	: public ExternalFunction,
	  protected ExternalCFunctionException
      {
	void
	setVariableValue(const std::vector<double>::size_type,
			 const double);
	std::vector<double>::size_type
	getNumberOfVariables(void) const;
	void
	checkCyclicDependency(const std::string&) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::string&) const;
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
	virtual ~ExternalCFunctionBase();
      protected:
	double variables[N];
      }; // end of struct ExternalCFunction

      template<unsigned short N>
      ExternalCFunctionBase<N>::~ExternalCFunctionBase()
      {} // end of ExternalCFunctionBase<N>::~ExternalCFunctionBase

      template<unsigned short N>
      void
      ExternalCFunctionBase<N>::getParametersNames(std::set<std::string>&) const
      {} // end of ExternalCFunctionBase<N>::getParametersNames


      template<unsigned short N>
      void
      ExternalCFunctionBase<N>::checkCyclicDependency(const std::string&) const
      {} // end of ExternalCFunctionBase::checkCyclicDependency

      template<unsigned short N>
      void
      ExternalCFunctionBase<N>::checkCyclicDependency(std::vector<std::string>&) const
      {} // end of ExternalCFunctionBase::checkCyclicDependency

      template<unsigned short N>
      tfel::utilities::shared_ptr<ExternalFunction>
      ExternalCFunctionBase<N>::differentiate(const std::vector<double>::size_type) const
      {
	using namespace tfel::utilities;
	ExternalCFunctionBase::throwUnimplementedDifferentiateFunctionException();
	return shared_ptr<ExternalFunction>(static_cast<ExternalFunction*>(0));
      } // end of ExternalCFunctionBase<N>::differentiate

      template<unsigned short N>
      tfel::utilities::shared_ptr<ExternalFunction>
      ExternalCFunctionBase<N>::createFunctionByChangingParametersIntoVariables(const std::vector<std::string>&) const
      {
	using namespace tfel::utilities;
	ExternalCFunctionException::throwInvalidCreateFunctionByChangingParametersIntoVariables();
	return shared_ptr<ExternalFunction>(static_cast<ExternalFunction*>(0));
      }

      template<unsigned short N>
      tfel::utilities::shared_ptr<ExternalFunction>
      ExternalCFunctionBase<N>::createFunctionByChangingParametersIntoVariables(std::vector<std::string>& v,
										const std::vector<double>&,
										const std::vector<std::string>&,
										const std::map<std::string,
										std::vector<double>::size_type>&) const
      {
	v.clear();
	return this->resolveDependencies();
      } // end of ExternalCFunctionBase<N>::createFunctionByChangingParametersIntoVariables

      template<unsigned short N>
      tfel::utilities::shared_ptr<ExternalFunction>
      ExternalCFunctionBase<N>::differentiate(const std::string&) const
      {
	using namespace tfel::utilities;
	ExternalCFunctionBase::throwUnimplementedDifferentiateFunctionException();
	return shared_ptr<ExternalFunction>(static_cast<ExternalFunction*>(0));
      } // end of ExternalCFunctionBase<N>::differentiate

      template<unsigned short N>
      void
      ExternalCFunctionBase<N>::setVariableValue(const std::vector<double>::size_type pos,
						 const double value)
      {
	using namespace std;
	if(pos>=N){
	  ExternalCFunctionException::throwInvalidVariableIndex(pos,N);
	}
	this->variables[pos] = value;
      } // end of ExternalCFunctionBase::setVariableValue;

      template<unsigned short N>
      std::vector<double>::size_type
      ExternalCFunctionBase<N>::getNumberOfVariables(void) const
      {
	return N;
      } // end of ExternalCFunctionBase<N>::getNumberOfVariables(void) const

      template<>
      struct ExternalCFunctionBase<0u>
	: public ExternalFunction
      {
	void
	setVariableValue(const std::vector<double>::size_type,
			 const double);
	std::vector<double>::size_type
	getNumberOfVariables(void) const;
	void
	checkCyclicDependency(const std::string&) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::string&) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(std::vector<std::string>&,
							const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(const std::vector<std::string>&) const;
	void
	getParametersNames(std::set<std::string>&) const;
      }; // end of struct ExternalCFunction

      template<unsigned short N>
      struct ExternalCFunction
      {}; // end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<0u>
	: public ExternalCFunctionBase<0u>
      {
	typedef tfel::system::CFunction0Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<1u>
	: public ExternalCFunctionBase<1u>
      {
	typedef tfel::system::CFunction1Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<2u>
	: public ExternalCFunctionBase<2u>
      {
	typedef tfel::system::CFunction2Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<3u>
	: public ExternalCFunctionBase<3u>
      {
	typedef tfel::system::CFunction3Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<4u>
	: public ExternalCFunctionBase<4u>
      {
	typedef tfel::system::CFunction4Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<5u>
	: public ExternalCFunctionBase<5u>
      {
	typedef tfel::system::CFunction5Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<6u>
	: public ExternalCFunctionBase<6u>
      {
	typedef tfel::system::CFunction6Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<7u>
	: public ExternalCFunctionBase<7u>
      {
	typedef tfel::system::CFunction7Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<8u>
	: public ExternalCFunctionBase<8u>
      {
	typedef tfel::system::CFunction8Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<9u>
	: public ExternalCFunctionBase<9u>
      {
	typedef tfel::system::CFunction9Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<10u>
	: public ExternalCFunctionBase<10u>
      {
	typedef tfel::system::CFunction10Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<11u>
	: public ExternalCFunctionBase<11u>
      {
	typedef tfel::system::CFunction11Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<12u>
	: public ExternalCFunctionBase<12u>
      {
	typedef tfel::system::CFunction12Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<13u>
	: public ExternalCFunctionBase<13u>
      {
	typedef tfel::system::CFunction13Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<14u>
	: public ExternalCFunctionBase<14u>
      {
	typedef tfel::system::CFunction14Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct TFEL_VISIBILITY_EXPORT ExternalCFunction<15u>
	: public ExternalCFunctionBase<15u>
      {
	typedef tfel::system::CFunction15Ptr FunctionPtr;
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction
 
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX */
