/*!
 * \file   src/Math/Variable.cxx
 * \brief  
 * 
 * \author Thomas Helfer
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>

#include"TFEL/Raise.hxx"
#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/Variable.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      Variable::Variable(const std::vector<double>& v_,
			 const std::vector<double>::size_type p_)
	: v(v_), pos(p_)
      {} // end of Variable::Variable

      double
      Variable::getValue() const
      {
	return this->v[this->pos];
      } // end of Variable::getValue

      void
      Variable::checkCyclicDependency(std::vector<std::string>&) const
      {} // end of Variable::checkCyclicDependency
      
      void
      Variable::getParametersNames(std::set<std::string>&) const
      {} // end of Variable::getParametersNames

      std::shared_ptr<Expr>
      Variable::differentiate(const std::vector<double>::size_type dpos,
			      const std::vector<double>&) const
      {
	if(dpos!=this->pos){
	  return std::shared_ptr<Expr>(new Number(0.));
	}
	return std::shared_ptr<Expr>(new Number(1.));
      } // end of Variable::differentiate

      std::shared_ptr<Expr>
      Variable::clone(const std::vector<double>& v_) const
      {
	return std::shared_ptr<Expr>(new Variable(v_,this->pos));
      } // end of Variable::clone

      std::shared_ptr<Expr>
      Variable::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v_,
								const std::vector<std::string>&,
								const std::map<std::string,
								std::vector<double>::size_type>&) const
      {
	raise_if(v_.size()<this->pos,
		 "Variable::createFunctionByChanging"
		 "ParametersIntoVariables: "
		 "invalid vector size");
	return std::shared_ptr<Expr>(new Variable(v_,this->pos));
      } // end of Variable::createFunctionByChangingParametersIntoVariables

      std::shared_ptr<Expr>
      Variable::resolveDependencies(const std::vector<double>& nv) const
      {
	return std::shared_ptr<Expr>(new Variable(nv,this->pos));
      } // end of Variable::resolveDependencies

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
