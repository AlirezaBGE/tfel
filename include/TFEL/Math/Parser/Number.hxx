/*!
 * \file   include/TFEL/Math/Parser/Number.hxx
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

#ifndef LIB_TFEL_NUMBER_HXX
#define LIB_TFEL_NUMBER_HXX

#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      struct Number final : public Expr {
        Number(const std::string&, const double);
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual std::shared_ptr<Expr> differentiate(
            const std::vector<double>::size_type,
            const std::vector<double>&) const override;
        virtual std::shared_ptr<Expr> clone(
            const std::vector<double>&) const override;
        virtual std::shared_ptr<Expr> resolveDependencies(
            const std::vector<double>&) const override;
        virtual void getParametersNames(std::set<std::string>&) const override;
        virtual std::shared_ptr<Expr>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        /*!
         * \return a string representation of the evaluator suitable to
         * be integrated in a C++ code.
         * \param[in] m: a map used to change the names of the variables
         */
        std::string getCxxFormula(
            const std::vector<std::string>&) const override;
        //! \return the number value
        double getValue() const override;

       private:
        Number& operator=(const Number&) = delete;
        Number& operator=(Number&&) = delete;
        //! string representation
        const std::string str;
        //! number value
        const double value;
      };  // end of struct Number

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_NUMBER_HXX */
