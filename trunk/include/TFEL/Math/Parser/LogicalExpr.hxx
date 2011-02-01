/*!
 * \file   LogicalExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 */

#ifndef _LIB_TFEL_LOGICALEXPR_HXX_
#define _LIB_TFEL_LOGICALEXPR_HXX_ 

#include<set>
#include<vector>
#include<string>
#include<stdexcept>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Parser/Expr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct OpEqual
      {
	static  bool TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpEqual

      struct OpGreater
      {
	static bool TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpGreater

      struct OpGreaterOrEqual
      {
	static bool TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpGreaterOrEqual

      struct OpLesser
      {
	static bool TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpLess

      struct OpLesserOrEqual
      {
	static bool TFEL_VISIBILITY_LOCAL
	apply(const double,const double);
      }; // end of struct OpLessOrEqual

      struct OpAnd
      {
	static bool TFEL_VISIBILITY_LOCAL
	apply(const bool,const bool);
      }; // end of struct OpAnd

      struct OpOr
      {
	static bool
	apply(const bool,const bool);
      }; // end of struct OpOr

      struct LogicalExpr
      {
      	virtual bool getValue(void) const = 0;
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const = 0;
	virtual tfel::utilities::shared_ptr<LogicalExpr>
	resolveDependencies(const std::vector<double>&) const = 0;
	virtual tfel::utilities::shared_ptr<LogicalExpr>
	clone(const std::vector<double>&) const = 0;
	virtual void
	getParametersNames(std::set<std::string>&) const = 0;
	virtual tfel::utilities::shared_ptr<LogicalExpr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const = 0;
	virtual ~LogicalExpr();
      }; // end of struct LogicalExpr

      template<typename Op>
      struct TFEL_VISIBILITY_LOCAL LogicalOperation
	: public LogicalExpr
      {
	LogicalOperation(const tfel::utilities::shared_ptr<Expr>,
			 const tfel::utilities::shared_ptr<Expr>);
	bool
	getValue(void) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	resolveDependencies(const std::vector<double>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	clone(const std::vector<double>&) const;
	void
	getParametersNames(std::set<std::string>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	~LogicalOperation();
      private:
	const tfel::utilities::shared_ptr<Expr> a;
	const tfel::utilities::shared_ptr<Expr> b;
      }; // end of struct LogicalOperation

      template<typename Op>
      struct TFEL_VISIBILITY_LOCAL LogicalBinaryOperation
	: public LogicalExpr
      {
	LogicalBinaryOperation(const tfel::utilities::shared_ptr<LogicalExpr>,
			       const tfel::utilities::shared_ptr<LogicalExpr>);
	bool
	getValue(void) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	resolveDependencies(const std::vector<double>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	clone(const std::vector<double>&) const;
	void
	getParametersNames(std::set<std::string>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	~LogicalBinaryOperation();
      private:
	const tfel::utilities::shared_ptr<LogicalExpr> a;
	const tfel::utilities::shared_ptr<LogicalExpr> b;
      }; // end of struct LogicalBinaryOperation

      struct TFEL_VISIBILITY_LOCAL NegLogicalExpression
	: public LogicalExpr
      {
	NegLogicalExpression(const tfel::utilities::shared_ptr<LogicalExpr>);
	bool
	getValue(void) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	resolveDependencies(const std::vector<double>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	clone(const std::vector<double>&) const;
	void
	getParametersNames(std::set<std::string>&) const;
	tfel::utilities::shared_ptr<LogicalExpr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	~NegLogicalExpression();
      private:
	const tfel::utilities::shared_ptr<LogicalExpr> a;
      }; // end of struct LogicalBinaryOperation

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/LogicalExpr.ixx"

#endif /* _LIB_TFEL_LOGICALEXPR_HXX_ */
