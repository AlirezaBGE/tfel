/*! 
 * \file  mtest/include/MTest/AsterFiniteStrainBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_MTESTASTERFINITESTRAINBEHAVIOUR_HXX
#define LIB_MTEST_MTESTASTERFINITESTRAINBEHAVIOUR_HXX 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MTest/AsterStandardBehaviour.hxx"

namespace mtest
{

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AsterFiniteStrainBehaviour
    : public AsterStandardBehaviour
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AsterFiniteStrainBehaviour(const Hypothesis,
			       const std::string&,
			       const std::string&);
    /*!
     * \param[out] v : initial values of the driving variables
     */
    void getGradientsDefaultInitialValues(tfel::math::vector<real>&) const override;
    /*!
     * \brief allocate internal workspace
     * \param[out] wk : work space
     */
    void allocate(BehaviourWorkSpace&) const override;
    //! destructor
    ~AsterFiniteStrainBehaviour() override;
  protected:
    /*!
     * \brief call the mechanical behaviour
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out]    Kt    : tangent operator
     * \param[in,out] s     : current state
     * \param[out]    wk    : workspace
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     * \param[in]     b     : if true, integrate the behaviour over the time
     * step, if false compute a prediction of the stiffness matrix
     */
    virtual std::pair<bool,real>
    call_behaviour(tfel::math::matrix<real>&,
		   CurrentState&,
		   BehaviourWorkSpace&,
		   const real,
		   const StiffnessMatrixType,
		   const bool) const override;
    //! finite strain formulation
    unsigned short afsf = 0u;
  }; // end of struct Behaviour
  
} // end of namespace mtest

#endif /* LIB_MTEST_MTESTASTERFINITESTRAINBEHAVIOUR_HXX */
