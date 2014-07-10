/*!
 * \file   LogicalExpr.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 jan 2009
 */

#include<cmath>
#include<limits>

#include"TFEL/Math/Parser/LogicalExpr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      bool
      OpEqual::apply(const double a,
		     const double b)
      {
	using namespace std;
	return abs(a-b) < 10*numeric_limits<double>::min();
      } // end of OpEqual::apply

      bool
      OpGreater::apply(const double a,
		       const double b)
      {
	return a > b;
      } // end of OpGreater::apply

      bool
      OpGreaterOrEqual::apply(const double a,
			      const double b)
      {
	return a >= b;
      } // end of OpGreaterOrEqual::apply

      bool
      OpLesser::apply(const double a,
		      const double b)
      {
	return a < b;
      } // end of OpLesser::apply

      bool
      OpLesserOrEqual::apply(const double a,
			     const double b)
      {
	return a <= b;
      } // end of OpLesserOrEqual::apply

      bool
      OpAnd::apply(const bool a,
		   const bool b)
      {
	return a && b;
      } // end of OpAnd::apply
      
      bool
      OpOr::apply(const bool a,
		  const bool b)
      {
	return a || b;
      } // end of OpAnd::apply
      
      LogicalExpr::~LogicalExpr()
      {} // end of LogicalExpr::~LogicalExpr()

      NegLogicalExpression::NegLogicalExpression(const tfel::utilities::shared_ptr<LogicalExpr> a_)
	: a(a_)
      {} // end of NegLogicalExpression::NegLogicalOperation

      bool
      NegLogicalExpression::getValue(void) const
      {
	return !this->a->getValue();
      } // end of NegLogicalExpression::getValue(void) const
      
      void
      NegLogicalExpression::checkCyclicDependency(std::vector<std::string>& names) const
      {
	this->a->checkCyclicDependency(names);
      } // end of NegLogicalExpression::checkCyclicDependency
      
      tfel::utilities::shared_ptr<LogicalExpr>
      NegLogicalExpression::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return shared_ptr<LogicalExpr>(new NegLogicalExpression(this->a->resolveDependencies(v)));
      } // end of NegLogicalExpression::resolveDependencies(void) const
      
      tfel::utilities::shared_ptr<LogicalExpr>
      NegLogicalExpression::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return shared_ptr<LogicalExpr>(new NegLogicalExpression(this->a->clone(v)));
      }

      tfel::utilities::shared_ptr<LogicalExpr>
      NegLogicalExpression::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
										  const std::vector<std::string>& p,
										  const std::map<std::string,
										  std::vector<double>::size_type>& pos) const
      {
	using namespace tfel::utilities;
	shared_ptr<LogicalExpr> na = this->a->createFunctionByChangingParametersIntoVariables(v,p,pos);
	return shared_ptr<LogicalExpr>(new NegLogicalExpression(na));
      }

      void
      NegLogicalExpression::getParametersNames(std::set<std::string>& p) const
      {
	this->a->getParametersNames(p);
      } // end of NegLogicalExpression::getParametersNames
      
      NegLogicalExpression::~NegLogicalExpression()
      {}

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
