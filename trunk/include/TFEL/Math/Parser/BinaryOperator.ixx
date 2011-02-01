/*!
 * \file   BinaryOperator.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 oct 2007
 */

#ifndef _LIB_TFEL_BINARYOPERATOR_IXX_
#define _LIB_TFEL_BINARYOPERATOR_IXX_ 

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      template<typename Op>
      BinaryOperation<Op>::BinaryOperation(const tfel::utilities::shared_ptr<Expr> a_,
					   const tfel::utilities::shared_ptr<Expr> b_)
	: a(a_),b(b_)
      {} // end of BinaryOperation<Op>::BinaryOperation 
      
      template<typename Op>
      BinaryOperation<Op>::~BinaryOperation()
      {} // end of BinaryOperation<Op>::~BinaryOperation()
      
      template<typename Op>
      double
      BinaryOperation<Op>::getValue(void) const
      {
	return Op::apply(this->a->getValue(),this->b->getValue());
      } // end of BinaryOperation<Op>::getValue

      template<typename Op>
      void
      BinaryOperation<Op>::checkCyclicDependency(std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	using namespace std;
	using std::vector;
	vector<string> a_names;
	vector<string> b_names;
	this->a->checkCyclicDependency(a_names);
	this->b->checkCyclicDependency(b_names);
	mergeVariablesNames(names,a_names);
	mergeVariablesNames(names,b_names);
      } // end of BinaryOperation<Op>::checkCyclicDependency

      template<typename Op>
      void
      BinaryOperation<Op>::getParametersNames(std::set<std::string>& p) const
      {
	this->a->getParametersNames(p);
	this->b->getParametersNames(p);
      } // end of BinaryOperation<Op>::getParametersNames

      template<typename Op>
      tfel::utilities::shared_ptr<Expr>
      BinaryOperation<Op>::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return shared_ptr<Expr>(new BinaryOperation<Op>(this->a->resolveDependencies(v),
						      this->b->resolveDependencies(v)));
      } // end of BinaryOperation<Op>::resolveDependencies

      template<typename Op>
      tfel::utilities::shared_ptr<Expr>
      BinaryOperation<Op>::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return shared_ptr<Expr>(new BinaryOperation<Op>(this->a->clone(v),
						      this->b->clone(v)));
      } // end of BinaryOperation<Op>::clone

      template<typename Op>
      tfel::utilities::shared_ptr<Expr>
      BinaryOperation<Op>::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
									   const std::vector<std::string>& params,
									   const std::map<std::string,
									   std::vector<double>::size_type>& pos) const
      {
	using namespace tfel::utilities;
	shared_ptr<Expr> na(this->a->createFunctionByChangingParametersIntoVariables(v,params,pos));
	shared_ptr<Expr> nb(this->b->createFunctionByChangingParametersIntoVariables(v,params,pos));
	return shared_ptr<Expr>(new BinaryOperation<Op>(na,nb));
      } // end of BinaryOperation<Op>::createFunctionByChangingParametersIntoVariables

      template<typename BinaryOperation>
      tfel::utilities::shared_ptr<Expr>
      differentiateBinaryOperation(const tfel::utilities::shared_ptr<Expr>,
				   const tfel::utilities::shared_ptr<Expr>,
				   const std::vector<double>::size_type,
				   const std::vector<double>&)
      {
	using namespace std;
	using namespace tfel::utilities;
	BinaryOperationBase::throwUnimplementedDifferentiateFunctionException();
	return shared_ptr<Expr>(0);
      } // end of differentiateBinaryOperation

      template<>
      tfel::utilities::shared_ptr<Expr>
      differentiateBinaryOperation<OpPlus>(const tfel::utilities::shared_ptr<Expr>,
					   const tfel::utilities::shared_ptr<Expr>,
					   const std::vector<double>::size_type,
					   const std::vector<double>&);

      template<>
      tfel::utilities::shared_ptr<Expr>
      differentiateBinaryOperation<OpMinus>(const tfel::utilities::shared_ptr<Expr>,
					   const tfel::utilities::shared_ptr<Expr>,
					   const std::vector<double>::size_type,
					   const std::vector<double>&);


      template<>
      tfel::utilities::shared_ptr<Expr>
      differentiateBinaryOperation<OpMult>(const tfel::utilities::shared_ptr<Expr>,
					   const tfel::utilities::shared_ptr<Expr>,
					   const std::vector<double>::size_type,
					   const std::vector<double>&);


      template<>
      tfel::utilities::shared_ptr<Expr>
      differentiateBinaryOperation<OpDiv>(const tfel::utilities::shared_ptr<Expr>,
					  const tfel::utilities::shared_ptr<Expr>,
					  const std::vector<double>::size_type,
					  const std::vector<double>&);

      template<>
      tfel::utilities::shared_ptr<Expr>
      differentiateBinaryOperation<OpPower>(const tfel::utilities::shared_ptr<Expr>,
					    const tfel::utilities::shared_ptr<Expr>,
					    const std::vector<double>::size_type,
					    const std::vector<double>&);

      template<typename Op>
      tfel::utilities::shared_ptr<Expr>
      BinaryOperation<Op>::differentiate(const std::vector<double>::size_type pos,
					 const std::vector<double>& v) const
      {
	return differentiateBinaryOperation<Op>(this->a,this->b,pos,v);
      } // end of BinaryOperation<Op>::differentiate

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_BINARYOPERATOR_IXX */

